# RETO 2
Reto 2: Secuencia de LEDs
Usar 8 LEDs.
Al presionar el botón, los LEDs deben encenderse en secuencia.
Al soltarlo, los LEDs deben apagarse en orden inverso.
## Explicacion del code
 Para la solucion de este codigo:
 Explicación:

leds[]: Un arreglo para almacenar los pines de los LEDs, así es fácil recorrerlos con bucles.

INPUT_PULLUP: Igual que en el reto anterior, para no usar resistencias externas.

Estructura for:

Cuando el botón está presionado (LOW), recorre el arreglo de inicio a fin para encender los LEDs.

Cuando está suelto (HIGH), recorre de fin a inicio para apagarlos.
## codigo comentado
```cpp
// Pines
const int pinBoton = 2;     // Pin del pulsador
const int leds[] = {3, 4, 5, 6, 7, 8, 9, 10}; // Pines de los 8 LEDs
const int numLeds = 8;      // Cantidad de LEDs

// Variables
bool estadoBoton;

void setup() {
  pinMode(pinBoton, INPUT_PULLUP); // Botón con resistencia pull-up interna
  
  // Configurar todos los LEDs como salidas
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW); // Asegurar que empiecen apagados
  }
}

void loop() {
  estadoBoton = digitalRead(pinBoton);

  if (estadoBoton == LOW) {  
    // Botón presionado → Encender LEDs en secuencia
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(leds[i], HIGH);
      delay(100); // Retraso entre encendidos
    }
  } 
  else {  
    // Botón soltado → Apagar LEDs en orden inverso
    for (int i = numLeds - 1; i >= 0; i--) {
      digitalWrite(leds[i], LOW);
      delay(100); // Retraso entre apagados
    }
  }
}
```