# RETO 4
### Colas de mensajes para comunicación entre tareas
Objetivo: Entender cómo las tareas se comunican de manera ordenada.

Descripción:

Crear una tarea de lectura de sensor (por ejemplo, temperatura con un DHT11).
Esta tarea envía los valores a través de una cola a otra tarea.
La tarea de procesamiento recibe los datos de la cola y:
Los muestra en Serial.
Enciende un LED si la temperatura pasa cierto umbral.
Implementar además una tarea de "monitoreo" que corra en paralelo y muestre un "sistema en funcionamiento".
## Explicación del codigo
Este codigo se realizo de la siguiente manera:
### Inclusión de librerías y configuración
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>


Arduino_FreeRTOS.h → Permite usar un sistema operativo en tiempo real (RTOS) en Arduino, que gestiona varias tareas “simultáneamente”.

queue.h → Librería de FreeRTOS para manejar colas de comunicación entre tareas.

DHT.h → Librería del sensor DHT11, usado para medir temperatura y humedad.

⚙️ 2. Definición de pines y objetos
#define DHTPIN 2       // Pin donde se conecta el DHT11
#define DHTTYPE DHT11  // Tipo de sensor DHT
#define LED_PIN 9      // LED que indica temperatura alta

DHT dht(DHTPIN, DHTTYPE);


Se crea un objeto dht que controla el sensor.
El LED_PIN (9) se usará como indicador de alerta si la temperatura supera un valor límite.

📦 3. Variables globales y configuración inicial
QueueHandle_t colaTemperatura;  // Cola para enviar datos entre tareas
const float UMBRAL_TEMP = 28.0; // Umbral de temperatura en °C


colaTemperatura: servirá para transferir los valores de temperatura desde la tarea de lectura hacia la tarea que los procesa.

UMBRAL_TEMP: valor límite para decidir si encender o apagar el LED.

🏁 4. Función setup()
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);

  colaTemperatura = xQueueCreate(5, sizeof(float));


Se inicializa la comunicación serial, el sensor y el pin del LED.

xQueueCreate(5, sizeof(float)) crea una cola con capacidad para 5 valores tipo float.

Creación de las tareas
  if (colaTemperatura != NULL) {
    xTaskCreate(TareaLectura, "Lectura", 128, NULL, 2, NULL);
    xTaskCreate(TareaProcesamiento, "Procesamiento", 128, NULL, 1, NULL);
    xTaskCreate(TareaMonitoreo, "Monitoreo", 128, NULL, 1, NULL);
  }
}


Cada tarea se crea con:

Nombre (solo descriptivo).

Tamaño de pila (128).

Prioridad (mayor número = mayor prioridad).
👉 Aquí la tarea de lectura tiene prioridad 2, ya que es la fuente principal de datos.

🚫 5. Loop vacío
void loop() {
  // No se usa en FreeRTOS
}


FreeRTOS reemplaza el loop() con su planificador de tareas (scheduler), por lo que esta función no hace nada.

🔄 6. Tareas del sistema
🔹 TareaLectura
void TareaLectura(void *pvParameters) {
  for (;;) {
    float temperatura = dht.readTemperature();
    if (!isnan(temperatura)) {
      xQueueSend(colaTemperatura, &temperatura, portMAX_DELAY);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}


📤 Función: lee la temperatura cada 2 segundos y la envía a la cola si la lectura es válida.
Usa vTaskDelay() para esperar sin bloquear a las demás tareas.

🔹 TareaProcesamiento
void TareaProcesamiento(void *pvParameters) {
  float valorRecibido;
  for (;;) {
    if (xQueueReceive(colaTemperatura, &valorRecibido, portMAX_DELAY)) {
      Serial.print("🌡️ Temperatura: ");
      Serial.print(valorRecibido);
      Serial.println(" °C");

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


📥 Función: recibe los valores de temperatura desde la cola y:

Los muestra en el monitor serial.

Enciende o apaga el LED dependiendo del umbral.

👉 Esta tarea reacciona a los datos, no los genera.

🔹 TareaMonitoreo
void TareaMonitoreo(void *pvParameters) {
  for (;;) {
    Serial.println("🟢 Sistema en funcionamiento...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}


🟢 Función: cada 5 segundos, imprime un mensaje indicando que el sistema sigue activo.
Sirve como “watchdog” o indicador de que el RTOS no se bloqueó.