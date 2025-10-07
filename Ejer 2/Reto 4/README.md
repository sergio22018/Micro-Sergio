# RETO 4
## Explicacion del code
 Para la solucion de este codigo:
 ### Código explicado y comentado
volatile unsigned int pulsos = 0;  // Contador de pulsos
volatile unsigned int frecuencia = 0;


🧠 volatile indica que estas variables pueden cambiar dentro de una interrupción,
y evita que el compilador optimice su valor erróneamente.

pulsos: cuenta los flancos detectados en el pin 2.

frecuencia: almacena el resultado final (pulsos por segundo).

void setup() {
  Serial.begin(9600);


📟 Inicializa la comunicación serial para mostrar los resultados en el monitor.

🧩 Configuración de interrupción externa
  attachInterrupt(digitalPinToInterrupt(2), contarPulsos, RISING);


🔹 Configura una interrupción externa en el pin 2 (INT0).
Cada vez que la señal pasa de LOW → HIGH (RISING), se ejecuta la función contarPulsos().

📍 Otras opciones posibles:

FALLING: flanco descendente (HIGH → LOW)

CHANGE: cualquier cambio de estado

LOW: cuando el pin está en nivel bajo

⚙️ Configuración del Timer1 para generar una interrupción cada 1 segundo
  noInterrupts();           // Apagar interrupciones globales


🔒 Se desactivan las interrupciones mientras se configura el temporizador.

  TCCR1A = 0;               // Reset registros
  TCCR1B = 0;
  TCNT1  = 0;


🧹 Limpia los registros del Timer1 (TCCR1A, TCCR1B) y reinicia el contador (TCNT1 = 0).

  // Valor de comparación para 1s
  // 16MHz / 1024 = 15625 ticks por segundo
  OCR1A = 15624;            