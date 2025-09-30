const int Boton =3;
const int LedPWM = 11;

int Brillo = 0;
const int Crece = 15;       
volatile bool estadoBotonAnterior = true; //Cambio de setado subida a bajada y biceversa


void setup(){
  
  pinMode(Boton,INPUT_PULLUP);
  pinMode(LedPWM,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(Boton), toggleestadoBotonAnterior, RISING);
}

void loop(){
  
  analogWrite(LedPWM,Brillo);
  
  if (estadoBotonAnterior) {
    Brillo+=Crece;
    if(Brillo>255)Brillo=255;
  }else{
    Brillo-=Crece;
    if (Brillo <= 0) Brillo = 0;
  }
   delay(500);
  
}
void toggleestadoBotonAnterior(){
  estadoBotonAnterior = !estadoBotonAnterior;
}