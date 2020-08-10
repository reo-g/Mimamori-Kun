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

void loop() {
  get_env_info();

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
  delay(2000);
}
