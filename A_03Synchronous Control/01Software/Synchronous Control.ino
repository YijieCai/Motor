// 定义连接到28BYJ-48步进电机驱动板的GPIO引脚
#define IN1 13
#define IN2 12

#define IN1_ 14
#define IN2_ 27

// 步进序列，用于驱动28BYJ-48

int step_Length = 2;
int steps[2] = {0b0100,0b0001};  //2拍信号分配22.5°一步 16步=45°


// 延迟时间，控制转动速度
int delayTime = 50;

void setup() {
  // 设置引脚模式为输出
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN1_, OUTPUT);
  pinMode(IN2_, OUTPUT);
}

void loop() {
  // 让电机正转一圈
  while (1){

    digitalWrite(IN1, 1); 
    digitalWrite(IN2, 0);
    digitalWrite(IN1_, 1);
    digitalWrite(IN2_, 0);
    delay(delayTime);

    digitalWrite(IN1, 0); 
    digitalWrite(IN2, 1);
    digitalWrite(IN1_, 0);
    digitalWrite(IN2_, 1);
    delay(delayTime);
  }
}
