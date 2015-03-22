void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("160.2,3.704,102.4,5.012,24.6,48");
  delay(1000);
}
