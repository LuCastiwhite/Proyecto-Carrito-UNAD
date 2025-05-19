#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Configuración LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección I2C puede variar (normalmente 0x27 o 0x3F)

SoftwareSerial BT(10, 11); // RX, TX Bluetooth

// Pines de motores
#define ENA 3
#define IN1 2
#define IN2 4
#define IN3 7
#define IN4 8
#define ENB 9

// Pines de sensores
#define SENSOR_IZQUIERDO 12
#define SENSOR_DERECHO 13

// Variables de control
const float FACTOR_A = 1.17;
const int VELOCIDAD_BASE = 43;
const int IMPULSO_INICIAL = 70;
const int GIRO_EXTRA = 1;
const int IMPULSO_GIRO = 60;
const int TIEMPO_IMPULSO = 30;

int velocidadManual = 60;
int velocidadManualB = velocidadManual * 1.20; // Compensa reversa

bool arrancado = false;
bool enGiro = false;
bool modoManual = true; // FALSE = AUTOMÁTICO, TRUE = MANUAL

void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  // Motor
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Sensores
  pinMode(SENSOR_IZQUIERDO, INPUT);
  pinMode(SENSOR_DERECHO, INPUT);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  actualizarLCD();

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
  mover(IMPULSO_INICIAL, IMPULSO_INICIAL, 1, 1);
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

// Funciones de control manual
void adelante() {
  analogWrite(ENA, velocidadManual);
  analogWrite(ENB, velocidadManual);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void atras() {
  analogWrite(ENA, velocidadManual);
  analogWrite(ENB, velocidadManualB);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void izquierda() {
  analogWrite(ENA, velocidadManual);
  analogWrite(ENB, velocidadManual);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void derecha() {
  analogWrite(ENA, velocidadManual);
  analogWrite(ENB, velocidadManual);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void actualizarLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" MODO:");
  lcd.setCursor(0, 1);
  if (modoManual) {
    lcd.print("    MANUAL");
  } else {
    lcd.print("  AUTOMATICO");
  }
}

void loop() {
  if (BT.available()) {
    char comando = BT.read();
    Serial.println(comando);

    // Cambio de modo
    if (comando == 'M') { // Letra 'M' para cambiar modo
      modoManual = !modoManual; // Alternar modo
      detener(); // Detener antes de cambiar de modo
      actualizarLCD();
    }

    if (modoManual) {
      // Control manual por Bluetooth
      if (comando == 'F') {
        adelante();
      } else if (comando == 'B') {
        atras();
      } else if (comando == 'L') {
        izquierda();
      } else if (comando == 'R') {
        derecha();
      } else if (comando == 'S') {
        detener();
      } else if (comando == 'U' && velocidadManual < 150) { // Aumentar velocidad
        velocidadManual += 10;
        velocidadManualB = velocidadManual * 1.20;
      } else if (comando == 'D' && velocidadManual > 20) { // Disminuir velocidad
        velocidadManual -= 10;
        velocidadManualB = velocidadManual * 1.20;
      }
    }
  }

  if (!modoManual) {
    // Modo automático (Seguidor de línea)
    bool izq = digitalRead(SENSOR_IZQUIERDO); // LOW = sobre la línea
    bool der = digitalRead(SENSOR_DERECHO);   // LOW = sobre la línea

    if (izq == LOW && der == LOW) { // Avanzar recto
      if (!arrancado || enGiro) {
        impulsoArranque();
        enGiro = false;
      }
      mover(VELOCIDAD_BASE, VELOCIDAD_BASE, 1, 1);
    }
    else if (izq == LOW && der == HIGH) { // Girar a la derecha
      impulsoGiro(false);
      mover(VELOCIDAD_BASE + GIRO_EXTRA, VELOCIDAD_BASE, 0, 1);
      enGiro = true;
    }
    else if (izq == HIGH && der == LOW) { // Girar a la izquierda
      impulsoGiro(true);
      mover(VELOCIDAD_BASE, VELOCIDAD_BASE + GIRO_EXTRA, 1, 0);
      enGiro = true;
    }
    else if (izq == HIGH && der == HIGH) { // Parar si no detecta línea
      detener();
      arrancado = false;
      enGiro = false;
    }
  }
}
