#include <MultiFuncShield.h>

const int BUZZER = 3;

unsigned long lastSecond = 0;

long clockSeconds = 0;
long stopwatchSeconds = 0;

bool clockSet = false;
bool stopwatchSet = false;

bool clockAlarmSet = false;
long clockAlarmSeconds = 0;
int clockMelody = 0;
int stopwatchMelody = 0;

bool stopwatchFinished = false;

enum Mode { CLOCK,
            STOPWATCH };
Mode mode = CLOCK;

const int melodies[2][4] = { { 262, 330, 392, 523 }, { 392, 349, 330, 262 } };

int playingMelody = -1;
int noteIndex = 0;
unsigned long nextNote = 0;

char input[40];
byte inputPos = 0;

bool parseTime(const char* s, int& a, int& b) {
  if (strlen(s) != 5 || s[2] != ':') return false;
  if (!isDigit(s[0]) || !isDigit(s[1]) || !isDigit(s[3]) || !isDigit(s[4])) return false;
  a = (s[0] - '0') * 10 + s[1] - '0';
  b = (s[3] - '0') * 10 + s[4] - '0';
  return true;
}

void startMelody(int index) {
  if (index < 0 || index > 1) return;
  if (playingMelody != -1) return;
  playingMelody = index;
  noteIndex = 0;
  nextNote = millis();
}

void updateMelody() {
  if (playingMelody == -1) return;
  if ((long)(millis() - nextNote) < 0) return;
  if (noteIndex == 4) {
    noTone(BUZZER);
    playingMelody = -1;
    return;
  }

  tone(BUZZER, melodies[playingMelody][noteIndex], 180);
  noteIndex++;
  nextNote = millis() + 250;
}

void updateTime() {
  unsigned long now = millis();

  while (now - lastSecond >= 1000) {
    lastSecond += 1000;
    if (clockSet) {
      clockSeconds = (clockSeconds + 1) % 86400;
      if (clockAlarmSet && clockSeconds == clockAlarmSeconds) { startMelody(clockMelody); }
    }

    if (stopwatchSet && stopwatchSeconds > 0) {
      stopwatchSeconds--;
      if (stopwatchSeconds == 0 && !stopwatchFinished) {
        stopwatchFinished = true;
        startMelody(stopwatchMelody);
      }
    }
  }
}

void processCommand(char* command) {
  int a, b, melody;

  if (strcmp(command, "clock") == 0) {
    mode = CLOCK;
    Serial.println("Mod CLOCK");
  } else if (strcmp(command, "stopwatch") == 0) {
    mode = STOPWATCH;
    Serial.println("Mod STOPWATCH");
  } else if (strncmp(command, "clock ", 6) == 0) {
    if (clockSet) {
      Serial.println("Clock deja setat!");
      return;
    }

    if (!parseTime(command + 6, a, b) || a > 23 || b > 59) {
      Serial.println("Ora invalida!");
      return;
    }

    clockSeconds = a * 3600L + b * 60L;
    clockSet = true;
    mode = CLOCK;
    Serial.println("Clock setat!");
  } else if (strncmp(command, "stopwatch ", 10) == 0) {
    if (stopwatchSet) {
      Serial.println("Stopwatch deja setat!");
      return;
    }

    if (!parseTime(command + 10, a, b) || b > 59) {
      Serial.println("Timp invalid!");
      return;
    }

    stopwatchSeconds = a * 60L + b;
    stopwatchSet = true;
    mode = STOPWATCH;

    if (stopwatchSeconds == 0) {
      stopwatchFinished = true;
      startMelody(stopwatchMelody);
    }
    Serial.println("Stopwatch setat!");
  } else if (sscanf(command, "alarm clock %d:%d %d", &a, &b, &melody)  == 3) {
    if (a > 23 || a < 0 || b > 59 || b < 0 || melody < 0 || melody > 1) {
      Serial.println("Alarma invalida!");
      return;
    }

    clockAlarmSeconds = a * 3600L + b * 60L;
    clockMelody = melody;
    clockAlarmSet = true;
    Serial.println("Alarma clock setata!");
  } else if (sscanf(command, "alarm stopwatch %d", &melody) == 1 && strncmp(command, "alarm stopwatch ", 16) == 0) {
    if (melody < 0 || melody > 1) {
      Serial.println("Melodie invalida!");
      return;
    }

    stopwatchMelody = melody;
    Serial.println("Alarma stopwatch setata!");
  } else {
    Serial.println("Comanda necunoscuta!");
  }
}

void readSerial() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (inputPos > 0) {
        input[inputPos] = '\0';
        processCommand(input);
        inputPos = 0;
      }
    } else if (inputPos < sizeof(input) - 1) {
      input[inputPos++] = c;
    } else {
      inputPos = 0;
      Serial.println("Comanda prea lunga!");
    }
  }
}

void updateDisplay() {
  static unsigned long lastDisplay = 0;

  if (millis() - lastDisplay < 100) return;
  lastDisplay = millis();

  int first, second;

  if (mode == CLOCK) {
    first = clockSeconds / 3600;
    second = (clockSeconds % 3600) / 60;
  } else {
    first = stopwatchSeconds / 60;
    second = stopwatchSeconds % 60;
  }

  char displayText[5];
  snprintf(displayText, sizeof(displayText), "%02d%02d", first, second);
  MFS.write(displayText);
}

void setup() {
  Serial.begin(9600);
  MFS.initialize();

  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);

  lastSecond = millis();

  Serial.println("Sistem pregatit!");
}

void loop() {
  readSerial();
  updateTime();
  updateMelody();
  updateDisplay();

  MFS.manualDisplayRefresh();
}