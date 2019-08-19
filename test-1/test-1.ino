#define INTERVAL_MESSAGE1 30000
#define INTERVAL_MESSAGE2 500
unsigned long time_1 = 0;
unsigned long time_2 = 0;
bool _ABVAR_1_BTS= false ;
bool _ABVAR_2_BTL= false ;
int _ABVAR_3_count = 0 ;
int A[300];
int a=0;
const int motorIn1=5;
const int motorIn2=6;
void setup() {
  pinMode( 2 , INPUT);
  Serial.begin(9600);
  pinMode(motorIn1,OUTPUT);
  pinMode(motorIn2,OUTPUT);
  analogWrite(motorIn2,0);
}
 
void loop() {
  count_step();
  
  unsigned long currentTime2 = millis();
  unsigned long currentTime1 = millis();
  if ((currentTime1-time_1)==INTERVAL_MESSAGE2){
    count_sort();
    time_1=currentTime1;
  }
  else if((currentTime2-time_2)>INTERVAL_MESSAGE1){
    count_Total();
    time_2=currentTime2;
  } 
}
void count_step(){
  _ABVAR_1_BTS = digitalRead(2) ;
  if (( ( _ABVAR_1_BTS ) != ( _ABVAR_2_BTL ) ))
  {
    if (( ( _ABVAR_1_BTS ) == ( HIGH ) ))
    {
      _ABVAR_3_count = ( _ABVAR_3_count + 1 ) ;
    }
  }
  _ABVAR_2_BTL = _ABVAR_1_BTS ;
  }

void count_sort(){
  A[a]+=_ABVAR_3_count;
  Serial.println(_ABVAR_3_count);
  if(_ABVAR_3_count>20){
    step_stop();
    } 
  _ABVAR_3_count=0;
  }

void count_Total(){
  Serial.print("A[");
  Serial.print(a);
  Serial.print("}");
  Serial.print(A[a]);
  a++;
  Serial.println();
  } 

void step_stop(){
  analogWrite(motorIn1,255);
  delay(170);  
  analogWrite(motorIn1,0);
  delay(3);
  }     
