#include <EEPROM.h>

const int ledPin = 13;
int opcion;           // Valor de configuración (0 = lenta, 1 = media, 2 = rápida)
const int addr = 0;   // Dirección en EEPROM para guardar la opción

// Tiempos según la opción
int tiempos[] = {1000, 500, 200}; // Lento, medio, rápido

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Leer configuración almacenada
  EEPROM.get(addr, opcion);

  if (opcion < 0 || opcion > 2) {
    opcion = 0; // Si la EEPROM está vacía o corrupta, default = lenta
  }

  Serial.println("=== MENU DE CONFIGURACION ===");
  Serial.println("0: Lenta (1s)");
  Serial.println("1: Media (0.5s)");
  Serial.println("2: Rapida (0.2s)");
  Serial.println("-----------------------------");
  Serial.print("Configuracion actual: ");
  Serial.println(opcion);
  Serial.println("Escribe el numero de la opcion y presiona ENTER.");
}

void loop() {
  // Revisa si el usuario escribe algo en el Serial
  if (Serial.available() > 0) {
    int seleccion = Serial.parseInt(); // Lee número ingresado
    if (seleccion >= 0 && seleccion <= 2) {
      opcion = seleccion;
      EEPROM.put(addr, opcion);  // Guardar en EEPROM
      Serial.print("Nueva configuracion guardada: ");
      Serial.println(opcion);
    }
    // Limpia el buffer de entrada
    while (Serial.available() > 0) {
      Serial.read();
    }
  }

  // Ejecuta comportamiento según configuración
  digitalWrite(ledPin, HIGH);
  delay(tiempos[opcion]);
  digitalWrite(ledPin, LOW);
  delay(tiempos[opcion]);
}