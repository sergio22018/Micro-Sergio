# RETO 4
## Explicacion del code
 Para la solucion de este codigo:
 ### Código explicado y comentado
const int buzzerPin = 9;   // Pin de salida conectado al buzzer
volatile bool alarmaActiva = false;   // Estado actual de la alarma (false = apagada)
volatile bool toggleBuzzer = false;   // Variable para alternar el estado del buzzer


volatile: se usa porque estas variables cambian dentro de una interrupción (ISR).

alarmaActiva controla si la alarma está encendida o no.

toggleBuzzer alterna el encendido/apagado del pin del buzzer, creando el tono.

void setup() {
  pinMode(buzzerPin, OUTPUT);   // Configura el pin 9 como salida
  Serial.begin(9600);           // Inicializa la comunicación serial


Aquí se prepara el pin del buzzer para enviar señales y se abre el monitor serial para mostrar mensajes.

  // --- Interrupción externa en pin 2 (INT0) para activar alarma ---
  attachInterrupt(digitalPinToInterrupt(2), activarAlarma, RISING);


Se configura el pin 2 como entrada de interrupción INT0.

Cuando se detecta un flanco ascendente (RISING) —por ejemplo, al presionar un botón— se ejecuta la función activarAlarma().

  // --- Interrupción externa en pin 3 (INT1) para desactivar alarma ---
  attachInterrupt(digitalPinToInterrupt(3), desactivarAlarma, RISING);


Similar al anterior, pero en el pin 3 (INT1).

Llama a desactivarAlarma() al presionar el botón correspondiente.

  // --- Configurar Timer2 en modo CTC ---
  noInterrupts();      // Desactivar interrupciones globales mientras se configura
  TCCR2A = 0;          // Limpiar registro de control A
  TCCR2B = 0;          // Limpiar registro de control B
  TCNT2  = 0;          // Reiniciar contador del timer


Se “resetean” los registros del Timer2 antes de configurarlo desde cero.

  // OCR2A determina la frecuencia del buzzer
  // Ejemplo: 16 MHz / (prescaler 64 * (249+1)) = 1 kHz
  OCR2A = 249;              // Valor de comparación (ajusta la frecuencia)


OCR2A define cuántos ciclos de reloj deben pasar antes de que se dispare la interrupción.

Con el reloj de 16 MHz y un prescaler de 64, esto genera una señal de 1 kHz (tono audible).

  TCCR2A |= (1 << WGM21);   // Activa modo CTC (Clear Timer on Compare)
  TCCR2B |= (1 << CS22);    // Establece prescaler a 64
  TIMSK2 |= (1 << OCIE2A);  // Habilita interrupción cuando TCNT2 == OCR2A
  interrupts();              // Reactiva interrupciones globales
}


El modo CTC hace que el contador se reinicie automáticamente cuando llega al valor OCR2A.

El prescaler 64 reduce la velocidad del conteo.

Se habilita la interrupción que ejecutará una función automáticamente cada vez que se cumple la comparación (es decir, a la frecuencia del tono).

🌀 loop principal
void loop() {
  // El loop no hace nada, todo lo manejan las ISR
}


El programa principal no necesita hacer nada.

Todo el control se realiza dentro de las rutinas de interrupción.

🚨 Rutina para activar la alarma
void activarAlarma() {
  alarmaActiva = true;               // Cambia el estado de la alarma
  Serial.println("🚨 Alarma activada");
}


Esta ISR se ejecuta al presionar el botón del pin 2.

Simplemente activa la alarma.

✅ Rutina para desactivar la alarma
void desactivarAlarma() {
  alarmaActiva = false;              // Cambia el estado de la alarma
  digitalWrite(buzzerPin, LOW);      // Apaga el buzzer inmediatamente
  Serial.println("✅ Alarma desactivada");
}


Esta ISR se ejecuta al presionar el botón del pin 3.

Detiene el sonido y apaga la alarma.

🎵 Interrupción del Timer2: genera el tono
ISR(TIMER2_COMPA_vect) {
  if (alarmaActiva) {                // Solo si la alarma está activa
    toggleBuzzer = !toggleBuzzer;    // Cambia el estado del pin (HIGH ↔ LOW)
    digitalWrite(buzzerPin, toggleBuzzer);
  }
}


Esta ISR se ejecuta cada vez que el Timer2 alcanza el valor OCR2A.

Si la alarma está activada, el pin se alterna (ON/OFF) generando una onda cuadrada.

Esa onda cuadrada se traduce en un tono audible en el buzzer.