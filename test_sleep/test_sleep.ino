void setup() {
  Serial.begin(9600);
  Serial.println("Rebooting...");
}

void loop() {
  // put your main code here, to run repeatedly:
  ESP.deepSleep(10000000);
}
