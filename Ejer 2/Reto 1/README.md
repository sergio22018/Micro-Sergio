const int Boton = 2;
int Contador = 0;

void setup()
{
  pinMode(Boton,INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(Boton),toggleContador,RISING);
    
}

void loop()
{  
}
void toggleContador(){
  
    Contador++; 
   Serial.println(Contador);
   delay(3000);
   }