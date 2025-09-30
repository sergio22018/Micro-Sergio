# RETO 1
## Explicacion del code
Para la solucion de este code:

  Explicación rápida

INPUT_PULLUP: Se usa para evitar resistencias externas. Esto invierte la lógica del botón (presionado = LOW).

Variable estadoLED: Guarda si el LED está encendido o apagado.

Detección de flanco:

-Evita que el LED cambie de estado varias veces mientras mantienes presionado el botón.

-Solo se cambia el estado cuando detecta que el botón fue presionado (transición de HIGH a LOW).

## COMO SE USA
Se crean 2 variables :
 -const int pinBoton = 13; 
 -const int pinLED = 8;    

El control del estado del boton:
 -bool estadoLED = false;   
 -bool estadoBoton;         
 -bool estadoBotonAnterior = HIGH; 

Se configura la funcion de las variables:

Se realiza una vez --void setup() {
                      pinMode(pinBoton, INPUT_PULLUP); 
                      pinMode(pinLED, OUTPUT);         
                            }

Se configura el siclo que se repite varial veces y 
se crea el siclo "if", para la funcion:

void loop() {
  
  estadoBoton = digitalRead(pinBoton);

  
  if (estadoBoton == LOW && estadoBotonAnterior == HIGH) {
       estadoLED = !estadoLED;

        digitalWrite(pinLED, estadoLED);
  }
  estadoBotonAnterior = estadoBoton;
}

