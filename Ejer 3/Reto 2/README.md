# RETO 2

## Explicacion del code
 Para la solucion de este codigo:
 ### Código explicado línea por línea
#include <EEPROM.h>  


Se incluye la librería EEPROM.h, que permite leer y escribir datos persistentes en la memoria EEPROM del microcontrolador.

La EEPROM mantiene su contenido aunque el Arduino se reinicie o se desconecte de la corriente.

const int ledPin = 13;     // LED en el pin digital 13 (el LED integrado del Arduino)
int opcion;                // Variable que almacenará la velocidad seleccionada
const int addr = 0;        // Dirección en la EEPROM donde se guardará la opción


ledPin: pin de salida para el LED.

opcion: almacena el modo actual de parpadeo (0 = lento, 1 = medio, 2 = rápido).

addr: dirección en la EEPROM (posición de memoria) donde se guardará esa opción.

int tiempos[] = {1000, 500, 200}; // Tiempos de parpadeo (ms): lento, medio, rápido


Se define un arreglo con los tiempos de encendido y apagado en milisegundos.

Dependiendo del valor de opcion, se usa uno de estos tiempos.

tiempos[0] = 1000 ms → 1 segundo

tiempos[1] = 500 ms → 0.5 segundos

tiempos[2] = 200 ms → 0.2 segundos

⚙️ Función setup()
void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);


Se inicia la comunicación Serial a 9600 baudios.

Se configura el pin del LED como salida.

  EEPROM.get(addr, opcion);  // Leer valor guardado desde la EEPROM


EEPROM.get() permite leer un tipo de dato completo (por ejemplo int, float, etc.) desde la EEPROM.

Aquí recupera el valor que estaba guardado en la dirección 0 (addr).

  if (opcion < 0 || opcion > 2) {
    opcion = 0; // Si el dato está corrupto o vacío, usar modo lento (por defecto)
  }


Se valida el valor leído.

Si no corresponde a una opción válida (0, 1 o 2), se asigna el valor por defecto 0 (modo lento).

📜 Muestra un menú informativo en el Monitor Serial:

  Serial.println("=== MENU DE CONFIGURACION ===");
  Serial.println("0: Lenta (1s)");
  Serial.println("1: Media (0.5s)");
  Serial.println("2: Rapida (0.2s)");
  Serial.println("-----------------------------");
  Serial.print("Configuracion actual: ");
  Serial.println(opcion);
  Serial.println("Escribe el numero de la opcion y presiona ENTER.");
}


Al iniciar, el usuario ve el menú con las tres opciones posibles.

También muestra la configuración actual (leída de la EEPROM).

Luego pide al usuario que escriba una opción en el monitor serial (por ejemplo 1 o 2) y presione ENTER.

🔁 Función loop()

Aquí se ejecuta el programa principal en dos partes:

🧭 1. Lectura de la entrada del usuario:
if (Serial.available() > 0) {
  int seleccion = Serial.parseInt(); // Lee número ingresado


Serial.available() indica si hay datos pendientes en el buffer serial.

Serial.parseInt() convierte el texto ingresado por el usuario (por ejemplo “1”) en un número entero.

  if (seleccion >= 0 && seleccion <= 2) {
    opcion = seleccion;
    EEPROM.put(addr, opcion);  // Guardar la nueva configuración
    Serial.print("Nueva configuracion guardada: ");
    Serial.println(opcion);
  }


Si el valor ingresado está dentro del rango permitido (0, 1 o 2), se guarda en la variable opcion.

Luego se usa EEPROM.put() para almacenar el valor en la EEPROM.
🟢 A diferencia de EEPROM.write(), esta función escribe solo si el valor cambió, evitando desgaste prematuro de la memoria.

Finalmente, se confirma el nuevo modo al usuario.

  while (Serial.available() > 0) {
    Serial.read(); // Limpia el buffer de entrada
  }
}


Se vacía el buffer serial para evitar lecturas repetidas del mismo dato.

💡 2. Control del LED según la opción almacenada:
digitalWrite(ledPin, HIGH);
delay(tiempos[opcion]);
digitalWrite(ledPin, LOW);
delay(tiempos[opcion]);


Enciende el LED durante el tiempo correspondiente (tiempos[opcion]).

Luego lo apaga durante el mismo tiempo.

Así el LED parpadea a distinta velocidad según la opción guardada:

0 → lento (1 segundo)

1 → medio (0.5 segundos)

2 → rápido (0.2 segundos)

📊 Ejemplo de ejecución

1️⃣ Abres el monitor serial (9600 baudios).
2️⃣ El programa muestra:

=== MENU DE CONFIGURACION ===
0: Lenta (1s)
1: Media (0.5s)
2: Rapida (0.2s)
-----------------------------
Configuracion actual: 1
Escribe el numero de la opcion y presiona ENTER.


3️⃣ Escribes 2 → ENTER
El LED empieza a parpadear rápido y se muestra:

Nueva configuracion guardada: 2


4️⃣ Si apagas y vuelves a encender el Arduino, el LED sigue parpadeando rápido, porque la configuración quedó guardada en la EEPROM.