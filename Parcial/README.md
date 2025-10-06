# 🌡️ Proyecto ESP32 + Arduino UNO + MQTT (TLS)

## 📘 Descripción general

Este proyecto implementa un **sistema de monitoreo y control de temperatura** basado en:

- **Arduino UNO** → mide la temperatura con el sensor **DHT11** y la envía por **I2C**.  
- **ESP32** → recibe la lectura, publica los datos en un **broker MQTT con TLS**, y controla un **motor** de forma **remota o automática**.  
- Comunicación **segura (TLS/SSL)** mediante **HiveMQ Cloud**.  
- Control automático del motor según temperatura:  
  🔸 *ON* si T > 30 °C  
  🔸 *OFF* si T < 25 °C  

---

## 🔹 Librerías incluidas

```cpp
#include <WiFi.h>              // Manejo de WiFi en ESP32
#include <WiFiClientSecure.h>  // Cliente seguro (TLS/SSL)
#include <PubSubClient.h>      // Cliente MQTT
#include <Preferences.h>       // Almacenamiento NVS (memoria no volátil)
#include <Wire.h>              // Comunicación I2C
#include <esp_timer.h>         // Timers de alta precisión (FreeRTOS)
Estas librerías permiten que el ESP32:

✅ Se conecte a WiFi de forma segura
✅ Publique y reciba datos en un broker MQTT (TLS, puerto 8883)
✅ Se comunique con el Arduino UNO (I2C esclavo)
✅ Guarde estado en memoria NVS (no volátil)
✅ Ejecute tareas y temporizadores bajo FreeRTOS

## 🌐 Configuración de red y MQTT
const char* ssid = "moto";
const char* password = "12345678seh";

const char* mqtt_server = "aebfc567fc434938bf244e554c3cb715.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "prueba";
const char* mqtt_password = "Prueba1#";


El ESP32 se conecta primero a la red WiFi local y luego al servidor HiveMQ Cloud, usando autenticación por usuario y contraseña junto con un certificado raíz TLS para garantizar la seguridad.

## 🧭 Topics MQTT definidos
const char* CONTROL_MOTOR_TOPIC = "control-motor";   // Recibe ON/OFF
const char* TEMPERATURA_TOPIC   = "temperatura";     // Envía la lectura


📤 temperatura → El ESP32 publica periódicamente el valor leído desde el UNO.
📥 control-motor → El ESP32 recibe comandos para encender o apagar el motor de forma remota.

## ⚙️ Pines y dirección I2C
#define MOTOR_PIN 27
#define LED_PIN   26
#define I2C_SLAVE_ADDR 0x08

Elemento	Descripción
MOTOR_PIN (27)	Controla el motor
LED_PIN (26)	Indicador visual del estado del motor
I2C_SLAVE_ADDR	Dirección del Arduino UNO en el bus I2C
🔒 Certificado raíz TLS

El código incluye un Root Certificate Authority (CA) que valida la autenticidad del servidor MQTT.
Esto asegura que toda la comunicación esté cifrada y autenticada.

## 🧩 Objetos principales
Preferences preferences;       // Guarda configuración en memoria NVS
WiFiClientSecure secureClient; // Cliente TLS
PubSubClient client(secureClient); // Cliente MQTT sobre TLS


preferences → almacena el último valor de temperatura y estado del motor.

secureClient → establece la conexión TLS segura.

client → maneja las suscripciones y publicaciones MQTT.

## 🔧 Variables globales
volatile float temperatura = 0.0;
volatile bool motorEncendido = false;
volatile bool lecturaPending = false;

Variable	Descripción
temperatura	Última lectura recibida desde el UNO
motorEncendido	Estado actual del motor
lecturaPending	Bandera que indica cuándo leer nueva temperatura
🕒 Callback del Timer
void IRAM_ATTR timer_callback(void* arg) {
  lecturaPending = true;
  Serial.println("[Timer] Señal de lectura enviada.");
}


Cada 30 segundos, el timer activa la bandera lecturaPending indicando que debe leerse una nueva temperatura desde el Arduino UNO.

## 📡 Lectura por I2C
bool leerTemperaturaI2C(float &outTemp) {
  Wire.requestFrom(I2C_SLAVE_ADDR, (uint8_t)8);
  ...
  outTemp = atof(buffer);
}


Solicita al Arduino UNO la temperatura actual y la convierte de texto a número flotante (float).

## 📤 Publicar temperatura en MQTT
void publicarTemperatura(float temp) {
  dtostrf(temp, 4, 2, buffer);
  client.publish(TEMPERATURA_TOPIC, buffer, true);
}


Convierte la lectura a string y la publica en el broker MQTT con retención (retain=true).

## 📥 Callback MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, CONTROL_MOTOR_TOPIC) == 0) {
    if (payload[0] == '1') { ... }  // Encender motor
    else if (payload[0] == '0') { ... } // Apagar motor
  }
}


Recibe comandos remotos y actualiza el estado del motor, LED y memoria NVS.

## 🔄 Tareas principales (FreeRTOS)
## 🧵 taskMQTT

Mantiene conexión activa con el broker.

Reconecta si se pierde.

Se suscribe al topic control-motor.

## ⚙️ taskMotor

Cada vez que lecturaPending es true:

Lee temperatura del Arduino UNO (I2C).

Guarda en memoria NVS.

Aplica control automático:

🔥 T > 30 °C → motor ON

❄️ T < 25 °C → motor OFF

Publica el nuevo valor en MQTT.

## 🚀 Setup del ESP32
void setup() {
  inicializarWiFi();
  inicializarMQTT();
  configurarTimer();
  crearTareasFreeRTOS();
}


Pasos realizados:

Inicializa los pines y comunicación I2C.

Conecta a WiFi.

Recupera último estado guardado.

Configura cliente MQTT con TLS.

Crea las tareas (taskMQTT, taskMotor).

Inicia el temporizador de 30 s.

## 🔁 Loop principal
void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}


No ejecuta lógica directa: todo se maneja con tareas y temporizadores del sistema operativo FreeRTOS.

## 📊 Flujo de funcionamiento

Arduino UNO mide la temperatura (DHT11) y la entrega por I2C.

ESP32 cada 30 s pide el valor y lo publica en el broker MQTT (TLS).

El broker envía comandos remotos (control-motor) para encender/apagar el motor.

El ESP32 también aplica control automático local.

Los valores se guardan en memoria NVS para persistir tras reinicio.

🧠 Esquema general del sistema
[DHT11] → [Arduino UNO (I2C esclavo)] → [ESP32 (WiFi + MQTT TLS)] → [HiveMQ Cloud]
                                                  ↓
                                               [Motor + LED]