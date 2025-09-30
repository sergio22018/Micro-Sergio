volatile unsigned int pulsos = 0;  // Contador de pulsos
volatile unsigned int frecuencia = 0;

void setup() {
  Serial.begin(9600);

  // --- Configurar interrupción externa INT0 (pin 2) ---
  attachInterrupt(digitalPinToInterrupt(2), contarPulsos, RISING);
  // Opciones: RISING, FALLING, CHANGE

  // --- Configurar Timer1 en modo CTC para 1s ---
  noInterrupts();           // Apagar interrupciones globales

  TCCR1A = 0;               // Reset registros
  TCCR1B = 0;
  TCNT1  = 0;

  // Valor de comparación para 1s
  // 16MHz / 1024 = 15625 ticks por segundo
  OCR1A = 15624;            

  TCCR1B |= (1 << WGM12);   // Modo CTC
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024

  TIMSK1 |= (1 << OCIE1A);  // Habilitar interrupción por comparación

  interrupts();             // Activar interrupciones globales
}

void loop() {
  // Solo mostramos resultados si hay nueva frecuencia calculada
  static unsigned int ultimaFrecuencia = 0;

  if (frecuencia != ultimaFrecuencia) {
    Serial.print("Frecuencia: ");
    Serial.print(frecuencia);
    Serial.println(" Hz");
    ultimaFrecuencia = frecuencia;
  }
}

// ISR: cada pulso externo
void contarPulsos() {
  pulsos++;
}

// ISR: cada 1 segundo (Timer1)
ISR(TIMER1_COMPA_vect) {
  frecuencia = pulsos;  // Guardamos número de pulsos por segundo
  pulsos = 0;           // Reiniciamos contador
}
