# RETO 3
## Explicacion del code
 Para la solucion de este codigo:
 ### Explicación detallada del código
#include <Arduino_FreeRTOS.h>
#include <semphr.h>


👉 Estas librerías permiten usar FreeRTOS (sistema operativo en tiempo real) y semáforos en Arduino.

Arduino_FreeRTOS.h → habilita la multitarea.

semphr.h → proporciona funciones para crear y usar semáforos (xSemaphoreCreateBinary, xSemaphoreTake, xSemaphoreGive, etc.).

// Declaramos el semáforo binario
SemaphoreHandle_t xSerialSemaphore;


👉 Se declara una variable de tipo semáforo binario (manejado por FreeRTOS) que servirá para controlar el acceso al recurso Serial.
Un semáforo binario solo puede estar en dos estados:

Disponible (1) → el recurso puede usarse.

Tomado (0) → el recurso está en uso.

// Pines de los LEDs
const int ledTarea1 = 9;
const int ledTarea2 = 10;


👉 LEDs indicadores para cada tarea, de modo que puedas ver cuándo cada una tiene el control del semáforo.

⚙️ Configuración inicial
void setup() {
  Serial.begin(9600);

  pinMode(ledTarea1, OUTPUT);
  pinMode(ledTarea2, OUTPUT);


👉 Se inicializa el puerto Serial y los pines de salida para los LEDs.

  // Crear semáforo binario
  xSerialSemaphore = xSemaphoreCreateBinary();


👉 Se crea el semáforo binario.
Por defecto, al crearse, está en estado bloqueado (no disponible).

  // Lo ponemos en estado "disponible"
  xSemaphoreGive(xSerialSemaphore);


👉 Esta instrucción libera el semáforo, poniéndolo en estado disponible, listo para que alguna tarea lo tome (xSemaphoreTake).

  if (xSerialSemaphore != NULL) {
    // Crear dos tareas que competirán por el recurso Serial
    xTaskCreate(Tarea1, "Tarea1", 128, NULL, 1, NULL);
    xTaskCreate(Tarea2, "Tarea2", 128, NULL, 1, NULL);
  }


👉 Solo si el semáforo se creó correctamente, se lanzan las dos tareas:

Ambas tienen igual prioridad (1).

Cada una intentará escribir en el Serial.

FreeRTOS se encargará de asignarles tiempo de ejecución.

🌀 loop vacío
void loop() {
  // El loop queda vacío porque FreeRTOS gestiona las tareas
}


👉 En sistemas con FreeRTOS, el loop() ya no se usa, porque el sistema operativo maneja el flujo mediante tareas.

🧩 Definición de las tareas
🔹 Tarea 1
void Tarea1(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (xSemaphoreTake(xSerialSemaphore, (TickType_t) 10) == pdTRUE) {
      digitalWrite(ledTarea1, HIGH);  // Encender LED de Tarea 1
      Serial.println(" - Tarea 1 está escribiendo en el puerto Serial...");
      vTaskDelay(300 / portTICK_PERIOD_MS);  
      digitalWrite(ledTarea1, LOW);   // Apagar LED
      xSemaphoreGive(xSerialSemaphore);     
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

🧩 Explicación paso a paso:

for(;;) → bucle infinito (la tarea nunca termina).

xSemaphoreTake() → intenta tomar el semáforo durante 10 ticks (~10 ms):

Si el semáforo está disponible, devuelve pdTRUE → la tarea obtiene acceso al recurso.

Si no lo está, la tarea espera hasta que quede libre o se cumpla el tiempo.

Una vez dentro del bloque protegido:

Enciende el LED de la tarea (ledTarea1).

Escribe un mensaje por el puerto Serial.

Espera 300 ms (vTaskDelay(300 / portTICK_PERIOD_MS)), sin bloquear el microcontrolador.

Apaga el LED.

Libera el semáforo con xSemaphoreGive(), permitiendo que otra tarea lo use.

Luego espera 500 ms antes de volver a intentarlo.

🔹 Tarea 2
void Tarea2(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (xSemaphoreTake(xSerialSemaphore, (TickType_t) 10) == pdTRUE) {
      digitalWrite(ledTarea2, HIGH);  // Encender LED de Tarea 2
      Serial.println(" + Tarea 2 está escribiendo en el puerto Serial...");
      vTaskDelay(300 / portTICK_PERIOD_MS);  
      digitalWrite(ledTarea2, LOW);   // Apagar LED
      xSemaphoreGive(xSerialSemaphore);     
    }
    vTaskDelay(700 / portTICK_PERIOD_MS);
  }
}


🔍 Muy similar a la Tarea 1, pero:

Tiene un retraso mayor (700 ms), por lo que intenta acceder al semáforo con menos frecuencia.

Esto simula una carga de trabajo diferente o distinta prioridad de ejecución temporal.

⚙️ Cómo funciona el semáforo en este caso

Ambas tareas quieren escribir en Serial (un recurso compartido y no seguro si se usa desde múltiples tareas simultáneamente).

El semáforo binario garantiza que solo una tarea lo use a la vez.

Cuando Tarea1 toma el semáforo, Tarea2 no puede escribir hasta que lo libere.

📊 Ejemplo de salida en el monitor serial:

 - Tarea 1 está escribiendo en el puerto Serial...
 + Tarea 2 está escribiendo en el puerto Serial...
 - Tarea 1 está escribiendo en el puerto Serial...
 + Tarea 2 está escribiendo en el puerto Serial...


Y mientras tanto, los LEDs 9 y 10 parpadean alternadamente, mostrando qué tarea está activa.