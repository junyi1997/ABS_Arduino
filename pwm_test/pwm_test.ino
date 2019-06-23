bool _ABVAR_1_BTS= false ;
bool _ABVAR_2_BTL= false ;
int _ABVAR_3_count = 0 ;
long previousTime = 0;          // 用來保存前一次 LED 更新狀態的時間
long interval = 1500;           // 燈號閃爍間隔時間，單位為毫秒(miliseconds)
int A[300];
int a=0;
void setup()
{
  pinMode( 2 , INPUT);
  Serial.begin(9600);
}

void loop()
{
  unsigned long currentTime = millis();

  _ABVAR_1_BTS = digitalRead(2) ;
  if (( ( _ABVAR_1_BTS ) != ( _ABVAR_2_BTL ) ))
  {
    if (( ( _ABVAR_1_BTS ) == ( HIGH ) ))
    {
      _ABVAR_3_count = ( _ABVAR_3_count + 1 ) ;
    }
  }
  _ABVAR_2_BTL = _ABVAR_1_BTS ;
 
  
  if(currentTime - previousTime > interval) {
    
    A[a]=_ABVAR_3_count;
    Serial.print("A[");
    Serial.print(a);
    Serial.print("}");
    
    Serial.print(A[a]);
    a++;
    Serial.println();
     _ABVAR_3_count=0;
    previousTime = currentTime; 
    }
}
