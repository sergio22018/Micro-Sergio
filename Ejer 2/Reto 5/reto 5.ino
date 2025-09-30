const int buzzerPin = 9;   // Pin conectado al buzzer
volatile bool alarmaActiva = false;
volatile bool toggleBuzzer = false;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);

  // --- Interrupción externa en pin 2 (INT0) para activar alarma ---
  attachInterrupt(digitalPinToInterrupt(2), activarAlarma, RISING);

  // --- Interrupción externa en pin 3 (INT1) para desactivar alarma ---
  attachInterrupt(digitalPinToInterrupt(3), desactivarAlarma, RISING);

  // --- Configurar Timer2 en modo CTC ---
  noInterrupts();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;

  // OCR2A determina la frecuencia del buzzer
  // Ejemplo: 16 MHz / (prescaler 64 * (249+1)) = 1 kHz
  OCR2A = 249;             
  TCCR2A |= (1 << WGM21);   // Modo CTC
  TCCR2B |= (1 << CS22);    // Prescaler 64
  TIMSK2 |= (1 << OCIE2A);  // Habilitar interrupción
  interrupts();
}

void loop() {
  // El loop no hace nada, todo lo manejan las ISR
}

// ISR: Activar alarma
void activarAlarma() {
  alarmaActiva = true;
  Serial.println("🚨 Alarma activada");
}

// ISR: Desactivar alarma
void desactivarAlarma() {
  alarmaActiva = false;
  digitalWrite(buzzerPin, LOW);
  Serial.println("✅ Alarma desactivada");
}

// ISR del Timer2: genera el sonido del buzzer
ISR(TIMER2_COMPA_vect) {
  if (alarmaActiva) {
    toggleBuzzer = !toggleBuzzer;
    digitalWrite(buzzerPin, toggleBuzzer);
  }
}
