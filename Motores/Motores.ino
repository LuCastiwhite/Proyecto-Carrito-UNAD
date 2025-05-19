// Pines de conexión
int ENA = 3, ENB = 9;
int IN1 = 2, IN2 = 4, IN3 = 7, IN4 = 8;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  // Mueve los motores hacia adelante por 3 segundos
  adelante();
  delay(3000);

  // Mueve los motores hacia atrás por 3 segundos
  atras();
  delay(3000);
  
  // Mueve los motores a la izquierda por 3 segundos
  izquierda();
  delay(3000);

  // Mueve los motores a la derecha por 3 segundos
  derecha();
  delay(3000);

  // Detiene los motores por 2 segundos antes de repetir el ciclo
  detener();
  delay(2000); // Pausa antes de reiniciar la secuencia
}

void adelante() {
  // Ambos motores hacia adelante
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 150); // Ajusta la velocidad
  analogWrite(ENB, 150); // Ajusta la velocidad
}

void atras() {
  // Ambos motores hacia atrás
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 150); // Ajusta la velocidad
  analogWrite(ENB, 150); // Ajusta la velocidad
}

void izquierda() {
  // Mueve los motores para girar hacia la izquierda
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 150); // Ajusta la velocidad
  analogWrite(ENB, 150); // Ajusta la velocidad
}

void derecha() {
  // Mueve los motores para girar hacia la derecha
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 150); // Ajusta la velocidad
  analogWrite(ENB, 150); // Ajusta la velocidad
}

void detener() {
  // Detiene ambos motores
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); // Apaga los motores
  analogWrite(ENB, 0); // Apaga los motores
}
