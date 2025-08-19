const int pinBoton = 13;     
const int leds[] = {2, 3, 4, 5, 6, 7, 8, 9}; 
const int numLeds = 8;      
bool estadoBoton;

void setup() {
  pinMode(pinBoton, INPUT_PULLUP); 
  
  
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW); 
  }
}

void loop() {
  estadoBoton = digitalRead(pinBoton);

  if (estadoBoton == HIGH) {  
     for (int i = 0; i < numLeds; i++) {
      digitalWrite(leds[i], HIGH);
      delay(100); 
     }
  } 
  else {  
    
    for (int i = numLeds - 1; i >= 0; i--) {
      digitalWrite(leds[i], LOW);
      delay(100); 
      
      
    }
  }
}
