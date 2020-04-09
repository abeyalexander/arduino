volatile float i = 0.24;
volatile float R = 12;
volatile float D = 0.95;
volatile float L = 0.1;
volatile float A= 0 ;
volatile float Ti=0;
volatile float Ta = 25;
volatile float delt = 97e-3;
volatile float h =95;
volatile float m = 0;
volatile float Cp = 477;
volatile float K ;
volatile float Tnext = 0;
volatile float d = 0.0005;
volatile float Pi = 3.14;
volatile float dens = 6500;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
A = Pi*d*L;
m = dens*Pi*sq(d)*1/4;

}

void loop() {
  // put your main code here, to run repeatedly:
Tnext = TempEstimate(D,delt);
Serial.println(Tnext);
}
float TempEstimate(float D,float delt)
{
  Tnext = Ti + ((i*i*R*D-h*A*(Ti-Ta))*delt)/(m*Cp);
  Ti=Tnext;
  return(Tnext);
}

