
// I2Cdev和MPU6050必須作為庫安裝，否則必須安裝.cpp / .h文件
// 這兩個類都必須在項目的包含路徑中
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//＃include“ MPU6050.h” //如果使用MotionApps包含文件則不需要


//如果執行I2Cdev I2CDEV_ARDUINO_WIRE實現，則需要Arduino Wire庫
//在I2Cdev.h中使用
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

//類的默認I2C地址為0x68
//特定的I2C地址可以在此處作為參數傳遞
// AD0低= 0x68（SparkFun突破和InvenSense評估板的默認設置）
// AD0 high = 0x69
MPU6050 mpu1(0x68);
MPU6050 mpu2(0x69); // <-- use for AD0 high

/* =========================================================================
    注意：除了連接3.3v，GND，SDA和SCL外，此草圖
    取決於將MPU-6050的INT引腳連接到Arduino的
    外部中斷＃0引腳。 在Arduino Uno和Mega 2560上，這是
    數字I / O引腳2。
 * ========================================================================== */

/* =========================================================================
    注意：帶有Leonardo開發板的Arduino v1.0.1會產生編譯錯誤
    當使用Serial.write（buf，len）時。 茶壺輸出使用此方法。
    該解決方案需要修改Arduino USBAPI.h文件，該文件
    很簡單，但很煩人。 這將在下一個IDE中修復
    發布。 有關更多信息，請參見以下鏈接：

    http://arduino.cc/forum/index.php/topic,109987.0.html
    http://code.google.com/p/arduino/issues/detail?id=958
  * ========================================================================== */


#define OUTPUT_READABLE_YAWPITCHROLL

#define LED_PIN 13 //（Arduino是13，Teensy是11，Teensy ++是6）
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;   //如果DMP初始化成功，則設置為true
uint8_t mpuIntStatus1;   // holds actual interrupt status byte from MPU
uint8_t mpuIntStatus2;   //保存來自MPU的實際中斷狀態字節
uint8_t devStatus1;      //每次設備操作後返回狀態（0 =成功，！0 =錯誤）
uint8_t devStatus2;      //每次設備操作後返回狀態（0 =成功，！0 =錯誤）
uint16_t packetSize1;    //預期的DMP數據包大小（默認為42個字節）
uint16_t packetSize2;    //預期的DMP數據包大小（默認為42個字節）
uint16_t fifoCount1;     //當前FIFO中所有字節的計數
uint16_t fifoCount2;     //當前FIFO中所有字節的計數
uint8_t fifoBuffer1[64]; // FIFO存儲緩衝區
uint8_t fifoBuffer2[64]; // FIFO存儲緩衝區

// orientation/motion vars
Quaternion q1;           // [w, x, y, z]         四元數容器
Quaternion q2;           // [w, x, y, z]         四元數容器

VectorFloat gravity1;    // [x, y, z]            重力矢量
VectorFloat gravity2;    // [x, y, z]            重力矢量

float ypr1[3];           // [yaw, pitch, roll]   偏航/俯仰/滾動容器和重力矢量
float ypr2[3];           // [yaw, pitch, roll]   偏航/俯仰/滾動容器和重力矢量

// InvenSense茶壺演示的數據包結構




// ================================================================
// ===                       中斷檢測程序                         ===
// ================================================================

volatile bool mpuInterrupt = false;     //指示MPU中斷引腳是否已變為高電平
void dmpDataReady() {
    mpuInterrupt = true;
}



// ================================================================
// ===                         初始設置                          ===
// ================================================================

