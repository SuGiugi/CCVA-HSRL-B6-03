#include "MatrixMiniR4.h"
#include <algorithm> 

unsigned int camData[10];
float last_error = 0;
int color_id, x, y, last_color_id, cnt = 0, line_color = 10, step = 0;
bool final = false;

// Hằng số
const int Y_IGNOR = 70; // Vị trí tối thiểu để đọc khối theo trục Y (nếu xa hơn sẽ không đọc)
const int GREEN = 310; // Vị trí né khối theo trục x của màu xanh
const int RED = 5; // Vị trí né khối theo trục y của màu đỏ
const int DOOR = 12; // Điều kiện kết thúc vòng chạy



// Giới hạn góc xoay
float limit(float value, float min, float max) {
  if (value < min) value = min;
  if (value > max) value = max;
  return value;
}

/*
  Tính toán vị trí xoay
  value: góc muốn xoay (0 = đi thẳng)
  l: giới hạn góc xoay
*/
void servoMotor(float value, float l = 50) {
  float diff = 90 + limit(value, -l, l);
  MiniR4.RC1.setAngle(diff);
}

// dò đường trái
void doduong_laser_trai(float khoang_cach, float kp, float kd) {
  float error = khoang_cach - MiniR4.I2C1.MXLaserV2.getDistance() / 10;
  float D = last_error - error;
  float PID = error * kp + D * kd;
  servoMotor(PID, 40);
  last_error = error;
}

// dò đường phải
void doduong_laser_phai(float khoang_cach, float kp, float kd) {
  float error = MiniR4.I2C2.MXLaserV2.getDistance() / 10  - khoang_cach;
  float D = last_error - error;
  float PID = error * kp + D * kd;
  servoMotor(PID);
  last_error = error;
}

// Kiểm tra điều kiện kết thúc nếu: đi đủ khoảng cách + chạm đường xanh cam hoặc vòng đầu tiên + chạm đường xanh cam
void line_check() {
  line_color = MiniR4.I2C3.MXColorV3.getColorID();

  if ((MiniR4.M1.getDegrees() > 2000 and (line_color == 9 or line_color == 3)) or (cnt == 0 and (line_color == 9 or line_color == 3))) {
    cnt++;
    MiniR4.M1.resetCounter();
  }
}

// Khi đi tới vị trí góc sa bàn (ở giữa 2 đường xanh cam) robot sẽ xoay 1 góc rộng để quét các khối tiếp theo (nếu có)
bool turn() {
  bool check = false; // Kiểm tra xem đã đến góc sa bàn chưa
    if (line_color == 9 and cnt != DOOR) {
    last_error = 0; 
    check = true;
    MiniR4.LED.setColor(1, 255,255, 0);
      color_id = camData[0];
      y = camData[2];
      if (y < Y_IGNOR) color_id = 255;
      // Nếu không có khối nào tiếp theo
      if (color_id == 255) {
        MiniR4.LED.setColor(1, 255, 0, 255);
        unsigned int timeStart = millis();
        // Vừa đi vừa xoay 1 góc rộng ví dụ -15 -> 47 để quét xem liệu có màu để bám theo ko
        int turn_angle = 0;
        while (line_color != 3 and color_id == 255 and millis() - timeStart < 4000) {
          line_check();

          MiniR4.Vision.Data(camData);

          servoMotor(turn_angle);
          if (turn_angle <= 42) {
            turn_angle += 12;
          }

          color_id = camData[0];
          y = camData[2];
          if (y < Y_IGNOR) color_id = 255;
          if (color_id != 255) {
            MiniR4.LED.setColor(1, 255, 0, 255);
            break;
          } 

          MiniR4.M1.setSpeed(20);
          line_color = MiniR4.I2C3.MXColorV3.getColorID();
        }

        timeStart = millis();
        while (millis() - timeStart < 300 and color_id == 255) {
          MiniR4.Vision.Data(camData);
          // servoMotor(0);

          color_id = camData[0];
          y = camData[2];
          if (y < Y_IGNOR) color_id = 255;
          if (color_id != 255) break;

          MiniR4.M1.setSpeed(30);
        }
      }

      line_color = MiniR4.I2C3.MXColorV3.getColorID();
  }
  return check;
}

// Một khối chờ cho việc đi mù nếu gặp được khối nào đó thì sẽ bỏ qua và trực tiếp bám khối
bool dichuyen_cm(float quang_duong) {
  last_error = 0;
  int startDegrees = abs(MiniR4.M1.getDegrees());
  while(abs(MiniR4.M1.getDegrees() - startDegrees) *19.6 / 360 <= quang_duong) {
    if(turn()) {
      return false;
    }
  }
  return true;
}

void last_step(bool check = true) {
  if (final) {
    if (dichuyen_cm(last_color_id == 0 ? 13 : 13) && last_color_id == 1 && check) {
      servoMotor(-45);
      dichuyen_cm(11);
    }
    final = false;
    last_error = 0;
    last_color_id = 255;
  } 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  MiniR4.begin();
  MiniR4.M1.setReverse(false);
  MiniR4.RC1.begin();
  MiniR4.RC1.setHWDir(true);
  MiniR4.I2C1.MXLaserV2.begin();
  MiniR4.I2C2.MXLaserV2.begin();
  MiniR4.I2C3.MXColorV3.begin();
  MiniR4.Vision.Begin();
  MiniR4.Vision.reset();
  
  while (true) {
    if (MiniR4.BTN_DOWN.getState()) {
      while(1) {
        MiniR4.Vision.Data(camData);
      }
    } else if (MiniR4.BTN_UP.getState()) {
      break;
    }
  };
  MiniR4.Vision.Data(camData);
}

void loop() {

  MiniR4.Vision.Data(camData);
  color_id = camData[0];
  x = camData[1];
  y = camData[2];
  if (y < Y_IGNOR) color_id = 255;
  MiniR4.M1.setSpeed(25);
  
  line_color = MiniR4.I2C3.MXColorV3.getColorID();
  turn();


  if (color_id != 255 and y < 120) {
      last_step(false);
      MiniR4.LED.setColor(1, 0, 0, 255);
      servoMotor(int(x - 160) * 0.6, 30);
      MiniR4.M1.setSpeed(20);
      step = 1;
      final = false;
  } else if (color_id == 0 && step == 1) {
      // MiniR4.M1.setBrake(true);
      // while(1);

      MiniR4.LED.setColor(1, 0, 255, 0);
      MiniR4.M1.setSpeed(15);
      servoMotor(int(x - GREEN) * 0.5, 36);
      final = true;
      last_color_id = 0;
  } else if (color_id == 1 && step == 1) {
      MiniR4.LED.setColor(1, 255, 0, 0);
      MiniR4.M1.setSpeed(15);
      servoMotor(int(x - RED) * 0.6, 36);
      final = true;
      last_color_id = 1;
  } else if (color_id == 255 and line_color == 10) {
    last_step();
    MiniR4.LED.setColor(1, 0, 0, 0);
    MiniR4.M1.setSpeed(45);
    doduong_laser_trai(60, 1.4, 2);
    step = 0;
  } 
}
