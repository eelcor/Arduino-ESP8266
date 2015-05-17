void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Rebooted");
}

void loop() {
  // put your main code here, to run repeatedly:
  ESP.deepSleep(10000000);
}
