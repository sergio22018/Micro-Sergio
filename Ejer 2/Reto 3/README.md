# RETO 3

## Explicacion del code
 Para la solucion de este codigo:
 ### Código explicado y comentado
const int Led = 13;  // LED en pin 13 (puede ser el LED integrado del Arduino)


🟢 Se define el pin 13 como salida para el LED.
Ese es el pin del LED integrado en la placa UNO, aunque también puede usarse un LED externo.

void setup() {
  pinMode(Led, OUTPUT);  // Configura el pin del LED como salida


⚙️ Prepara el pin 13 para poder encender/apagar el LED.

  noInterrupts();           // Desactivar interrupciones globales


🚫 Se desactivan todas las interrupciones momentáneamente para evitar interferencias mientras se configura el Timer1.

  TCCR1A = 0;               // Limpiar registros de control
  TCCR1B = 0;
  TCNT1  = 0;               // Reiniciar contador


🧹 Limpia los registros de control (TCCR1A y TCCR1B) y pone el contador (TCNT1) en cero.
Esto asegura que el Timer1 comienza desde un estado limpio y predecible.

  // Configuración de comparación
  OCR1A = 7812;             // Valor para 500 ms con prescaler 1024