#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Declaramos el semáforo binario
SemaphoreHandle_t xSerialSemaphore;

// Pines de los LEDs
const int ledTarea1 = 9;
const int ledTarea2 = 10;

void setup() {
  Serial.begin(9600);

  pinMode(ledTarea1, OUTPUT);
  pinMode(ledTarea2, OUTPUT);

  // Crear semáforo binario
  xSerialSemaphore = xSemaphoreCreateBinary();

  // Lo ponemos en estado "disponible"
  xSemaphoreGive(xSerialSemaphore);

  if (xSerialSemaphore != NULL) {
    // Crear dos tareas que competirán por el recurso Serial
    xTaskCreate(Tarea1, "Tarea1", 128, NULL, 1, NULL);
    xTaskCreate(Tarea2, "Tarea2", 128, NULL, 1, NULL);
  }
}

void loop() {
  // El loop queda vacío porque FreeRTOS gestiona las tareas
}

// ---------------- TAREAS ----------------

// Tarea 1
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

// Tarea 2
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
