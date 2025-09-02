#include <EEPROM.h>
const int conexion = 0  ;
int contador = 0 ;
void setup() {

Serial.begin(9600);

contador = EEPROM.read (conexion);
contador++;

EEPROM.write(conexion, contador);

Serial.print("conexion num:");
Serial.println(contador);

}

void loop() {
  // put your main code here, to run repeatedly:

}
