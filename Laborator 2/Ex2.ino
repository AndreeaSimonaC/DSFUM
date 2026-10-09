//Faceți un program care să afișeze în binar (pe biți) pe cele 6 leduri conectate la Arduino în primul exercițiu o valoare recepționată pe portul serial.
const int leduri[6] = {8, 9, 10, 11, 12, 13};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 6; i++) { pinMode(leduri[i], OUTPUT); }
  Serial.println("Numar intre 0 si 63:");
}

void loop() {
  if (Serial.available() > 0) {
    int nr = Serial.parseInt();
    if (nr >= 0 && nr <= 63) {
      for (int bit = 0; bit < 6; bit++) {
        digitalWrite(leduri[bit], (nr >> bit) & 1);
      }

      Serial.print("Valoarea: ");
      Serial.println(nr);
    } else {
      Serial.println("Not good. NUmarul trebuie sa fie intre 0-63.");
    }

    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}
