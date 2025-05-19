#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // RX, TX

int LED = 2; // LED conectado al pin digital 8

void setup() {
    pinMode(LED, OUTPUT); // Configura el pin del LED

    Serial.begin(9600);
    BT.begin(9600);
}

void loop() {
    if (BT.available()) {
        char command = BT.read();
        Serial.println(command);
        ledControl(command);
    }
}

void ledControl(char cmd) {
    switch (cmd) {
        case 'A': digitalWrite(LED, HIGH); break; // Enciende LED
        case 'B': digitalWrite(LED, LOW); break;  // Apaga LED
    }
}