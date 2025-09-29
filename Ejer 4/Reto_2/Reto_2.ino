#include <Arduino.h>

// Pines
const int ledBajo = 5;      // LED de baja prioridad
const int ledAlto = 6;      // LED de alta prioridad
const int boton = 7;        // Botón
const int pot = A0;         // Potenciómetro

// Declaración de tareas
void TareaBaja(void *pvParameters);
void TareaMedia(void *pvParameters);
void TareaAlta(void *pvParameters);

void setup() {
  pinMode(ledBajo, OUTPUT);
  pinMode(ledAlto, OUTPUT);
  pinMode(boton, INPUT_PULLUP);

  Serial.begin(9600);

  // Crear tareas
  xTaskCreate(
    TareaBaja,
    "LED_Bajo",
    128,
    NULL,
    1,         // Prioridad baja
    NULL
  );

  xTaskCreate(
    TareaMedia,
    "Lectura_Pot",
    128,
    NULL,
    2,         // Prioridad media
    NULL
  );

  xTaskCreate(
    TareaAlta,
    "Boton",
    128,
    NULL,
    3,         // Prioridad alta
    NULL
  );

  vTaskStartScheduler(); // Inicia el planificador
}

void loop() {
  // No se usa con FreeRTOS
}

// 🔹 Tarea de baja prioridad: parpadeo cada 1000 ms
void TareaBaja(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(ledBajo, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(ledBajo, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// 🔹 Tarea de prioridad media: leer potenciómetro
void TareaMedia(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    int valor = analogRead(pot);
    Serial.print("Potenciometro: ");
    Serial.println(valor);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// 🔹 Tarea de prioridad alta: responder al botón
void TareaAlta(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (digitalRead(boton) == LOW) {
      digitalWrite(ledAlto, HIGH);
    } else {
      digitalWrite(ledAlto, LOW);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); // Anti-rebote ligero
  }
}
