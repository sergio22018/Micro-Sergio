#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

// Pines
const int led1 = 10;   // LED 1
const int led2 = 11;   // LED 2

// Declaración de tareas
void Tarea1(void *pvParameters);
void Tarea2(void *pvParameters);

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Crear tareas
  xTaskCreate(
    Tarea1,           // Función de la tarea
    "LED_500ms",      // Nombre de la tarea
    128,              // Stack
    NULL,             // Parámetro
    1,                // Prioridad
    NULL              // Handle
  );

  xTaskCreate(
    Tarea2,           // Función de la tarea
    "LED_1000ms",     // Nombre
    128,              // Stack
    NULL,             // Parámetro
    1,                // Prioridad
    NULL              // Handle
  );

  // Iniciar el planificador
  vTaskStartScheduler();
}

void loop() {
  // No se usa con FreeRTOS
}

void Tarea1(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(led1, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Espera 500 ms sin bloquear
    digitalWrite(led1, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void Tarea2(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(led2, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Espera 1000 ms sin bloquear
    digitalWrite(led2, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
