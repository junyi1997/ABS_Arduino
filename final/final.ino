#define INTERVAL_MESSAGE1 30000
#define INTERVAL_MESSAGE2 300
unsigned long time_1 = 0;
unsigned long time_2 = 0;
bool _ABVAR_1_BTS= false ;
bool _ABVAR_2_BTL= false ;
int _ABVAR_3_count = 0 ;
int A[100];
int a=0;
int i=0;
const int motorIn1=5;//定義馬達接腳1為5
const int motorIn2=6;//定義馬達接腳2為6
int Btn_ABS_ONOFF=7; //定義ABS開啟按鈕為7
int Btn_Stop=8;      //定義煞車按鈕為8
void setup() {
  pinMode( 2 , INPUT);
  pinMode( Btn_ABS_ONOFF , INPUT);
  pinMode( Btn_Stop , INPUT);
  Serial.begin(9600);
  pinMode(motorIn1,OUTPUT);
  pinMode(motorIn2,OUTPUT);
  analogWrite(motorIn2,0);
}
 
void loop() {
  unsigned long currentTime2 = millis();
  unsigned long currentTime1 = millis();
  if ((currentTime1-time_1)==INTERVAL_MESSAGE2){
    count_sort();
    time_1=currentTime1;
  }else if((currentTime2-time_2)>INTERVAL_MESSAGE1){
    count_Total();
    time_2=currentTime2;
  }else{
    count_step();
    analogWrite(motorIn1,0);
    //Serial.println(digitalRead(Btn_ABS_ONOFF));
    if(digitalRead(Btn_Stop)==1){analogWrite(motorIn1,255);}
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
  Serial.print(i);
  Serial.print(" ");
  Serial.println(_ABVAR_3_count);
  i++;
  if(_ABVAR_3_count>15){
    if(digitalRead(Btn_ABS_ONOFF)==1){
    step_stop();
    }} 
  _ABVAR_3_count=0;
  }

void count_Total(){
  Serial.print("A[");
  Serial.print(a);
  Serial.print("]");
  Serial.print(A[a]);
  a++;
  Serial.println();
  } 

void step_stop(){
  analogWrite(motorIn1,255);
  delay(90);  
  analogWrite(motorIn1,0);
  delay(3);
  }     
