# RETO 1

## Explicacion del code
 Para la solucion de este codigo:
 ### Código explicado y comentado
#include <EEPROM.h>  // Librería para usar la memoria EEPROM interna del Arduino


La EEPROM (Electrically Erasable Programmable Read-Only Memory) es una pequeña memoria no volátil, es decir, no se borra al apagar el Arduino.

Esta librería permite leer y escribir datos en ella fácilmente.

const int conexion = 0;  // Dirección de memoria EEPROM donde se guarda el contador
int contador = 0;        // Variable que almacenará el número de conexiones


La EEPROM se organiza como una matriz de direcciones (como celdas de memoria).

Aquí se usa la posición 0 para guardar el valor del contador.

En un Arduino UNO hay 1024 posiciones (de 0 a 1023).

🏁 Función setup()
void setup() {
  Serial.begin(9600);  // Inicializa la comunicación serial para mostrar datos en el monitor


Se inicia la comunicación con el PC a 9600 baudios, para poder ver el resultado en el monitor serial.

  contador = EEPROM.read(conexion);  // Lee el valor almacenado en la dirección 0


EEPROM.read() devuelve el valor guardado en esa dirección (entre 0 y 255).

La primera vez que se ejecuta, esa celda puede contener basura (valor aleatorio) si nunca se escribió antes.

  contador++;  // Incrementa el contador (una conexión más)


Cada vez que el Arduino se enciende, aumenta el valor leído en 1.

  EEPROM.write(conexion, contador);  // Guarda el nuevo valor en la EEPROM


Se sobrescribe el valor anterior con el nuevo contador.

Así, cuando el Arduino se apague y vuelva a encender, el número se conservará.

⚠️ Advertencia:
La EEPROM tiene un límite de escritura (aprox. 100,000 ciclos por celda), así que no conviene escribir constantemente en el loop().

  Serial.print("conexion num: ");
  Serial.println(contador);  // Muestra el número total de veces que se encendió el Arduino
}


Imprime en el monitor serial el número de veces que el Arduino ha sido encendido.

🔁 Función loop()
void loop() {
  // No se hace nada en el ciclo principal
}


No hay código en el bucle porque el conteo solo ocurre una vez al iniciar.

📊 Ejemplo de salida en el monitor serial

Supón que conectas y desconectas el Arduino varias veces.
Verás algo como esto:

conexion num: 1
conexion num: 2
conexion num: 3
conexion num: 4


Cada vez que se inicia el programa, el número aumenta en 1.