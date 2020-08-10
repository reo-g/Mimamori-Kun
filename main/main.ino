#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AE_SHT31.h"


//ピン設定
const int buzzer_pin = 4;
const int emergency_button = 5;
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
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();

  tone(buzzer_pin,523,300);
  delay(300);
  
}

//変数
float temp,humi;
unsigned long open_start_time;
boolean door_recorded = false;
int door_elapsed_time = 0;
int b4_elapsed_time = 100;

void loop() {
  if (digitalRead(emergency_button) == LOW){ //緊急ボタンを押された
  emergency();
  }
  if(digitalRead(door_sensor_pin) == HIGH){ //冷蔵庫の扉が開かれた
    if(door_recorded == false){
      open_start_time = millis(); //冷蔵庫の扉が開かれた時の時刻を記録
      door_recorded = true; 
    }else{
      door_elapsed_time = (millis() - open_start_time)/1000;
      if(door_elapsed_time != b4_elapsed_time){      
        display.clearDisplay();
        display.setCursor(0,0);
        display.setTextSize(2);
        display.print(door_elapsed_time); display.println(F(" sec"));
        display.display();
        b4_elapsed_time = door_elapsed_time;
      }
    }
  }else{ //冷蔵庫の扉が開かれていない場合
    if(door_recorded == true){
      door_recorded = false;
      b4_elapsed_time = 100;
    }
    get_env_info();
  }
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

void emergency(){
  tone(buzzer_pin,1046,3000);
}
