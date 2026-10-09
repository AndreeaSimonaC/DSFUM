const int ledPin = 7;//pinul non-pwm
int brightness = 0;
int fadeAmount = 5;

void setup() { pinMode(ledPin, OUTPUT); }

void loop() {
  for (brightness = 0; brightness <= 255; brightness += fadeAmount) { softwarePWM(ledPin, brightness); }
  for (brightness = 255; brightness >= 0; brightness -= fadeAmount) { softwarePWM(ledPin, brightness); }
}
void softwarePWM(int pin, int valoare) {
  int timpAprins = map(valoare, 0, 255, 0, 100);
  int timpStins = 100 - timpAprins;
  digitalWrite(pin, HIGH);
  delay(timpAprins);
  digitalWrite(pin, LOW);
  delay(timpStins);
}