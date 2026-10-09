int leduri[] = { 8, 9, 10, 11, 12, 13 };
int brightness[6] = { 0, 0, 0, 0, 0, 0 };
int step[6] = { 1, 1, 1, 1, 1, 1 };
int speed[6] = { 5, 10, 15, 20, 25, 30 };
unsigned long antTime[6] = { 0, 0, 0, 0, 0, 0 };

void setup() {
  for (int i = 0; i < 6; i++) { pinMode(leduri[i], OUTPUT); }
}

void loop() {
  for (int i = 0; i < 6; i++) {
    if (millis() - antTime[i] >= speed[i]) {
      antTime[i] = millis();
      brightness[i] += step[i];
      if (brightness[i] >= 255) {
        brightness[i] = 255;
        step[i] = -1;
      }

      if (brightness[i] <= 0) {
        brightness[i] = 0;
        step[i] = 1;
      }

      analogWrite(leduri[i], brightness[i]);
    }
  }
}
