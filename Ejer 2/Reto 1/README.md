# RETO 1
## Explicacion del code
 Para la solucion de este codigo:
 ### Funcionamiento

El pin 2 está configurado con INPUT_PULLUP, por lo que normalmente está en nivel alto (HIGH).

Cuando el botón se presiona, el pin cambia momentáneamente a bajo (LOW) y luego vuelve a alto (HIGH).

Ese cambio de bajo a alto (RISING) activa la interrupción externa.

La interrupción ejecuta la función toggleContador(), la cual:

Incrementa la variable Contador.

Imprime el valor por el monitor serial.

Espera 3 segundos para evitar rebotes o pulsos muy seguidos.

### codigo comentado
// ----------------------
// Declaración de variables
// ----------------------

// Pin donde está conectado el botón
const int Boton = 2;

// Variable que guarda el número de veces que se presionó el botón
int Contador = 0;


// ----------------------
// Configuración inicial
// ----------------------
void setup()
{
  // Configura el pin del botón como entrada con resistencia interna pull-up.
  // Esto significa que el pin estará normalmente en estado HIGH (1)
  // y pasará a LOW (0) cuando se presione el botón.
  pinMode(Boton, INPUT_PULLUP);

  // Inicializa la comunicación serial a 9600 baudios.
  // Permite enviar mensajes al monitor serial.
  Serial.begin(9600);

  // Asocia una interrupción externa al pin del botón.
  // digitalPinToInterrupt(Boton) convierte el número de pin a número de interrupción.
  // toggleContador es la función que se ejecutará cuando ocurra el evento.
  // RISING indica que la interrupción se activa cuando el pin pasa de LOW → HIGH.
  attachInterrupt(digitalPinToInterrupt(Boton), toggleContador, RISING);
}


// ----------------------
// Bucle principal
// ----------------------
void loop()
{
  // El loop está vacío porque el conteo se maneja mediante interrupciones.
  // El microcontrolador está a la espera de eventos (pulsaciones del botón).
}


// ----------------------
// Función de interrupción
// ----------------------
void toggleContador()
{
  // Incrementa el contador cada vez que se activa la interrupción
  Contador++;

  // Muestra el valor actual del contador en el monitor serial
  Serial.println(Contador);

  // Espera 3 segundos para evitar lecturas múltiples (rebotes)
  delay(3000);
}
