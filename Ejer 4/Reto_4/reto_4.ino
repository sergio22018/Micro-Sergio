#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

// ----------------- CONFIGURACIÓN -----------------
#define DHTPIN 2       // Pin de datos del sensor DHT11
#define DHTTYPE DHT11  // Tipo de sensor
#define LED_PIN 9      // LED que se enciende con temperatura alta

DHT dht(DHTPIN, DHTTYPE);

// Cola para comunicar tareas
QueueHandle_t colaTemperatura;

// Umbral de temperatura (ejemplo: 28 °C)
const float UMBRAL_TEMP = 28.0;

// ----------------- SETUP -----------------
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);

  // Crear cola con capacidad para 5 elementos tipo float
  colaTemperatura = xQueueCreate(5, sizeof(float));

  if (colaTemperatura != NULL) {
    // Crear tareas
    xTaskCreate(TareaLectura, "Lectura", 128, NULL, 2, NULL);
    xTaskCreate(TareaProcesamiento, "Procesamiento", 128, NULL, 1, NULL);
    xTaskCreate(TareaMonitoreo, "Monitoreo", 128, NULL, 1, NULL);
  }
}

// ----------------- LOOP -----------------
void loop() {
  // No se usa en FreeRTOS
}

// ----------------- TAREAS -----------------

// Tarea de Lectura del sensor
void TareaLectura(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    float temperatura = dht.readTemperature();
    if (!isnan(temperatura)) {
      // Enviar temperatura a la cola
      xQueueSend(colaTemperatura, &temperatura, portMAX_DELAY);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);  // cada 2 segundos
  }
}

// Tarea de Procesamiento
void TareaProcesamiento(void *pvParameters) {
  (void) pvParameters;
  float valorRecibido;
  for (;;) {
    if (xQueueReceive(colaTemperatura, &valorRecibido, portMAX_DELAY)) {
      Serial.print("🌡️ Temperatura: ");
      Serial.print(valorRecibido);
      Serial.println(" °C");

      // Control de LED según umbral
      if (valorRecibido > UMBRAL_TEMP) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("⚠️ Temperatura ALTA -> LED encendido");
      } else {
        digitalWrite(LED_PIN, LOW);
        Serial.println("✅ Temperatura normal -> LED apagado");
      }
    }
  }
}

// Tarea de Monitoreo
void TareaMonitoreo(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    Serial.println("🟢 Sistema en funcionamiento...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);  // cada 5 segundos
  }
}
