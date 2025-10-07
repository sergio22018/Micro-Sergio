# RETO 2
## Explicacion del code
 Para la solucion de este codiogo:
 ### Funcionamiento general

El LED está conectado al pin 11, que admite señal PWM.
analogWrite() controla su brillo con valores entre 0 (apagado) y 255 (máximo brillo).

El botón está conectado al pin 3, con resistencia interna INPUT_PULLUP.
Cuando se presiona, genera una interrupción.

Cada vez que se presiona el botón:

Se ejecuta toggleestadoBotonAnterior().

Esta función invierte el estado de estadoBotonAnterior.

En el siguiente ciclo del loop(), el LED cambia la dirección de cambio de brillo.

El bucle principal:

Ajusta el brillo cada 500 ms (+15 o -15 unidades).

Cuando llega al máximo (255) o mínimo (0), se detiene en ese extremo.

Si se pulsa el botón, cambia de dirección (por ejemplo, de aumentar a disminuir).
 ### Codigo comentado
 // ----------------------
// Declaración de constantes y variables
// ----------------------

// Pin donde está conectado el botón
const int Boton = 3;

// Pin PWM donde está conectado el LED
const int LedPWM = 11;

// Variable que guarda el nivel de brillo actual del LED (0–255)
int Brillo = 0;

// Valor del incremento/decremento del brillo en cada ciclo
const int Crece = 15;       

// Variable que indica si el LED debe aumentar (true) o disminuir (false) el brillo
// Se usa "volatile" porque cambia dentro de una interrupción
volatile bool estadoBotonAnterior = true; 


// ----------------------
// Configuración inicial
// ----------------------
void setup() {
  // Configura el pin del botón como entrada con resistencia interna pull-up
  // (normalmente HIGH, pasa a LOW al presionar)
  pinMode(Boton, INPUT_PULLUP);

  // Configura el pin del LED como salida PWM
  pinMode(LedPWM, OUTPUT);

  // Configura una interrupción en el pin del botón
  // Se ejecuta la función toggleestadoBotonAnterior cuando hay un flanco de subida (LOW → HIGH)
  attachInterrupt(digitalPinToInterrupt(Boton), toggleestadoBotonAnterior, RISING);
}


// ----------------------
// Bucle principal
// ----------------------
void loop() {

  // Aplica el valor de brillo actual al LED usando PWM (0 = apagado, 255 = máximo brillo)
  analogWrite(LedPWM, Brillo);

  // Si estadoBotonAnterior es true → aumenta el brillo
  if (estadoBotonAnterior) {
    Brillo += Crece;
    if (Brillo > 255) Brillo = 255;  // Limita el máximo brillo a 255
  } 
  // Si estadoBotonAnterior es false → disminuye el brillo
  else {
    Brillo -= Crece;
    if (Brillo <= 0) Brillo = 0;     // Limita el brillo mínimo a 0
  }

  // Pausa de medio segundo antes del siguiente cambio
  delay(500);
}


// ----------------------
// Función de interrupción
// ----------------------
void toggleestadoBotonAnterior() {
  // Invierte el estado del botón cada vez que se presiona:
  // true → false, false → true
  estadoBotonAnterior = !estadoBotonAnterior;
}
