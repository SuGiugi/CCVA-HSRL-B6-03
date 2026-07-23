#include "MatrixMiniR4.h"
#include <algorithm> 

unsigned int camData[10];
float last_error = 0;
int color_id, x, y, last_color_id, cnt = 0, line_color = 10, step = 0, cnt_degrees = 0;
bool final = false;
int green, red, start, end, direction, last_postion, b_timer = 0;

// Hằng số
const int Y_IGNOR = 70; // Vị trí tối thiểu để đọc khối theo trục Y (nếu xa hơn sẽ không đọc)
const int DOOR = 12; // Điều kiện kết thúc vòng chạy
const int d = 19.6;


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
  float D = error - last_error;
  float PID = error * kp + D * kd;
  servoMotor(PID, 45);
  last_error = error;
}

// dò đường phải
void doduong_laser_phai(float khoang_cach, float kp, float kd) {
  float error = MiniR4.I2C2.MXLaserV2.getDistance() / 10 - khoang_cach;
  float D = error - last_error;
  float PID = error * kp + D * kd;
  servoMotor(PID, 45);
  last_error = error;
}

// Kiểm tra điều kiện kết thúc nếu: đi đủ khoảng cách + chạm đường xanh cam hoặc vòng đầu tiên + chạm đường xanh cam
void line_check() {
  line_color = MiniR4.I2C3.MXColorV3.getColorID();

  if ((abs(cnt_degrees - MiniR4.M1.getDegrees()) * d / 360 > 100 and (line_color == 9 or line_color == 3)) or (cnt == 0 and (line_color == 9 or line_color == 3))) {
    cnt++;
    cnt_degrees = MiniR4.M1.getDegrees();
  }
}

// Khi đi tới vị trí góc sa bàn (ở giữa 2 đường xanh cam) robot sẽ xoay 1 góc rộng để quét các khối tiếp theo (nếu có)
bool turn() {
  bool check = false; // Kiểm tra xem đã đến góc sa bàn chưa
    if (line_color == start and cnt != DOOR) {
    
    last_error = 0; 
    check = true;
    MiniR4.LED.setColor(1, 255,255, 0);
    MiniR4.Vision.Data(camData);
      color_id = camData[0];
      y = camData[2];
      if (y < Y_IGNOR) color_id = 255;
      // Nếu không có khối nào tiếp theo
      if (color_id == 255) {
        int startDegrees = abs(MiniR4.M1.getDegrees());
        // Vừa đi vừa xoay 1 góc rộng ví dụ 0 -> 42 để quét xem liệu có màu để bám theo ko
        int turn_angle = 15;
        while (line_color != end and abs(MiniR4.M1.getDegrees() - startDegrees) * d / 360 <= 30) {
          
          line_check();

          // MiniR4.Vision.Data(camData);

          // color_id = camData[0];
          // y = camData[2];
          // if (y < Y_IGNOR) color_id = 255;
          // if (color_id != 255) {
          //   MiniR4.LED.setColor(1, 255, 0, 255);
          //     line_check();
          //     return check;
          // } 
          
          servoMotor(turn_angle * direction);
          if (turn_angle <= 45) {
            turn_angle += 15;
          }

          MiniR4.M1.setSpeed(25);

          line_color = MiniR4.I2C3.MXColorV3.getColorID();
          Serial.print(startDegrees);
          Serial.print(" ");
          Serial.print(MiniR4.M1.getDegrees());
          Serial.print(" ");
          Serial.println(abs(MiniR4.M1.getDegrees() - startDegrees) * d / 360);
        }
        if (color_id == 255) {
        int turn_back = min(abs(MiniR4.M1.getDegrees() - startDegrees) * d / 360 / 3, 10);
          startDegrees = abs(MiniR4.M1.getDegrees());
          while (abs(MiniR4.M1.getDegrees() - startDegrees) * d / 360 <= turn_back) {
            MiniR4.M1.setSpeed(-20);
            MiniR4.Vision.Data(camData);
            servoMotor(-40 * direction);

            // color_id = camData[0];
            // y = camData[2];
            // if (y < Y_IGNOR) color_id = 255;
            // if (color_id != 255) break;
          }
          MiniR4.M1.setSpeed(20);  
        }
      }

      line_color = MiniR4.I2C3.MXColorV3.getColorID();
  }
  line_check();
  return check;
}

