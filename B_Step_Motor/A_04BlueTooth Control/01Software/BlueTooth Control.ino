#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// 定义连接到28BYJ-48步进电机驱动板的GPIO引脚
#define IN1 13
#define IN2 12
#define IN3 14
#define IN4 27

// 步进序列，用于驱动28BYJ-48
int steps[8] = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001};

bool running = true; // 控制电机是否运行
bool forward = true;  // 控制旋转方向
int delayTime = 50;   // 初始延迟时间

void setup() {
  // 设置引脚模式为输出
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // 初始化串行通信
  Serial.begin(9600);
  
  // 初始化蓝牙
  SerialBT.begin("ESP32_BT"); // 蓝牙设备名称
  Serial.println("The device started, now you can pair it!");
}

void loop() {
  if (SerialBT.available()) {
    char command = SerialBT.read();
    handleCommand(command);
  }

  if(running) {
    if(forward) {
      stepForwardSingleStep();
    } else {
      stepBackwardSingleStep();
    }
  } else {
    setStep(0); // 停止电机
  }
}

void handleCommand(int command) {
  switch(command) {
    case '0':
      running = false;
      SerialBT.println("Motor stopped.");
      break;
    case '1':
      running = true;
      forward = true;
      SerialBT.println("Motor started in forward direction.");
      break;
    case '2':
      forward = !forward;
      if(forward){
        SerialBT.println("Direction changed to forward.");
      } else {
        SerialBT.println("Direction changed to backward.");
      }
      break;
    case '3':
      SerialBT.println("Please enter the number of steps:");
      if(SerialBT.available()) {
        int stepsToMove = SerialBT.parseInt();
        moveSteps(stepsToMove);
      }
      break;
    case '4':
      if(delayTime > 4) delayTime--;
      SerialBT.print("Speed increased. Current delay time: ");
      SerialBT.println(delayTime);
      break;
    case '5':
      delayTime++;
      SerialBT.print("Speed decreased. Current delay time: ");
      SerialBT.println(delayTime);
      break;
  }
}

void moveSteps(int stepsToMove) {
  for (int i = 0; i < stepsToMove; i++) {
    if(forward) {
      stepForwardSingleStep();
    } else {
      stepBackwardSingleStep();
    }
  }
  running = false; // 移动指定步数后停止电机
}

void stepForwardSingleStep() {
  for (int step = 0; step < 8; step++) {
    setStep(steps[step]);
    delay(delayTime);
  }
}

void stepBackwardSingleStep() {
  for (int step = 7; step >= 0; step--) {
    setStep(steps[step]);
    delay(delayTime);
  }
}

void setStep(int step) {
  digitalWrite(IN1, (step & 0b0001) > 0);
  digitalWrite(IN2, (step & 0b0010) > 0);
  digitalWrite(IN3, (step & 0b0100) > 0);
  digitalWrite(IN4, (step & 0b1000) > 0);
}
