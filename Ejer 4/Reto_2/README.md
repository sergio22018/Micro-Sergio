# RETO 2
## Explicacion del code
 Para la solucion de este codigo:
 ### Explicación detallada del código
#include <Arduino.h>


👉 Incluye la librería base de Arduino (necesaria para usar pinMode, digitalWrite, etc.).

// Pines
const int ledBajo = 5;      // LED de baja prioridad
const int ledAlto = 6;      // LED de alta prioridad
const int boton = 7;        // Botón
const int pot = A0;         // Potenciómetro


👉 Se definen los pines usados:

ledBajo (5): indicador de la tarea de baja prioridad.

ledAlto (6): indicador de la tarea de alta prioridad.

boton (7): entrada con pulsador.

pot (A0): entrada analógica para el potenciómetro.

// Declaración de tareas
void TareaBaja(void *pvParameters);
void TareaMedia(void *pvParameters);
void TareaAlta(void *pvParameters);


👉 Se declaran las funciones que representarán las tres tareas que ejecutará FreeRTOS.

⚙️ Configuración inicial
void setup() {
  pinMode(ledBajo, OUTPUT);
  pinMode(ledAlto, OUTPUT);
  pinMode(boton, INPUT_PULLUP);

  Serial.begin(9600);


👉 Configura los pines:

Los LED como salidas.

El botón como entrada con resistencia pull-up interna (valor en reposo = HIGH, pulsado = LOW).
También inicia la comunicación serial para enviar datos del potenciómetro.

🔸 Creación de las tareas
  xTaskCreate(
    TareaBaja,
    "LED_Bajo",
    128,
    NULL,
    1,         // Prioridad baja
    NULL
  );


👉 Crea la tarea de baja prioridad (TareaBaja) con un stack de 128 bytes y prioridad = 1.
Esta tarea se ejecutará solo cuando no haya tareas más urgentes pendientes.

  xTaskCreate(
    TareaMedia,
    "Lectura_Pot",
    128,
    NULL,
    2,         // Prioridad media
    NULL
  );


👉 Crea la tarea de prioridad media, encargada de leer el potenciómetro cada 500 ms.
Tiene prioridad 2, por encima de la tarea baja.

  xTaskCreate(
    TareaAlta,
    "Boton",
    128,
    NULL,
    3,         // Prioridad alta
    NULL
  );


👉 Crea la tarea de prioridad alta, la más importante, que revisa constantemente el botón.
Con prioridad 3, interrumpe a las otras tareas si es necesario.

  vTaskStartScheduler(); // Inicia el planificador
}


👉 Inicia el planificador de FreeRTOS, que se encarga de distribuir el tiempo de CPU entre las tareas según su prioridad.
A partir de aquí, loop() no se ejecuta nunca más.

🔄 loop vacío
void loop() {
  // No se usa con FreeRTOS
}


👉 En FreeRTOS, el bucle principal no se usa, ya que el sistema operativo gestiona las tareas por su cuenta.

💡 Tarea de baja prioridad: Parpadeo del LED
void TareaBaja(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(ledBajo, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(ledBajo, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


👉 Esta tarea hace parpadear el LED de baja prioridad cada 1 segundo (500 ms encendido y 500 ms apagado).

vTaskDelay() no bloquea el micro, solo cede el control a otras tareas.

Si alguna tarea de mayor prioridad necesita ejecutarse, interrumpe esta sin problema.

🎛️ Tarea de prioridad media: Lectura del potenciómetro
void TareaMedia(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    int valor = analogRead(pot);
    Serial.print("Potenciometro: ");
    Serial.println(valor);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


👉 Esta tarea lee el valor del potenciómetro (A0) cada 500 ms y lo muestra por el monitor serial.
Tiene prioridad media (2), por lo que interrumpe la tarea del LED bajo si necesita ejecutarse.

🛎️ Tarea de prioridad alta: Botón
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


👉 Esta es la tarea más prioritaria (nivel 3).
Lee el botón cada 10 ms y enciende o apaga el LED alto según su estado.

Al usar vTaskDelay(10), evita saturar la CPU y también filtra rebotes del botón.

Si el usuario presiona el botón, el LED responde inmediatamente, incluso si las otras tareas están corriendo.