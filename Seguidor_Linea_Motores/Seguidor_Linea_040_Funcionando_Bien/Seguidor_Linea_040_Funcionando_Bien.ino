#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // RX, TX (Bluetooth)

#define ENA 3
#define IN1 2
#define IN2 4
#define IN3 7
#define IN4 8
#define ENB 9

#define SENSOR_IZQUIERDO 12
#define SENSOR_DERECHO 13

const float FACTOR_A = 1.17;
const int VELOCIDAD_BASE = 50;
const int IMPULSO_INICIAL = 70;
const int GIRO_EXTRA = 1;
const int IMPULSO_GIRO = 70;
const int TIEMPO_IMPULSO = 30;

bool arrancado = false;
bool enGiro = false; // Nueva bandera para saber si venía girando

void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(SENSOR_IZQUIERDO, INPUT);
  pinMode(SENSOR_DERECHO, INPUT);

  detener();
}

void mover(int velA, int velB, bool dirA, bool dirB) {
  analogWrite(ENA, velA * FACTOR_A);
  analogWrite(ENB, velB);
  digitalWrite(IN1, dirA);
  digitalWrite(IN2, !dirA);
  digitalWrite(IN3, dirB);
  digitalWrite(IN4, !dirB);
}

void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void impulsoArranque() {
  mover(IMPULSO_INICIAL, IMPULSO_INICIAL, 1, 1); // Avanzar recto
  delay(TIEMPO_IMPULSO);
  arrancado = true;
}

void impulsoGiro(bool izquierda) {
  if (izquierda) {
    mover(IMPULSO_GIRO, IMPULSO_GIRO + GIRO_EXTRA, 1, 0);
  } else {
    mover(IMPULSO_GIRO + GIRO_EXTRA, IMPULSO_GIRO, 0, 1);
  }
  delay(TIEMPO_IMPULSO);
}

void loop() {
  bool izq = digitalRead(SENSOR_IZQUIERDO); // LOW = sobre la línea
  bool der = digitalRead(SENSOR_DERECHO);   // LOW = sobre la línea

  if (izq == LOW && der == LOW) { // Avanzar recto
    if (!arrancado || enGiro) { 
      impulsoArranque(); // Impulso inicial o impulso después de giro
      enGiro = false;    // Ya no está girando
    }
    mover(VELOCIDAD_BASE, VELOCIDAD_BASE, 1, 1);
  }
  else if (izq == LOW && der == HIGH) { // Girar a la derecha
    impulsoGiro(false);
    mover(VELOCIDAD_BASE + GIRO_EXTRA, VELOCIDAD_BASE, 0, 1);
    enGiro = true; // Está girando
  }
  else if (izq == HIGH && der == LOW) { // Girar a la izquierda
    impulsoGiro(true);
    mover(VELOCIDAD_BASE, VELOCIDAD_BASE + GIRO_EXTRA, 1, 0);
    enGiro = true; // Está girando
  }
  else if (izq == HIGH && der == HIGH) {
    detener();
    arrancado = false;
    enGiro = false;
  }
}