void setup() {
    //加入I2C總線（I2Cdev庫不會自動執行此操作）
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        // TWBR = 24; // 400kHz I2C時鐘（如果CPU為8MHz，則為200kHz）
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    //初始化串行通訊
    //（選擇了115200，因為它是Teapot Demo輸出所必需的，但是
    //取決於您的項目，具體取決於您）
    Serial.begin(115200);
    while (!Serial); //等待Leonardo枚舉，其他立即繼續

    //注意：8MHz或更慢的主機處理器，例如Teensy @ 3.3v或Ardunio
    // Pro Mini以3.3v運行，由於以下原因無法可靠地處理此波特率
    //波特計時與處理器的刻度太不一致。 您必須使用
    //在這種情況下為38400或更慢的速度，或者使用某種外部單獨的
    // UART定時器的晶振解決方案。

    //初始化設備
    Serial.println(F("Initializing I2C devices..."));
    mpu1.initialize();
    mpu2.initialize();
    
    //驗證連接
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu1.testConnection() ? F("MPU6050 1 connection successful") : F("MPU6050 1 connection failed"));
    Serial.println(mpu2.testConnection() ? F("MPU6050 2 connection successful") : F("MPU6050 2 connection failed"));

    //等待準備
    //Serial.println(F("\n發送任何字符以開始DMP編程和演示：“））;
    //同時（Serial.available（）&& Serial.read（））; //空緩衝區
    // while（！Serial.available（））; //等待數據
    //同時（Serial.available（）&& Serial.read（））; //再次清空緩衝區

    //加載並配置DMP
    Serial.println(F("Initializing DMP..."));
    devStatus1 = mpu1.dmpInitialize();
    devStatus1 = mpu2.dmpInitialize();


    //在此處提供您自己的陀螺儀偏移量，以最小靈敏度進行縮放
    mpu1.setXGyroOffset(220);
    mpu1.setYGyroOffset(76);
    mpu1.setZGyroOffset(-85);
    mpu1.setZAccelOffset(1788); //我的測試芯片的出廠默認值為1688

    mpu2.setXGyroOffset(220);
    mpu2.setYGyroOffset(76);
    mpu2.setZGyroOffset(-85);
    mpu2.setZAccelOffset(1788); //我的測試芯片的出廠默認值為1688


    //確保它正常工作（如果是，則返回0）
    if (devStatus1 == 0) {
        //打開DMP，現在已經準備好
        Serial.println(F("正在啟用DMP ..."));
        mpu1.setDMPEnabled(true);
        mpu2.setDMPEnabled(true);

        //啟用Arduino中斷檢測
        Serial.println(F("啟用中斷檢測（Arduino外部中斷0）..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus1 = mpu1.getIntStatus();
        mpuIntStatus2 = mpu2.getIntStatus();


        //設置DMP Ready標誌，以便主loop（）函數知道可以使用它
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        //獲得預期的DMP數據包大小，以便以後比較
        packetSize1 = mpu1.dmpGetFIFOPacketSize();
        packetSize2 = mpu2.dmpGetFIFOPacketSize();
    } else {
    //錯誤！
    // 1 =初始內存加載失敗
    // 2 = DMP配置更新失敗
    //（如果要中斷，通常代碼將為1）
        Serial.print(F("DMP初始化失敗（代碼"));
        Serial.print(devStatus1);
        Serial.println(F(")"));
    }

    //配置輸出LED
    pinMode(LED_PIN, OUTPUT);
}



// ================================================================
// ===                        主程序循環                         ===
// ================================================================

void loop() {
    //如果編程失敗，請不要嘗試做任何事情
    if (!dmpReady) return;

  
    //重置中斷標誌並獲取INT_STATUS字節
    mpuInterrupt = false;
    mpuIntStatus1 = mpu1.getIntStatus();
    mpuIntStatus2 = mpu2.getIntStatus();

    //獲取當前的FIFO計數
    fifoCount1 = mpu1.getFIFOCount();
    fifoCount2 = mpu2.getFIFOCount();

    //檢查是否有溢出（除非我們的代碼效率太低，否則永遠不會發生）
    
    
    if ((mpuIntStatus1 & 0x10) || fifoCount1 == 1024) {
        //重置，以便我們可以乾淨地繼續
        mpu1.resetFIFO();
        mpu2.resetFIFO();
        //Serial.println(F("FIFO溢出！"））;

    //否則，請檢查DMP數據就緒中斷（此情況應經常發生）
    } else if (mpuIntStatus1 & 0x02) {
        //等待正確的可用數據長度，應非常短的等待
        while (fifoCount1 < packetSize1) fifoCount1 = mpu1.getFIFOCount();

        //從FIFO讀取數據包
        mpu1.getFIFOBytes(fifoBuffer1, packetSize1);
        mpu2.getFIFOBytes(fifoBuffer2, packetSize2);

        //如果有> 1個數據包可用，在此跟踪FIFO計數
        //這使我們無需等待中斷即可立即閱讀更多內容
        fifoCount1 -= packetSize1;
        fifoCount2 -= packetSize2;



        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            //以度為單位顯示歐拉角
            mpu1.dmpGetQuaternion(&q1, fifoBuffer1);
            mpu1.dmpGetGravity(&gravity1, &q1);
            mpu1.dmpGetYawPitchRoll(ypr1, &q1, &gravity1);
            
            long time=millis();       
            Serial.print(time); Serial.print("\t");

            
            Serial.print("ypr1\t");
            Serial.print(ypr1[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr1[1] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr1[2] * 180/M_PI);
            Serial.print("\t");
            mpu2.dmpGetQuaternion(&q2, fifoBuffer2);
            mpu2.dmpGetGravity(&gravity2, &q2);
            mpu2.dmpGetYawPitchRoll(ypr2, &q2, &gravity2);
            Serial.print("ypr2\t");
            Serial.print(ypr2[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr2[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr2[2] * 180/M_PI);
            //delay(500);
            
        #endif

 
        //閃爍的LED指示活動
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
}
