# RETO 4
Reto 4: Contador Binario con LEDs
Usar cuatro LEDs para representar un número binario.
Un botón incrementará el número en binario (0 a 15).
Al llegar a 15, volverá a 0.
## Explicacion del code
 Para la solucion de este codigo:
 ### Explicación:

contador: Guarda el valor actual en decimal (0 a 15).

bitRead(): Extrae cada bit para encender/apagar el LED correspondiente.

Flanco de bajada:

Evita incrementos múltiples mientras se mantiene el botón presionado.

Anti-rebote básico con delay(50) para evitar lecturas falsas.

### Codigo comentado

// Pines
const int boton = 2;          // Botón para incrementar
const int leds[] = {8, 9, 10, 11}; // LEDs para bits (LSB a MSB)
const int numLeds = 4;

// Variables
int contador = 0;              // Valor actual (0 a 15)
bool estadoBotonAnterior = HIGH;

void setup() {
  pinMode(boton, INPUT_PULLUP);

  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  bool estadoBoton = digitalRead(boton);

  // Detección de flanco de bajada
  if (estadoBoton == LOW && estadoBotonAnterior == HIGH) {
    contador++;                 // Incrementar
    if (contador > 15) contador = 0; // Reiniciar si supera 15
    mostrarBinario(contador);   // Actualizar LEDs
    delay(50);                  // Anti-rebote básico
  }

  estadoBotonAnterior = estadoBoton;
}

// Función para mostrar un número en binario con LEDs
void mostrarBinario(int valor) {
  for (int i = 0; i < numLeds; i++) {
    bool bit = bitRead(valor, i); // Leer bit i del valor
    digitalWrite(leds[i], bit);
  }
}
