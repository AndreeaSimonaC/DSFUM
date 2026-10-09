//Dată o valoare pe portul serial, să se emuleze tensiunea (volți) respectiv la unul dintre pinii Arduino care nu este de tip PWM 
//(emularea corectă va fi măsurată cu un aparat de măsură). 
//Nu aveți voie să folosiți comanda delay în program.
const byte pinIesire = 8;
const unsigned long perioada = 10000;
unsigned long start = 0;
unsigned long timpHigh = 0;
float askedT = 0.0;

void setup() {
  pinMode(pinIesire, OUTPUT);
  digitalWrite(pinIesire, LOW);
  Serial.begin(9600);
  Serial.println("Tensiunea intre 0-5 V:");
}

void loop() {
  if (Serial.available() > 0) {
    float valoare = Serial.parseFloat();
    if (valoare >= 0.0 && valoare <= 5.0) {
      askedT = valoare;
      timpHigh = (unsigned long) (askedT / 5.0 * perioada);

      Serial.print("Tensiune ceruta: ");
      Serial.print(askedT, 2);
      Serial.println(" V");
    }

    while (Serial.available() > 0) { Serial.read();}
  }
  unsigned long acum = micros();
  unsigned long scurs = acum - start;

  if (scurs >= perioada) {
    start = acum;
    scurs = 0;
  }

  if (scurs < timpHigh) { digitalWrite(pinIesire, HIGH);
  } else { digitalWrite(pinIesire, LOW); }
}