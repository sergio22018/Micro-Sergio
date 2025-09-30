const int Led = 13;  // LED en pin 13 (o externo)

void setup() {
  pinMode(Led, OUTPUT);

  noInterrupts();           // Desactivar interrupciones globales

  TCCR1A = 0;               // Limpiar registros de control
  TCCR1B = 0;
  TCNT1  = 0;               // Reiniciar contador

  // Configuración de comparación
  OCR1A = 7812;             // Valor para 500 ms con prescaler 1024

  // Modo CTC: Clear Timer on Compare Match
  TCCR1B |= (1 << WGM12);

  // Prescaler de 1024 (CS12 y CS10 en 1)
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // Habilitar interrupción por comparación con OCR1A
  TIMSK1 |= (1 << OCIE1A);

  interrupts();             // Activar interrupciones globales
}

// ISR que se ejecuta cada 500 ms
ISR(TIMER1_COMPA_vect) {
  digitalWrite(Led, !digitalRead(Led));  // Invertir estado del LED
}

void loop() {
  // El loop queda vacío: todo lo maneja el Timer1
}
