// Pines
const int botonSubir = 2;   // Botón para aumentar brillo
const int botonBajar = 13;   // Botón para disminuir brillo
const int ledPWM = 6;       // LED en pin PWM (usar 3, 5, 6, 9, 10 o 11 en Arduino UNO)

// Variables
int brillo = 0;              // Nivel de brillo (0-255)
const int paso = 15;         // Incremento/decremento por pulsación
bool estadoAnteriorSubir = HIGH;
bool estadoAnteriorBajar = HIGH;

void setup() {
  pinMode(botonSubir, INPUT_PULLUP);
  pinMode(botonBajar, INPUT_PULLUP);
  pinMode(ledPWM, OUTPUT);
}

void loop() {
  bool estadoSubir = digitalRead(botonSubir);
  bool estadoBajar = digitalRead(botonBajar);

  // Botón de aumento
  if (estadoSubir == LOW && estadoAnteriorSubir == HIGH) {
    brillo += paso;
    if (brillo > 255) brillo = 255; // Limitar al máximo
    analogWrite(ledPWM, brillo);
    delay(50); // Anti-rebote
  }

  // Botón de disminución
  if (estadoBajar == LOW && estadoAnteriorBajar == HIGH) {
    brillo -= paso;
    if (brillo < 0) brillo = 0; // Limitar al mínimo
    analogWrite(ledPWM, brillo);
    delay(50); // Anti-rebote
  }

  // Guardar estados previos
  estadoAnteriorSubir = estadoSubir;
  estadoAnteriorBajar = estadoBajar;
}