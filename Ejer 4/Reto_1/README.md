# RETO 1
## Explicacion del code
 Para la solucion de este codigo:
 ### Código explicado línea por línea
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>


Arduino.h: Librería base del entorno Arduino.

Arduino_FreeRTOS.h: Librería que activa el sistema operativo FreeRTOS en microcontroladores Arduino (por ejemplo, ATmega328P del UNO).

FreeRTOS permite crear tareas paralelas, timers, colas de mensajes, semáforos, etc., para un control más robusto y preciso del tiempo.

⚙️ Configuración de pines
const int led1 = 10;   // LED 1
const int led2 = 11;   // LED 2


Se declaran los pines donde estarán conectados los dos LEDs.

Cada LED se controlará desde una tarea distinta.

🧩 Prototipos de tareas
void Tarea1(void *pvParameters);
void Tarea2(void *pvParameters);


En FreeRTOS, cada tarea se implementa como una función con el formato:

void nombreTarea(void *pvParameters);


El parámetro pvParameters permite pasar datos entre tareas (en este caso no se usa).

🧰 Función setup()
void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);


Configura ambos pines como salida digital.

🧱 Creación de tareas FreeRTOS
🔸 Primera tarea:
  xTaskCreate(
    Tarea1,           // Función asociada a la tarea
    "LED_500ms",      // Nombre (solo informativo)
    128,              // Tamaño del stack (memoria reservada)
    NULL,             // Parámetro que recibe la tarea (no usado)
    1,                // Prioridad (1 = baja)
    NULL              // Handle de la tarea (opcional)
  );


xTaskCreate() crea una nueva tarea y la registra en el planificador FreeRTOS.
Argumentos principales:

Parámetro	Descripción
Tarea1	Función que se ejecutará continuamente
"LED_500ms"	Nombre descriptivo
128	Espacio de pila (stack) en palabras
NULL	No se pasan parámetros
1	Prioridad de ejecución
NULL	No se guarda el handle

🟢 Esta tarea encenderá y apagará el LED 1 cada 500 ms.

🔸 Segunda tarea:
  xTaskCreate(
    Tarea2,
    "LED_1000ms",
    128,
    NULL,
    1,
    NULL
  );


🟢 Igual que la anterior, pero para el LED 2, con un período de 1000 ms (1 s).

🚀 Iniciar el planificador
  vTaskStartScheduler();
}


Esta función activa el kernel de FreeRTOS.

A partir de este momento:

El sistema operativo asume el control del microcontrolador.

Llama a las tareas creadas según su prioridad y disponibilidad.

La función loop() ya no se ejecuta.

⚙️ Loop vacío
void loop() {
  // No se usa con FreeRTOS
}


En programas con FreeRTOS, loop() no se ejecuta más una vez que el planificador inicia.

🔁 Definición de tareas
🔹 Tarea 1: LED cada 500 ms
void Tarea1(void *pvParameters) {
  (void) pvParameters;   // Evita advertencia por parámetro no usado
  for (;;) {             // Bucle infinito
    digitalWrite(led1, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Espera 500 ms sin bloquear
    digitalWrite(led1, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


🧩 Explicación:

for(;;) es el bucle infinito típico en tareas FreeRTOS.

vTaskDelay(tiempo / portTICK_PERIOD_MS) suspende solo la tarea actual durante ese tiempo, sin detener las demás.

Esto permite que otras tareas sigan ejecutándose en paralelo.

⏱ Ejemplo:

LED1 encendido 0.5 s → apagado 0.5 s → repite.

🔹 Tarea 2: LED cada 1000 ms
void Tarea2(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(led2, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    digitalWrite(led2, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


🧩 Explicación:

Idéntica estructura que la Tarea1, pero el periodo de parpadeo es 1 s.

El LED2 parpadea más lento que el LED1.

🧮 Cómo funciona FreeRTOS en este ejemplo
Recurso	Descripción
Scheduler (planificador)	Controla el orden y el tiempo de ejecución de cada tarea.
Tarea 1 (LED_500ms)	Parpadea rápido. Prioridad baja.
Tarea 2 (LED_1000ms)	Parpadea lento. Prioridad baja.
vTaskDelay()	Suspende solo la tarea actual. Las demás siguen activas.
Loop vacío	No se usa; las tareas toman su lugar.

Ambas tareas se ejecutan con la misma prioridad, así que el planificador les da tiempo de CPU equitativo.