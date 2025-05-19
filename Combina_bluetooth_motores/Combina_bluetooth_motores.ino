#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // RX, TX (Bluetooth)

#define ENA 3  // Habilitación Motor A
#define IN1 2 // Motor A dirección 1
#define IN2 4  // Motor A dirección 2
#define IN3 7  // Motor B dirección 1
#define IN4 8 // Motor B dirección 2
#define ENB 9  // Habilitación Motor B

int velocidad = 80;
int velocidadB = velocidad*1.20; //compensa el deficit en reversa

void setup() {
  Serial.begin(9600); // Monitor Serial
  BT.begin(9600);     // Bluetooth a 9600 baudios

  // Configurar pines de motor como salida
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  detener(); // Iniciar detenido
}

// Funciones de movimiento
void atras() {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidadB);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
}

void adelante() {
  analogWrite(ENA, velocidad);  
  analogWrite(ENB, velocidad);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void izquierda() {
  analogWrite(ENA, velocidad);  //antes: analogWrite(ENA, velocidad/1.5);
  analogWrite(ENB, velocidad);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void derecha() {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad); //antes:  analogWrite(ENB, velocidad/1.5);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void detener() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  if (BT.available()) {
    char comando = BT.read();
    Serial.println(comando); // Para ver qué llega
    
    if (comando == 'U' && velocidad < 150) {
      velocidad= velocidad + 10;
      analogWrite(ENA, velocidad);
      analogWrite(ENB, velocidad);
    }
    
    if (comando == 'D' ) {
      velocidad= velocidad - 10;
      analogWrite(ENA, velocidad);
      analogWrite(ENB, velocidad);
    }


    // Control según el comando recibido
    if (comando == 'F') { // Adelante
      adelante();
    } else if (comando == 'B') { // Atrás
      atras();
    } else if (comando == 'L') { // Izquierda
      izquierda();
    } else if (comando == 'R') { // Derecha
      derecha();
    } else if (comando == 'S') { // Stop
      detener();
    }
  }
}
