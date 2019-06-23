const int motorIn1=5;
const int motorIn2=6;

void setup()
{
  pinMode(motorIn1,OUTPUT);
  pinMode(motorIn2,OUTPUT);
}
void loop()
{
  for(int i=196; i<256 ; i+=60)
  {
    analogWrite(motorIn1,i);
    analogWrite(motorIn2,0);
    delay(140);  
  }
analogWrite(motorIn1,0);
analogWrite(motorIn2,0);
delay(30);
}
