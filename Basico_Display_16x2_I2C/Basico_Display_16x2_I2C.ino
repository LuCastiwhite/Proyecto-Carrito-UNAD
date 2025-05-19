#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Cambia 0x27 a 0x3F si no ves nada en pantalla
LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección I2C, columnas, filas

void setup() {
  lcd.init();            // Inicializar el LCD
  lcd.backlight();       // Encender la luz de fondo

  lcd.setCursor(0, 0);   // Fila 0, columna 0
  lcd.print("Bienvenidos!");

  lcd.setCursor(0, 1);   // Fila 1, columna 0
  lcd.print("Proyecto Display");
}

void loop() {
  // No necesitas nada aquí aún
}
