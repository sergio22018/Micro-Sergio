// Pines
const int boton = 2;          
const int leds[] = {8, 9, 10, 11}; 
const int numLeds = 4;

// Variables
int contador = 0;              
bool estadoBotonAnterior = HIGH;

void setup() {
  pinMode(boton, INPUT_PULLUP);
  Serial.begin(9600); // Poder escribir en el monitor serial

  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  bool estadoBoton = digitalRead(boton);

  // Detección de flanco de bajada
  if (estadoBoton == LOW && estadoBotonAnterior == HIGH) {
    contador++;                 // Incrementar
    if (contador > 15) contador = 0; // Reiniciar si supera 15
    mostrarBinario(contador);   // Actualizar LEDs
    delay(50);  // Anti-rebote básico
    Serial.println(contador); // Imprimr el numero en el monitor serial
  }

  estadoBotonAnterior = estadoBoton;
}

// Función para mostrar un número en binario con LEDs
void mostrarBinario(int valor) {
  for (int i = 0; i < numLeds; i++) {
    bool bit = bitRead(valor, i); // Leer bit i del valor
    digitalWrite(leds[i], bit);
  }
}