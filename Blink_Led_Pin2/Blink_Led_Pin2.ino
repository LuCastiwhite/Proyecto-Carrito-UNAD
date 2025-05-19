// Configura el pin del LED
const int ledPin = 2;

void setup() {
  // Establece el pin 2 como salida
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Enciende el LED
  digitalWrite(ledPin, HIGH);
  delay(1000); // espera 1 segundo

  // Apaga el LED
  digitalWrite(ledPin, LOW);
  delay(1000); // espera 1 segundo
}
