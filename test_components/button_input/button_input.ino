#define PIN_OUT 7

void setup() {
  pinMode(PIN_OUT, OUTPUT);
}

void loop() {
  //シングルクリック <1>
  digitalWrite(PIN_OUT, HIGH);
  delay(500);
  digitalWrite(PIN_OUT, LOW);
  
  delay(30000);
  
  
  
  //ダブルクリック <2>
  digitalWrite(PIN_OUT, HIGH);
  delay(500);
  digitalWrite(PIN_OUT, LOW);
  delay(500);
  digitalWrite(PIN_OUT, HIGH);
  delay(500);
  digitalWrite(PIN_OUT, LOW);
  
  delay(30000);

  //長押し <3>
  digitalWrite(PIN_OUT, HIGH);
  delay(1500);
  digitalWrite(PIN_OUT, LOW);

  delay(30000);
}
