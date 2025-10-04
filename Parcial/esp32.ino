// ================== LIBRERÍAS ==================
#include <WiFi.h>              // Manejo de red WiFi en ESP32
#include <WiFiClientSecure.h>  // Cliente seguro para TLS/SSL
#include <PubSubClient.h>      // Cliente MQTT
#include <Preferences.h>       // Almacenamiento en memoria NVS
#include <Wire.h>              // Comunicación I2C
#include <esp_timer.h>         // Timers de alta precisión en ESP32

// ================== CONFIGURACIÓN RED ==================
const char* ssid = "moto";          // SSID de la red WiFi
const char* password = "12345678seh"; // Contraseña de la red WiFi

// ================== CONFIGURACIÓN MQTT ==================
const char* mqtt_server = "aebfc567fc434938bf244e554c3cb715.s1.eu.hivemq.cloud"; // Broker MQTT en la nube
const int mqtt_port = 8883;   // Puerto TLS seguro
const char* mqtt_username = "prueba";   // Usuario de conexión
const char* mqtt_password = "Prueba1#"; // Contraseña de conexión

// Topics MQTT
const char* CONTROL_MOTOR_TOPIC = "control-motor";   // Recibe ON/OFF
const char* TEMPERATURA_TOPIC   = "temperatura";     // Envía temperatura leída

// ================== HARDWARE ==================
#define MOTOR_PIN 27         // Pin de control del motor
#define LED_PIN   26         // Pin del LED indicador
#define I2C_SLAVE_ADDR 0x08  // Dirección I2C del Arduino UNO esclavo

// ================== CERTIFICADO TLS ==================
// Certificado raíz para conexión segura con el broker
static const char* root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

// ================== OBJETOS ==================
Preferences preferences;       // Para guardar datos en memoria no volátil
WiFiClientSecure secureClient; // Cliente seguro TLS
PubSubClient client(secureClient); // Cliente MQTT sobre TLS

// Tareas FreeRTOS
TaskHandle_t taskMQTTHandle = NULL;
TaskHandle_t taskMotorHandle = NULL;

// Timer periódico
esp_timer_handle_t periodic_timer;

// ================== VARIABLES ==================
volatile float temperatura = 0.0;     // Última temperatura recibida del UNO
volatile bool motorEncendido = false; // Estado actual del motor
volatile bool lecturaPending = false; // Bandera de lectura pendiente

// ================== FUNCIONES ==================

// ---- Callback del timer (cada 30s) ----
void IRAM_ATTR timer_callback(void* arg) {
  lecturaPending = true;  // Señala que toca leer temperatura
  Serial.println("[Timer] Señal de lectura enviada.");
}

// ---- Lectura de temperatura desde UNO vía I2C ----
bool leerTemperaturaI2C(float &outTemp) {
  Wire.requestFrom(I2C_SLAVE_ADDR, (uint8_t)8);  // Solicita hasta 8 bytes
  char buffer[10];
  int i = 0;
  while (Wire.available() && i < sizeof(buffer)-1) {
    buffer[i++] = Wire.read(); // Lee byte a byte
  }
  buffer[i] = '\0'; // Termina el string

  if (i > 0) {
    outTemp = atof(buffer);  // Convierte a float
    Serial.print("[I2C] Temperatura recibida del UNO: ");
    Serial.println(outTemp);
    return true;
  }

  Serial.println("[I2C] Error: no se pudo leer temperatura del UNO.");
  return false;
}

// ---- Publicar temperatura en MQTT ----
void publicarTemperatura(float temp) {
  char buffer[32];
  dtostrf(temp, 4, 2, buffer);  // Convierte float a texto
  client.publish(TEMPERATURA_TOPIC, buffer, true); // QoS = retain
  Serial.print("[MQTT] Publicado en ");
  Serial.print(TEMPERATURA_TOPIC);
  Serial.print(" -> ");
  Serial.println(buffer);
}

// ---- Callback MQTT (recibe mensajes) ----
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[MQTT] Mensaje recibido en topic: ");
  Serial.println(topic);

  if (strcmp(topic, CONTROL_MOTOR_TOPIC) == 0) { // Si es control-motor
    if (payload[0] == '1') {
      // Enciende motor
      motorEncendido = true;
      digitalWrite(MOTOR_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH);
      preferences.putBool("motorState", true);
      Serial.println("[MQTT] Motor ENCENDIDO");
    } else if (payload[0] == '0') {
      // Apaga motor
      motorEncendido = false;
      digitalWrite(MOTOR_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      preferences.putBool("motorState", false);
      Serial.println("[MQTT] Motor APAGADO");
    }
  }
}

