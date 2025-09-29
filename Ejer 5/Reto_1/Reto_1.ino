#include <Wire.h>

void recibirEvento(int howMany) {
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}

void setup() {
  Wire.begin(8); // Dirección esclavo
  Wire.onReceive(recibirEvento);
  Serial.begin(115200);
}

void loop() {
  delay(100);
}