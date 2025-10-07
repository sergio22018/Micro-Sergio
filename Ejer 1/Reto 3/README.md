# RETO 3
Reto 3: Luz de Emergencia
Usar dos LEDs de diferente color.
Un botón activará un parpadeo alterno entre los LEDs.
Otro botón detendrá el parpadeo.
## Explicacion del code
 Para la solucion de este codigo:
### Explicación:

Dos botones independientes:

Uno inicia el parpadeo (botonInicio).

Otro lo detiene (botonParo).

Variable emergenciaActiva: Guarda el estado de si estamos en modo emergencia.

Parpadeo alterno:

Un LED se enciende mientras el otro se apaga.

Se alternan cada 200 ms.
### Codigo comentado
// Pines
const int botonInicio = 2;   // Botón para iniciar parpadeo
const int botonParo = 3;     // Botón para detener parpadeo
const int ledRojo = 8;       // LED rojo
const int ledAzul = 9;       // LED azul

// Variables
bool emergenciaActiva = false; // Estado del parpadeo

void setup() {
  pinMode(botonInicio, INPUT_PULLUP);
  pinMode(botonParo, INPUT_PULLUP);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  // Aseguramos LEDs apagados
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledAzul, LOW);
}

void loop() {
  // Leer botones
  if (digitalRead(botonInicio) == LOW) {
    emergenciaActiva = true;  // Activar parpadeo
  }
  if (digitalRead(botonParo) == LOW) {
    emergenciaActiva = false; // Detener parpadeo
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledAzul, LOW);
  }

  // Ejecutar parpadeo si está activo
  if (emergenciaActiva) {
    digitalWrite(ledRojo, HIGH);
    digitalWrite(ledAzul, LOW);
    delay(200);

    digitalWrite(ledRojo, LOW);
    digitalWrite(ledAzul, HIGH);
    delay(200);
  }
}
