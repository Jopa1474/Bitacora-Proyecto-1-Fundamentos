// Pines
const int sensorPin = 2;   // HW-513
const int dataPin   = 3;   // Pin DATA del 74HC164
const int clockPin  = 4;   // Pin CLOCK del 74HC164
const int ledPin    = 13;  // LED de depuración opcional

// Variables
int sensorState = 0;       // Lectura actual del sensor
int dataLatch   = 0;       // Dato almacenado hasta el pulso de reloj
unsigned long lastClock = 0;
const unsigned long clockPeriod = 1500; // 1 Hz (1 segundo)

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
  digitalWrite(ledPin, LOW);
}

void loop() {
  // 1. Detectar golpe -> almacenar 1
  sensorState = digitalRead(sensorPin);
  if (sensorState == LOW) {
    dataLatch = 1; // se mantiene hasta el siguiente flanco
    digitalWrite(ledPin, HIGH); // debug
    digitalWrite(dataPin, dataLatch);
  }

  // 2. Generar reloj cada segundo
  unsigned long now = millis();
  if (now - lastClock >= clockPeriod) {
    lastClock = now;

    // --- Flanco de subida ---
    digitalWrite(dataPin, dataLatch);   // Poner el dato en la entrada
    digitalWrite(clockPin, HIGH);       // Flanco de subida
    delayMicroseconds(10);              // Pequeña pausa
    digitalWrite(clockPin, LOW);        // Fin del pulso

    // Para debug visual
    if (dataLatch == 1) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }

    // 3. Resetear latch después de capturar
    dataLatch = 0;
  }
}