// ---- Tarea MQTT (hilo dedicado) ----
void taskMQTT(void* pvParameters) {
  (void) pvParameters;
  client.setCallback(callback);

  while (true) {
    if (!client.connected()) {
      Serial.println("[MQTT] Intentando conectar...");
      // Conectar al broker
      if (client.connect("ESP32TLSClient", mqtt_username, mqtt_password)) {
        Serial.println("[MQTT] Conectado (TLS)");
        client.subscribe(CONTROL_MOTOR_TOPIC); // Suscripción
        Serial.print("[MQTT] Suscrito a topic: ");
        Serial.println(CONTROL_MOTOR_TOPIC);
      } else {
        Serial.print("[MQTT] Fallo rc=");
        Serial.println(client.state());
        vTaskDelay(5000 / portTICK_PERIOD_MS); // Espera antes de reintentar
        continue;
      }
    }
    client.loop(); // Mantiene la conexión activa
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// ---- Tarea Motor/Control ----
void taskMotor(void* pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (lecturaPending) {  // Si el timer pidió lectura
      lecturaPending = false;
      float tempLocal = 0.0;
      if (leerTemperaturaI2C(tempLocal)) {
        temperatura = tempLocal;

        // Guardar en NVS
        preferences.putFloat("ultimaTemp", temperatura);

        // Mostrar en Serial
        Serial.print("[DATA] Temperatura recibida del esclavo: ");
        Serial.print(temperatura);
        Serial.println(" °C");

        // ---- Control automático ----
        if (temperatura > 30.0f && !motorEncendido) {
          motorEncendido = true;
          digitalWrite(MOTOR_PIN, HIGH);
          digitalWrite(LED_PIN, HIGH);
          preferences.putBool("motorState", true);
          Serial.println("[AUTO] Motor ENCENDIDO (T > 30°C)");
        } else if (temperatura < 25.0f && motorEncendido) {
          motorEncendido = false;
          digitalWrite(MOTOR_PIN, LOW);
          digitalWrite(LED_PIN, LOW);
          preferences.putBool("motorState", false);
          Serial.println("[AUTO] Motor APAGADO (T < 25°C)");
        }

        // Publicar en MQTT
        publicarTemperatura(temperatura);
      }
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n[ESP32] Iniciando sistema...");

  // Inicializar pines
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  // Iniciar I2C como maestro
  Wire.begin();
  Serial.println("[I2C] Iniciado como maestro.");

  // Conexión WiFi
  Serial.print("[WiFi] Conectando a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("[WiFi] Conectado, IP: ");
  Serial.println(WiFi.localIP());

  // Recuperar valores almacenados en NVS
  preferences.begin("sistema", false);
  temperatura = preferences.getFloat("ultimaTemp", 0.0f);
  motorEncendido = preferences.getBool("motorState", false);
  digitalWrite(MOTOR_PIN, motorEncendido ? HIGH : LOW);
  digitalWrite(LED_PIN, motorEncendido ? HIGH : LOW);
  Serial.print("[NVS] Última temperatura almacenada: ");
  Serial.println(temperatura);
  Serial.print("[NVS] Estado motor: ");
  Serial.println(motorEncendido ? "ENCENDIDO" : "APAGADO");

  // Configuración TLS para MQTT
  secureClient.setCACert(root_ca);
  client.setServer(mqtt_server, mqtt_port);
  Serial.println("[TLS] Certificado cargado y broker configurado.");

  // Crear tareas en FreeRTOS
  xTaskCreatePinnedToCore(taskMQTT, "MQTT", 4096, NULL, 1, &taskMQTTHandle, 1);
  xTaskCreatePinnedToCore(taskMotor, "Motor", 4096, NULL, 1, &taskMotorHandle, 1);
  Serial.println("[RTOS] Tareas MQTT y Motor creadas.");

  // Crear timer cada 30 segundos
  const esp_timer_create_args_t periodic_timer_args = {
    .callback = &timer_callback,
    .name = "periodic30s"
  };
  esp_timer_create(&periodic_timer_args, &periodic_timer);
  esp_timer_start_periodic(periodic_timer, 30000000ULL);
  Serial.println("[Timer] Configurado cada 30s.");

  Serial.println("[Setup] Completado.");
}

// ================== LOOP ==================
void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS); // El loop queda vacío, todo lo hacen las tareas
}