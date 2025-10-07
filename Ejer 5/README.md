# Introducción al Protocolo I²C con Arduino y ESP32
### (explicado por el ingeniero y puesto en practica)
## 1. ¿Qué es I²C?
I²C (Inter-Integrated Circuit) es un protocolo de comunicación serial que permite conectar múltiples dispositivos (sensores, memorias, expansores, etc.) usando solo **dos líneas**:
- **SDA (Serial Data):** Línea de datos.
- **SCL (Serial Clock):** Línea de reloj.

Características:
- Comunicación **maestro–esclavo**.
- Soporta múltiples dispositivos en el mismo bus.
- Identificación por **dirección única** para cada esclavo.
- Velocidades comunes: 100 kHz (estándar), 400 kHz (rápido).

---

## 2. Conexiones Básicas
### Arduino UNO
- **SDA:** Pin A4  
- **SCL:** Pin A5  

### ESP32
- **SDA:** GPIO 21 (configurable)  
- **SCL:** GPIO 22 (configurable)  

> Nota: Todos los dispositivos deben compartir **GND**.

---

## 3. Escaneo de Dispositivos I²C
Antes de trabajar con un sensor, es recomendable conocer su dirección I²C.

```cpp
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Escaneando dispositivos I2C...");
}

void loop() {
  byte error, address;
  int dispositivos = 0;

  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado en 0x");
      Serial.println(address, HEX);
      dispositivos++;
    }
  }
  
  if (dispositivos == 0) {
    Serial.println("No se encontraron dispositivos.");
  }
  delay(5000);
}

```

## 4. Ejemplo Maestro–Esclavo

Maestro (Arduino/ESP32)
```cpp
#include <Wire.h>

void setup() {
  Wire.begin(); // Inicia como maestro
  Serial.begin(115200);
}

void loop() {
  Wire.beginTransmission(8); // Dirección del esclavo
  Wire.write("Hola");
  Wire.endTransmission();
  delay(1000);
}
```

Esclavo (Arduino UNO)

```cpp
#include <Wire.h>

void recibirEvento(int howMany) {
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}

void setup() {
  Wire.begin(8); // Dirección esclavo
  Wire.onReceive(recibirEvento);
  Serial.begin(115200);
}

void loop() {
  delay(100);
}
```

## 5. Lectura de un Sensor I²C (Ejemplo: BMP280)

```cpp
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(115200);
  if (!bmp.begin(0x76)) { // Dirección I2C del BMP280
    Serial.println("No se encontró el BMP280");
    while (1);
  }
}

void loop() {
  Serial.print("Temperatura = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print("Presión = ");
  Serial.print(bmp.readPressure()/100.0F);
  Serial.println(" hPa");
  
  delay(2000);
}
```

## 6. Aplicaciones Comunes de I²C
Sensores ambientales (BMP280, BME280, MPL3115A2, etc.)

Expansores de puertos (PCF8574).

Pantallas LCD con adaptador I²C.

Comunicación entre dos microcontroladores.

## 7. Retos Propuestos

- Escaneo de dispositivos: Realiza un programa que detecte y muestre en pantalla todos los dispositivos I²C conectados.

- Comunicación Maestro–Esclavo: Envía un número desde el maestro y haz que el esclavo lo reciba e imprima en el monitor serial.

- Sensor + Pantalla: Conecta un sensor I²C (ej. BMP280) y muestra los valores de temperatura y presión en una pantalla LCD con módulo I²C.

- Red de Dispositivos: Conecta dos Arduinos/ESP32 (maestros) y un esclavo sensor, donde los maestros soliciten información en diferentes momentos.