// Một khối chờ cho việc đi mù nếu gặp được khối nào đó thì sẽ bỏ qua và trực tiếp bám khối
bool dichuyen_cm(float quang_duong) {
  last_error = 0;
  int startDegrees = abs(MiniR4.M1.getDegrees());
  while(abs(MiniR4.M1.getDegrees() - startDegrees) * d / 360 <= quang_duong) {
    if(turn()) {
      return false;
    }
  }
  return true;
}

void last_step(bool check = true) {
  if (final) {
    MiniR4.M1.setSpeed(30);
    if (dichuyen_cm(last_color_id == 0 ? 10 : 13) && check) {
      if (last_color_id == 1) {
        servoMotor(-45);
        dichuyen_cm(8);
      } else if (last_color_id == 0) {
        servoMotor(45);
        dichuyen_cm(5);
      }
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
}

void loop() {

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

  int p = 0;
  int s = 0;
  while (p < 50) {
    int r = MiniR4.I2C2.MXLaserV2.getDistance();
    int l = MiniR4.I2C1.MXLaserV2.getDistance();
    if (r > l) s++;
    else s--;
    p += 1;
  }

  direction = 0;
  if (s > 0) {
    direction = 1;
    green = 310;
    red = 5;
    start = 9;
    end = 3;
  } else {
    direction = -1;
    green = 10;
    red = 305;
    start = 3;
    end = 9;
  }

  last_error = 0;
  color_id = 255;
  x = 0;
  y = 0; 
  last_color_id = 255;
  cnt = 0;
  line_color = 10;
  step = 0;
  last_postion = 0;
  b_timer = 0;
  MiniR4.M1.resetCounter();
  cnt_degrees = abs(MiniR4.M1.getDegrees());
  final = false;

  servoMotor(50 * direction);
  MiniR4.M1.setSpeed(30);
  dichuyen_cm(6);
  servoMotor(-50 * direction);
  MiniR4.M1.setSpeed(-30);
  dichuyen_cm(4);
  MiniR4.M1.setSpeed(30);
  servoMotor(50 * direction);
  dichuyen_cm(12);
  servoMotor(-50 * direction);
  dichuyen_cm(4);
  
  while (true) {
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
        servoMotor(int(x - 160) * 0.7, 30);
        MiniR4.M1.setSpeed(20);
        step = 1;
        final = false;
    } else if (color_id == 0 && step == 1) {
        // MiniR4.M1.setBrake(true);
        // while(1);

        MiniR4.LED.setColor(1, 0, 255, 0);
        MiniR4.M1.setSpeed(15);
        servoMotor(int(x - green) * 0.5, 35);
        final = true;
        last_color_id = 0;
    } else if (color_id == 1 && step == 1) {
        MiniR4.LED.setColor(1, 255, 0, 0);
        MiniR4.M1.setSpeed(15);
        servoMotor(int(x - red) * 0.65, 38);
        final = true;
        last_color_id = 1;
    } else if (color_id == 255 and line_color == 10) {
      last_step();
      MiniR4.LED.setColor(1, 0, 0, 0);
      MiniR4.M1.setSpeed(50);
      (direction == 1 ? doduong_laser_trai(63, 0.6, 2) : doduong_laser_phai(60, 1.4, 2));
      step = 0;
    } 

    line_check();

    if (last_postion == MiniR4.M1.getDegrees()) {
      if (b_timer == 0) {
        b_timer = millis();
      }
      if (millis() - b_timer > 300) {
        b_timer = 0;
        servoMotor(0);
        MiniR4.M1.setSpeed(-100);
        dichuyen_cm(10);

        servoMotor(45 * direction);
        MiniR4.M1.setSpeed(100);
        dichuyen_cm(8);
        MiniR4.M1.setSpeed(25);
      }
    } else {
      b_timer = 0;
    }

    last_postion = MiniR4.M1.getDegrees();
    if (cnt == DOOR) {
      MiniR4.M1.setBrake(true);
      break;
    } 
  }
}
