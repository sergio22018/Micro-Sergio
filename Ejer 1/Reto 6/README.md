# RETO 6
Reto 6: Memoria de Secuencia (Juego de Simón)
Usar varios LEDs y botones.
Crear una secuencia de encendido de LEDs.
El usuario deberá repetir la secuencia con los botones.
Si la secuencia es correcta, se genera una nueva más larga.
Si es incorrecta, el juego reinicia.
## Explicacion del code
 Para la solucion de este codigo:
 ### Cómo funciona:
Secuencia inicial: Se comienza con 1 LED encendiéndose.

Mostrar secuencia: Los LEDs se iluminan en el orden guardado.

Jugador responde: El usuario debe presionar los botones correspondientes.

Correcto:

Se añade un paso nuevo aleatorio a la secuencia.

Incorrecto:

Muestra mensaje y reinicia el juego.

### Codigo comentado

// Configuración
const int numLeds = 4;  
const int leds[] = {3, 4, 5, 6};     // Pines de LEDs
const int botones[] = {7, 8, 9, 10}; // Pines de botones

// Variables
int secuencia[50];        // Almacena la secuencia (máx. 50 pasos)
int longitudSecuencia = 0;
bool jugando = false;

void setup() {
  Serial.begin(9600);

  // Configurar LEDs y botones
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(botones[i], INPUT_PULLUP);
  }

  randomSeed(analogRead(A0)); // Semilla aleatoria

  iniciarJuego();
}

void loop() {
  if (jugando) {
    mostrarSecuencia();
    if (!leerRespuestaUsuario()) {
      // Error → Reiniciar
      Serial.println("¡Incorrecto! Reiniciando...");
      iniciarJuego();
    } else {
      // Correcto → añadir un nuevo paso
      longitudSecuencia++;
      secuencia[longitudSecuencia - 1] = random(0, numLeds);
      delay(500);
    }
  }
}

void iniciarJuego() {
  longitudSecuencia = 1;
  secuencia[0] = random(0, numLeds);
  jugando = true;
}

void mostrarSecuencia() {
  for (int i = 0; i < longitudSecuencia; i++) {
    digitalWrite(leds[secuencia[i]], HIGH);
    delay(500);
    digitalWrite(leds[secuencia[i]], LOW);
    delay(250);
  }
}

bool leerRespuestaUsuario() {
  for (int i = 0; i < longitudSecuencia; i++) {
    int botonPresionado = esperarBoton();
    if (botonPresionado != secuencia[i]) {
      return false; // Fallo
    }
  }
  return true; // Todo correcto
}

int esperarBoton() {
  while (true) {
    for (int i = 0; i < numLeds; i++) {
      if (digitalRead(botones[i]) == LOW) {
        digitalWrite(leds[i], HIGH);
        delay(300); // Tiempo de confirmación
        digitalWrite(leds[i], LOW);
        while (digitalRead(botones[i]) == LOW); // Esperar que suelte
        delay(100);
        return i;
      }
    }
  }
}
