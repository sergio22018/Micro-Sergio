// Pines del semáforo
const int ledVerde = 3;
const int ledAmarillo = 4;
const int ledRojo = 5;

// LED peatonal
const int ledPeaton = 6;

// Botón
const int botonPeaton = 2;

// Variables
bool botonPresionado = false;

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledPeaton, OUTPUT);
  pinMode(botonPeaton, INPUT_PULLUP);

  // Estado inicial: verde encendido
  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledPeaton, LOW);
}

void loop() {
  // Verificar si se presionó el botón
  if (digitalRead(botonPeaton) == LOW) {
    botonPresionado = true;
  }

  // Si el botón fue presionado, ejecutar secuencia peatonal
  if (botonPresionado) {
    // Apagar verde, encender amarillo
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarillo, HIGH);
    delay(2000); // Amarillo por 2 segundos

    // Encender rojo
    digitalWrite(ledAmarillo, LOW);
    digitalWrite(ledRojo, HIGH);

    // Encender LED peatonal
    digitalWrite(ledPeaton, HIGH);
    delay(5000); // Peatones cruzan 5 segundos

    // Apagar peatonal y volver a verde
    digitalWrite(ledPeaton, LOW);
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledVerde, HIGH);

    botonPresionado = false; // Reiniciar estado
  }
}