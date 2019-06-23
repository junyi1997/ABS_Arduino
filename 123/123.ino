long previousTime = 0;          // 用來保存前一次 LED 更新狀態的時間
long interval = 1000;           // 燈號閃爍間隔時間，單位為毫秒(miliseconds)

void setup() {
  Serial.print('a');
  Serial.println(); 
}

void loop()

{

  unsigned long currentTime = millis();


  // 檢查是否已超過間隔時間

  // 是的話，就切換燈號並且記錄更新時間

  if(currentTime - previousTime > interval) { 
      Serial.print('1');
      Serial.println();  
    // 記錄更新時間
    previousTime = currentTime;       
  }
  // 你可以在這裏做其它事情

}
