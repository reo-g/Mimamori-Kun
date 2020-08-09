#include <PanasonicHeatpumpIR.h>

#define cool_button 12
#define stop_button 13

void setup(){
  // setting button pin mode  
    pinMode(cool_button,INPUT_PULLUP);
    pinMode(stop_button,INPUT_PULLUP);

    // Serial begin speed = 115200
    Serial.begin(115200);
}

void loop(){

IRSenderPWM irSender(3); // irSender(IRLEDpinNum);
PanasonicDKEHeatpumpIR *heatpumpIR;

  // read button state 
    unsigned int air_cool_on_status = digitalRead(cool_button);
    unsigned int air_off_status = digitalRead(stop_button);

    // 

    if(air_off_status == LOW){ // air conditioner off
        Serial.println("air off");
        heatpumpIR = new PanasonicDKEHeatpumpIR();
        heatpumpIR->send(irSender, POWER_OFF, MODE_COOL, FAN_AUTO, 26, VDIR_AUTO, HDIR_AUTO);
                  //send(irSender,スイッチON,OFF,冷暖房とか除湿,風量,温度,風向（上下）,風向（右左）);
      }else if(air_cool_on_status == LOW){ // air conditioner mode cool
        Serial.println("cool on");
        heatpumpIR = new PanasonicDKEHeatpumpIR();
        heatpumpIR->send(irSender, POWER_ON, MODE_COOL, FAN_AUTO, 26, VDIR_AUTO, HDIR_AUTO);
      }

      //delay time between this loop and next loop
    delay(500);
}
