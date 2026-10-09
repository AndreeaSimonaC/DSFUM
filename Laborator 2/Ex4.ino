const int led = 9;
int brightness = 0;
int step = 1;
unsigned long interval = 10;
unsigned long timpAnterior = 0;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial.println("Intervalul in ms (1-1000):");
}

void loop() {
  if (Serial.available() > 0) {
    long val = Serial.parseInt();
    if (val >= 1 && val <= 1000) {
      interval = val;
      Serial.print("Interval nou: ");
      Serial.print(interval);
      Serial.println("ms");
    }
    while (Serial.available() > 0) { Serial.read(); }
  }

  unsigned long timpCurent = millis();
  if (timpCurent - timpAnterior >= interval) {
    timpAnterior = timpCurent;
    brightness += step;
    if (brightness >= 255) { brightness = 255; step = -1;}
    if (brightness <= 0) { brightness = 0; step = 1; }
    analogWrite(led, brightness);
  }
}