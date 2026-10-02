void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}
int x, i;

void loop() {
  if (Serial.available() > 0) {
    i = Serial.parseInt();
    if (i != 0)
      x = i;
  }
  if (x == 1) {
    Serial.println("Pornit");
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (x == 2) {
    Serial.println("Oprit");
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (x == 3) {

    Serial.println("Blink");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
}