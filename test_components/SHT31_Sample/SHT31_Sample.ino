//---------------------------------------------------//
//  AKIZUKI DENSHI TSUSHO CO.,LTD.
//  Version 1.0  2017-06-20
//---------------------------------------------------//


#include <Arduino.h>
#include <Wire.h>
#include "AE_SHT31.h"


//---------------------------------------------------//
// AE-SHT31    ------     Arduino UNO                //
//                                                   //
//   V+        ------        5.0V                    //
//  SDA        ------        A4                      //
//  SCL        ------        A5                      //
//  ADR        ------       (Open = 0x45)            //
//  GND        ------        GND                     //
//---------------------------------------------------//

// SHT31のアドレスを設定
AE_SHT31 SHT31 = AE_SHT31(0x45);


void setup() {
  // シリアル通信を9600bpsに設定
  Serial.begin(9600);
  // シリアルに文字を出力
  Serial.println("SHT31 Test!!");
  // SHT31をソフトリセット
  SHT31.SoftReset();
  // 内蔵ヒーター 0:OFF 1:ON
  SHT31.Heater(0);
}



void loop()
{
  // SHT31から温湿度データを取得
  SHT31.GetTempHum();
  
  Serial.println("--------------------------");
  // SHT31.Temperature()　より温度データ取得
  Serial.println("Temperature ('C)");
  Serial.println(SHT31.Temperature());
  // SHT31.Humidity()　より相対湿度データ取得
  Serial.println("Humidity (%)");
  Serial.println(SHT31.Humidity());
  // 待ち時間
  delay(800);
}
