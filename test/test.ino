bool _ABVAR_1_BTS= false ;
bool _ABVAR_2_BTL= false ;
int _ABVAR_3_count = 0 ;
long previousTime1 = 0;          // 用來保存前一次 LED 更新狀態的時間
long previousTime2 = 0;
int A[300];
int a=0,b=0;
const int motorIn1=5;
const int motorIn2=6;
void setup()
{
  pinMode( 2 , INPUT);
  Serial.begin(9600);
  pinMode(motorIn1,OUTPUT);
  pinMode(motorIn2,OUTPUT);
}

void loop()
{
  unsigned long currentTime1 = millis();
  unsigned long currentTime2 = millis();
  _ABVAR_1_BTS = digitalRead(2) ;
  if (( ( _ABVAR_1_BTS ) != ( _ABVAR_2_BTL ) ))
  {
    if (( ( _ABVAR_1_BTS ) == ( HIGH ) ))
    {
      _ABVAR_3_count = ( _ABVAR_3_count + 1 ) ;
    }
  }
  _ABVAR_2_BTL = _ABVAR_1_BTS ;
  
  if(currentTime1 - previousTime1>997) {
    b++;
    Serial.print(b);Serial.print(":");
    Serial.println(_ABVAR_3_count);
    A[a]+=_ABVAR_3_count;
    _ABVAR_3_count=0;
    previousTime1 = currentTime1; 
    }
  else if(currentTime2 - previousTime2 > 30000) {
    Serial.print("A[");Serial.print(a);Serial.print("]：");
    Serial.print(A[a]);
    a++;
    Serial.println();
    previousTime2 = currentTime2; 
    }
      
  //if(_ABVAR_3_count>47){
    //for(int i=185; i<190 ; i+=15)
    //{
     // analogWrite(motorIn1,i);
      //analogWrite(motorIn2,0);
      //delay(150);  
    //}
  //analogWrite(motorIn1,0);
  //analogWrite(motorIn2,0);
  //delay(30);
  //}

}
