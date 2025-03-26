#include <Wire.h>
#include <U8g2lib.h>

// 使用 I2C 接口
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// 眼睛参数
const int eye1_x = 40, eye2_x = 88, eye_y = 32; // 眼睛中心坐标
const int eye_width = 15, eye_height = 25;      // 眼睛大小
int pupil_offset_x = 0, pupil_offset_y = 0;    // 瞳孔位置偏移

void setup() {
  Wire.begin(41,42);
  u8g2.begin();
  randomSeed(analogRead(0)); // 随机种子
}

void loop() {
  for (int i = 0; i < 10; i++) { // 让瞳孔随机移动
    pupil_offset_x = random(-5, 6);
    pupil_offset_y = random(-5, 6);
    drawEyes(true);
    delay(300);
  }
  
  drawEyes(false); // 眨眼
  delay(300);
  drawEyes(true); // 重新睁开眼睛
  delay(2000);
}

// 画眼睛（open = true 代表睁眼，false 代表闭眼）
void drawEyes(bool open) {
  u8g2.clearBuffer();
  
  if (open) {
    // 画眼睛轮廓
    u8g2.drawEllipse(eye1_x, eye_y, eye_width, eye_height, U8G2_DRAW_ALL);
    u8g2.drawEllipse(eye2_x, eye_y, eye_width, eye_height, U8G2_DRAW_ALL);
    
    // 画瞳孔（填充圆形）
    u8g2.drawDisc(eye1_x + pupil_offset_x, eye_y + pupil_offset_y, 5);
    u8g2.drawDisc(eye2_x + pupil_offset_x, eye_y + pupil_offset_y, 5);
  } else {
    // 画闭眼效果（两条水平线）
    u8g2.drawLine(eye1_x - eye_width, eye_y, eye1_x + eye_width, eye_y);
    u8g2.drawLine(eye2_x - eye_width, eye_y, eye2_x + eye_width, eye_y);
  }
  
  u8g2.sendBuffer();
}
