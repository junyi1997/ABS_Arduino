#define INTERVAL_MESSAGE1 5000
#define INTERVAL_MESSAGE2 1000
unsigned long time_1 = 0;
unsigned long time_2 = 0;

void print_time(unsigned long time_millis);

void setup() {
    Serial.begin(9600);
}
 
void loop() {
  unsigned long currentTime2 = millis();
  unsigned long currentTime1 = millis();
  if ((currentTime1-time_1)==INTERVAL_MESSAGE2){
    Serial.println(currentTime1-time_1);
    time_1=currentTime1;
  }
  else if((currentTime2-time_2)>INTERVAL_MESSAGE1){
    Serial.println("Return");
    time_2=currentTime2;
  } 

}
 
void print_time(unsigned long time_millis){
    Serial.print("Time: ");
    Serial.print(time_millis/1000);
    Serial.print("s - ");
}
