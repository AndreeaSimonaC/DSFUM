//Având un LED conectat la unul din pinii PWM (cei marcați cu ~),
//modificați exemplul "fade" din File->Examples->Basics pentru a face LED-ul de pe breadboard să facă fade in/out.

const int led = 9;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(led, brightness);
    delay(10);
  }

  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(led, brightness);
    delay(10);
  }
}
