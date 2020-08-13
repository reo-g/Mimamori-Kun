#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AE_SHT31.h"
#include <PanasonicHeatpumpIR.h>

//宣言
void door_buzzer();
void air_cool_on(int target_temp);
void air_cool_off();
void get_env_info();
void wbgt_check();
void emergency();
void heatstroke_alert();
void soracom_send_single();
void soracom_send_double();
void soracom_send_long();

//ピン設定
const int buzzer_pin = 9;
const int emergency_button = 10;
const int soracom_signal_pin = 8;
const int door_sensor_pin = 12;

AE_SHT31 SHT31 = AE_SHT31(0x45);
Adafruit_SSD1306 display(128, 64, &Wire, -1);


void setup() {
  // シリアル通信を9600bpsに設定
  Serial.begin(9600);
  // シリアルに文字を出力
  Serial.println("SHT31 Test!!");
  // SHT31をソフトリセット
  SHT31.SoftReset();
  // 内蔵ヒーター 0:OFF 1:ON
  SHT31.Heater(0);

  pinMode(emergency_button,INPUT_PULLUP);
  pinMode(door_sensor_pin,INPUT_PULLUP);
  pinMode(soracom_signal_pin, OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();

  tone(buzzer_pin,523,300);
  delay(300);
  
}

//変数
float temp,humi,wbgt;
unsigned long open_start_time;
unsigned long last_heatalert_time = 0;
boolean door_recorded = false;
int door_elapsed_time = 0;
int b4_elapsed_time = 100;
int buzzer_status = 0;

#define BUZZER_LEN 200

void loop() {
  if (digitalRead(emergency_button) == LOW){ //緊急ボタンを押された
  emergency();
  }
  if(digitalRead(door_sensor_pin) == HIGH){ //冷蔵庫の扉が開かれた
    if(door_recorded == false){
      open_start_time = millis(); //冷蔵庫の扉が開かれた時の時刻を記録
      door_recorded = true;
      soracom_send_single(); 
    }else{
      door_elapsed_time = (millis() - open_start_time)/1000;
      if(door_elapsed_time != b4_elapsed_time){      
        display.clearDisplay();
        display.setCursor(0,0);
        display.setTextSize(2);
        display.print(door_elapsed_time); display.println(F(" sec"));
        display.display();
        door_buzzer();
        b4_elapsed_time = door_elapsed_time;
      }
    }
  }else{ //冷蔵庫の扉が開かれていない場合
    if(door_recorded == true){
      if(buzzer_status >= 8){
        noTone(buzzer_pin);
      }
      door_recorded = false;
      b4_elapsed_time = 100;
      buzzer_status = 0;
    }
    get_env_info();
    if((millis() - last_heatalert_time)/1000 >= 1800){ //30分が経過して再確認
    wbgt_check();
    last_heatalert_time = millis();
    }
  }
}

void door_buzzer(){
  if(door_elapsed_time >= 25 && buzzer_status == 0){
    tone(buzzer_pin,262,BUZZER_LEN); // ド
    buzzer_status = 1;
  }
  if(door_elapsed_time >= 30 && buzzer_status == 1){
    tone(buzzer_pin,294,BUZZER_LEN); // レ
    buzzer_status = 2;
  }
  if(door_elapsed_time >= 35 && buzzer_status == 2){
    tone(buzzer_pin,330,BUZZER_LEN); // ミ
    buzzer_status = 3;
  }
  if(door_elapsed_time >= 40 && buzzer_status == 3){
    tone(buzzer_pin,349,BUZZER_LEN); // ファ
    buzzer_status = 4;
  }
  if(door_elapsed_time >= 45 && buzzer_status == 4){
    tone(buzzer_pin,392,BUZZER_LEN); // ソ
    buzzer_status = 5;
  }
  if(door_elapsed_time >= 50 && buzzer_status == 5){
    tone(buzzer_pin,440,BUZZER_LEN); // ラ
    buzzer_status = 6;
  }
  if(door_elapsed_time >= 55 && buzzer_status == 6){
    tone(buzzer_pin,494,BUZZER_LEN); // シ
    buzzer_status = 7;
  }
  if(door_elapsed_time >= 60 && buzzer_status == 7){
    tone(buzzer_pin,523); // ド
    buzzer_status = 8;
  }
  if(door_elapsed_time >= 60 && buzzer_status == 7){
    tone(buzzer_pin,523); // ド
    buzzer_status = 8;
  }
  if(door_elapsed_time >= 3600 && buzzer_status == 8){ //1時間も開けっ放しなのは明らかに緊急事態
    emergency();
    buzzer_status = 9;
  }
}

void air_cool_on(int target_temp=26){
  heatstroke_alert();
  IRSenderPWM irSender(3); // irSender(IRLEDpinNum);
  PanasonicDKEHeatpumpIR *heatpumpIR;
  heatpumpIR = new PanasonicDKEHeatpumpIR();
  heatpumpIR->send(irSender, POWER_ON, MODE_COOL, FAN_AUTO, target_temp, VDIR_AUTO, HDIR_AUTO);
  
}


void air_cool_off(){
  IRSenderPWM irSender(3); // irSender(IRLEDpinNum);
  PanasonicDKEHeatpumpIR *heatpumpIR;
  heatpumpIR = new PanasonicDKEHeatpumpIR();
  heatpumpIR->send(irSender, POWER_OFF, MODE_COOL, FAN_AUTO, 26, VDIR_AUTO, HDIR_AUTO);
  
}

void get_env_info() {
  SHT31.GetTempHum();
  
  temp = SHT31.Temperature();
  humi = SHT31.Humidity();

  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.print(temp); display.println(F(" C"));
  display.print(humi); display.println(F(" %"));
  display.display();
}

void wbgt_check(){
  wbgt = 0.735*temp+0.0374*humi+0.00292*temp*humi-4.064;
  if(wbgt>31){ //WBGT値が厳重警戒基準値に達したらエアコン24℃
    air_cool_on(24);
  }else if(wbgt>28){ //WBGT値が警戒値に達したらエアコン26℃
    air_cool_on(26);
  }
}

void emergency(){
  tone(buzzer_pin,1046,3000);
  soracom_send_long();
}

void heatstroke_alert(){
  tone(buzzer_pin,900,3000);
  soracom_send_double();
}

void soracom_send_single(){ //シングルクリック<1>　扉開閉時
  digitalWrite(soracom_signal_pin, HIGH);
  delay(500);
  digitalWrite(soracom_signal_pin, LOW);
}

void soracom_send_double(){ //ダブルクリック<2>　熱中症危険時
  digitalWrite(soracom_signal_pin, HIGH);
  delay(500);
  digitalWrite(soracom_signal_pin, LOW);
  delay(500);
  digitalWrite(soracom_signal_pin, HIGH);
  delay(500);
  digitalWrite(soracom_signal_pin, LOW);
}

void soracom_send_long(){ //長押し入力<3>　緊急ボタン押下時
  digitalWrite(soracom_signal_pin, HIGH);
  delay(1500);
  digitalWrite(soracom_signal_pin, LOW);
}
