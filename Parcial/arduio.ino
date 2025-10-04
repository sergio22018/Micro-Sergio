// ================= INCLUDES =================
#include <Wire.h>              // Librería para comunicación I2C
#include <DHT.h>               // Librería para sensor DHT11 (temperatura y humedad)
#include <LiquidCrystal_I2C.h> // Librería para controlar la pantalla LCD I2C

// ================= DEFINICIONES =================
#define DHTPIN 2       // Pin digital donde está conectado el DHT11
#define DHTTYPE DHT11  // Tipo de sensor (DHT11)

// Se crea el objeto del sensor DHT11 en el pin 2
DHT dht(DHTPIN, DHTTYPE);

// Se crea el objeto del LCD en dirección I2C 0x27, con 16 columnas y 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variable global para almacenar la temperatura leída
float temperatura = 0.0;

// ================= SETUP =================
void setup() {
  Wire.begin(8); // Inicializa el Arduino UNO como esclavo I2C con dirección 0x08
  dht.begin();   // Inicializa el sensor DHT11
  lcd.init();    // Inicializa la pantalla LCD
  lcd.backlight(); // Enciende la retroiluminación del LCD

  Serial.begin(9600); // Inicializa el puerto serie a 9600 baudios
  Serial.println("Iniciando Arduino UNO (I2C esclavo + LCD + DHT11)");

  // Define la función que se ejecutará cuando el maestro (ESP32) pida datos por I2C
  Wire.onRequest(enviarTemp);
}

// ================= LOOP =================
void loop() {
  // Leer temperatura del sensor DHT11
  temperatura = dht.readTemperature();

  // Verificar si la lectura es válida
  if (!isnan(temperatura)) {
    // ----- Mostrar en el LCD -----
    lcd.clear();           // Limpia la pantalla
    lcd.setCursor(0, 0);   // Posiciona el cursor en la primera fila, columna 0
    lcd.print("Temp: ");   // Muestra el texto fijo
    lcd.print(temperatura);// Muestra el valor de la temperatura
    lcd.print(" C");       // Muestra la unidad °C

    // ----- Mostrar en el Monitor Serie -----
    Serial.print("Temperatura actual: ");
    Serial.print(temperatura);
    Serial.println(" °C");
  } else {
    // Si hubo error de lectura del sensor
    Serial.println("Error leyendo DHT11");
  }

  delay(2000); // Espera 2 segundos antes de volver a medir
}

// ================= FUNCIÓN DE RESPUESTA I2C =================
void enviarTemp() {
  char buffer[10]; // Buffer para almacenar la temperatura en formato texto

  // Convierte el float a texto con 2 decimales:
  // (valor, ancho mínimo, nº decimales, buffer destino)
  dtostrf(temperatura, 4, 2, buffer);

  // Envía el string al maestro (ESP32) por I2C
  Wire.write(buffer);
}
