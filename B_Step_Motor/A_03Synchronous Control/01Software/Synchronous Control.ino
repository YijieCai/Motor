// 定义连接到28BYJ-48步进电机驱动板的GPIO引脚
#define IN1 13
#define IN2 12
#define IN3 14
#define IN4 27

// 步进序列，用于驱动28BYJ-48

int step_Length = 8;
int steps[8] = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001};  //8拍信号分配 5.625°一步 64步=45°


// 延迟时间，控制转动速度
int delayTime = 25 // 可以根据需要调整,最快的频率与拍数有关，实测结果为：最短延迟时间=8ms/拍数。因此旋转360°至少消耗4096ms

void setup() {
  // 设置引脚模式为输出
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // 让电机正转一圈
  for (int i = 0; i < 512; i++) { // 28BYJ-48大约需要64*8步转一圈 (每64步是45°，因此8*64=512步才能转360°)
    for (int step = 0; step < step_Length; step++) {  //步进电机需要给若干次指定顺序的信号才能旋转，信号次数等于拍数
      setStep(steps[step]); //给定信号
      delay(delayTime); //等待时间，最短延迟时间=8ms/拍数
    }
  }

//  delay(1000); // 等待5秒

  // 让电机反转一圈
  for (int i = 0; i < 512; i++) {
    for (int step = step_Length-1 ; step >= 0; step--) {
      setStep(steps[step]);
      delay(delayTime);
    }
  }
}

// 设置步进电机的状态
void setStep(int step) {
  digitalWrite(IN1, (step & 0b0001) > 0); //通过按位与的方式可以很巧妙的处理需要输出的信号
  digitalWrite(IN2, (step & 0b0010) > 0);
  digitalWrite(IN3, (step & 0b0100) > 0);
  digitalWrite(IN4, (step & 0b1000) > 0);
}
