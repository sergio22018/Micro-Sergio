const int botonInicio = 2;
const int botonParo   = 3;
const int ledRojo     = 8;
const int ledAzul     = 9;

bool emergenciaActiva = false;
bool estadoBotonInicio, estadoBotonParo;
bool estadoAntInicio = HIGH, estadoAntParo = HIGH;

void setup() {
  pinMode(botonInicio, INPUT_PULLUP);
  pinMode(botonParo,   INPUT_PULLUP);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledAzul, LOW);
}

void loop() {
  estadoBotonInicio = digitalRead(botonInicio);
  estadoBotonParo   = digitalRead(botonParo);

  // Flanco de bajada: inicia
  if (estadoAntInicio == HIGH && estadoBotonInicio == LOW) {
    emergenciaActiva = true;
  }

  // Flanco de bajada: para
  if (estadoAntParo == HIGH && estadoBotonParo == LOW) {
    emergenciaActiva = false;
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledAzul, LOW);
  }

  estadoAntInicio = estadoBotonInicio;
  estadoAntParo   = estadoBotonParo;

  // Parpadeo (bloqueante)
  if (emergenciaActiva) {
    digitalWrite(ledRojo, HIGH);
    digitalWrite(ledAzul, LOW);
    delay(200);
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledAzul, HIGH);
    delay(200);
  }
}