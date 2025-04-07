#include <Arduino.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>

// 定义蜂鸣器引脚
#define BUZZER_PIN 5
#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 音符频率定义（C大调）
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  393
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587  // 新增高音
#define NOTE_E5  659  // 新增高音
#define NOTE_F5  698  // 新增高音
#define NOTE_G5  784  // 新增高音
#define NOTE_A5  880  // 新增高音
#define NOTE_B5  988  // 新增高音
#define NOTE_C6  1047 // 新增高音
#define NOTE_REST 0  // 休止符标记（需特殊处理）

// 《小星星》乐谱（音符+时长）
int melody[] = {


  // 第一乐句：一闪一闪亮晶晶
  NOTE_C4, 200, NOTE_C4, 200,
  NOTE_G4, 200, NOTE_G4, 200,
  NOTE_A4, 200, NOTE_A4, 200,
  NOTE_G4, 400,

  // 第二乐句：满天都是小星星
  NOTE_F4, 200, NOTE_F4, 200,
  NOTE_E4, 200, NOTE_E4, 200,
  NOTE_D4, 200, NOTE_D4, 200,
  NOTE_C4, 400,

  // 第三乐句：挂在天空放光明
  NOTE_G4, 200, NOTE_G4, 200,
  NOTE_F4, 200, NOTE_F4, 200,
  NOTE_E4, 200, NOTE_E4, 200,
  NOTE_D4, 400,

  // 第四乐句：好像许多小眼睛
  NOTE_G4, 200, NOTE_G4, 200,
  NOTE_F4, 200, NOTE_F4, 200,
  NOTE_E4, 200, NOTE_E4, 200,
  NOTE_D4, 400,


  // 尾声（渐弱处理）
  NOTE_C5, 400, NOTE_REST, 200,
  NOTE_G4, 400, NOTE_REST, 200,
  NOTE_C4, 800
};

void setup() {
  Wire.begin(41,42);
  pinMode(BUZZER_PIN, OUTPUT);  // 设置引脚为输出模式
  display.begin(i2c_Address, true);
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Twinkle Twinkle Little Star");
   display.display();
}

void loop() {
  playMelody();
  delay(2000);  // 歌曲间隔
}

void playMelody() {
  for (int i = 0; i < sizeof(melody)/2; i += 2) {
    int note = melody[i];
    int duration = melody[i+1];  
    
    // 播放音符
    tone(BUZZER_PIN, note, duration);  // 启动PWM音调
    delay(duration);                    // 持续播放
    noTone(BUZZER_PIN);                // 停止PWM
    delay(100);                         // 音符间隔
  }
}
