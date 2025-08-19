const int pinBoton = 13; 
const int pinLED = 8;    

bool estadoLED = false;   
bool estadoBoton;         
bool estadoBotonAnterior = HIGH; 

void setup() {
  pinMode(pinBoton, INPUT_PULLUP); 
  pinMode(pinLED, OUTPUT);         
  }

void loop() {
  
  estadoBoton = digitalRead(pinBoton);

  
  if (estadoBoton == LOW && estadoBotonAnterior == HIGH) {
       estadoLED = !estadoLED;

        digitalWrite(pinLED, estadoLED);
  }
  estadoBotonAnterior = estadoBoton;
}
