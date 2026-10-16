/*
Pe fiecare dintre pinii 8-13 am conectat cate un led 
si trebuie ca sa il pornesc intr-un anumit fel, folosind o anumita comanda. 
Comanda este in formatul "nr_led,animatie,timp_in_ms". 
Animatiile pot fi oprite cu o animatie de tip "off", 
restul animatiilor sunt fadein, fadeout,fadeinout,blink. 
Ex.:
8,fadeinout,2000 va avea ca efect realizarea unui fadein pentru 1000ms si un fadeout pentru 1000ms dupa care se va relua.
*/

const int ledPins[] = { 8, 9, 10, 11, 12, 13 };
const int NUM_LEDS = 6;
enum Animations { OFF, FADEIN, FADEOUT, FADEINOUT, BLINK };

struct Led {
  Animations animation = OFF;
  unsigned long duration = 1000;
  unsigned long startTime = 0;
};

Led leds[NUM_LEDS];
String input = "";

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  Serial.println("Write the command:");
}

void readCommand() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (input.length() > 0) {
        processCommand(input);
        input = "";
      }
    } else {
      input += c;
    }
  }
}


void processCommand(String command) {
  int firstComma = command.indexOf(',');
  int secondComma = command.indexOf(',', firstComma + 1);

  if (firstComma < 0 || secondComma < 0) {
    Serial.println("Format invalid!");
    return;
  }

  int ledNumber = command.substring(0, firstComma).toInt();
  String anim = command.substring(firstComma + 1, secondComma);
  anim.trim();
  anim.toLowerCase();

  unsigned long duration = command.substring(secondComma + 1).toInt();

  if (ledNumber < 8 || ledNumber > 13 || duration == 0) {
    Serial.println("LED sau timp invalid!");
    return;
  }

  int index = ledNumber - 8;

  if (anim == "off") {
    leds[index].animation = OFF;
  } else if (anim == "fadein") {
    leds[index].animation = FADEIN;
  } else if (anim == "fadeout") {
    leds[index].animation = FADEOUT;
  } else if (anim == "fadeinout") {
    leds[index].animation = FADEINOUT;
  } else if (anim == "blink") {
    leds[index].animation = BLINK;
  } else {
    Serial.println("Animatie necunoscuta!");
    return;
  }

  leds[index].duration = duration;
  leds[index].startTime = millis();

  Serial.println("Comanda acceptata!");
}

void updateLed(int index, unsigned long now) {
  Led &led = leds[index];
  unsigned long elapsed = now - led.startTime;
  unsigned long duration = led.duration;

  switch (led.animation) {
    //cases: on, fadeint, fadeout, fadeinout, blink
    case OFF:
      digitalWrite(ledPins[index], LOW);
      break;
    case FADEIN:
      {
        int brightness = (elapsed % duration) * 255UL / duration;
        setBrightness(index, brightness);
        break;
      }
    case FADEOUT:
      {
        int brightness = 255 - (elapsed % duration) * 255UL / duration;
        setBrightness(index, brightness);
        break;
      }
    case FADEINOUT:
      {
        unsigned long phase = elapsed % duration;
        unsigned long half = duration / 2;

        int brightness;

        if (phase < half) {
          brightness = phase * 255UL / half;
        } else {
          brightness = (duration - phase) * 255UL / (duration - half);
        }

        setBrightness(index, brightness);
        break;
      }
    case BLINK:
      {
        bool state = (elapsed / (duration / 2 + 1)) % 2 == 0;
        digitalWrite(ledPins[index], state ? HIGH : LOW);
        break;
      }
  }
}

void setBrightness(int index, int brightness) {
  static unsigned long lastUpdate[NUM_LEDS] = { 0 };
  static int pwmCounter[NUM_LEDS] = { 0 };

  unsigned long now = micros();

  if (now - lastUpdate[index] >= 100) {
    lastUpdate[index] = now;
    pwmCounter[index] = (pwmCounter[index] + 1) % 256;
  }

  digitalWrite(ledPins[index], pwmCounter[index] < brightness ? HIGH : LOW);
}

void loop() {
  readCommand();
  unsigned long now = millis();
  for (int i = 0; i < NUM_LEDS; i++) {
    updateLed(i, now);
  }
}