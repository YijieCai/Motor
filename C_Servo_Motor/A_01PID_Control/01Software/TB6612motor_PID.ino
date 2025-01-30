#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//OLED Parameter

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
// URL: https://github.com/YijieCai/To-do-List-OLED/edit/main/01Arduino/TDL.ino
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//Motor Parameter
#define PWMA 32
#define AIN1 25
#define AIN2 33
#define PWMB 12
#define STBY 26
#define E1A 34
#define E1B 35
#define Voltage 36 //使用模拟引脚

int PwmA, PwmB;
double V;
double A1_Sign;
double A2_Sign;
int Vel_PWM;

//PID Parameter
volatile long encoderValue = 0; // 编码器计数值
long targetPosition = 0; // 目标位置
float kp = 1.0, ki = 0, kd = 0.15; // PID系数

void setup() {
  // COM setup
  Serial.begin(115200);

  // OLED setup
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Motor");
  display.print(" Information");
  display.display();



  // Motor setup
  // 设置电机控制引脚为输出模式
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  // 设置编码器引脚为输入模式
  pinMode(Voltage,INPUT); //初始化作为输入端
  pinMode(E1A, INPUT);
  pinMode(E1B, INPUT);

  // 初始化编码器中断
  attachInterrupt(digitalPinToInterrupt(E1A), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E1B), encoderISR, CHANGE);

  // 让电机驱动器退出待机模式
  digitalWrite(STBY, HIGH);
}




void loop() {
  // 假设我们想让电机转动到一个特定的目标位置
  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n');
    targetPosition = inputString.toInt(); // 假设输入是整数形式的目标位置
    Serial.print("New Target Position: ");
    Serial.println(targetPosition);
  }

  
  // 实现PID控制
  float error = targetPosition - encoderValue;
  static float lastError = 0;
  static float integral = 0;

  integral += error;
  float derivative = error - lastError;
  lastError = error;

  float output = (kp * error) + (ki * integral) + (kd * derivative);

  // 根据PID输出调整PWM信号
  if(output > 0){
      analogWrite(PWMA, (int)min(abs(output), (float)100)); // 强制转换为int类型
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
  } else {
      analogWrite(PWMA, (int)min(abs(output), (float)100)); // 强制转换为int类型
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
  }

  // 打印当前状态到串口监视器
  Serial.print("Encoder:");
  Serial.print(encoderValue);
  Serial.print(" Target:");
  Serial.println(targetPosition);


  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println("Encoder: ");  //对模拟量转换并通过串口输出
  display.println(encoderValue);

  display.println("Target: ");  //对模拟量转换并通过串口输出
  display.println(targetPosition);
  
  display.display();

  
  delay(10); // 简单的时间延迟来模拟PID循环
}

void encoderISR() {
  // 霍尔编码器中断服务程序，用于更新编码器值
  static int lastState = digitalRead(E1A);
  int currentState = digitalRead(E1A);

  if(currentState != lastState){
    if(digitalRead(E1B) != currentState){
      encoderValue++;
    } else {
      encoderValue--;
    }
  }
  lastState = currentState;
}
