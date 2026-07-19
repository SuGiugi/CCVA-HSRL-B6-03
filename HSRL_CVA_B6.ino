#include "MatrixMiniR4.h"
#include <algorithm> 

unsigned int camData[10];
unsigned int redData[10];
unsigned int greenData[10];
int camStatus;
int startDegrees;
float last_error = 0;
int color_id, x, y, last_color_id, cnt = 0, line_color = 10, step = 0;
bool final = false;

const int Y_IGNOR = 100;
const int GREEN = 310;
const int RED = 5;
const int DOOR = 12;



float limit(float value, float min, float max) {
  if (value < min) value = min;
  if (value > max) value = max;
  return value;
}

void servoMotor(float value, float l = 53) {
  float diff = 90 + limit(value, -l, l);
  MiniR4.RC1.setAngle(diff);
}

void doduong_laser_trai(float khoang_cach, float kp, float kd) {
  float error = khoang_cach - MiniR4.I2C1.MXLaserV2.getDistance() / 10;
  float D = last_error - error;
  float PID = error * kp + D * kd;
  servoMotor(PID);
  last_error = error;
}

void doduong_laser_phai(float khoang_cach, float kp, float kd) {
  float error = MiniR4.I2C2.MXLaserV2.getDistance() / 10  - khoang_cach;
  float D = last_error - error;
  float PID = error * kp + D * kd;
  servoMotor(PID);
  last_error = error;
}

void line_check() {
  line_color = MiniR4.I2C3.MXColorV3.getColorID();

  if ((MiniR4.M1.getDegrees() > 2000 and (line_color == 9 or line_color == 3)) or (cnt == 0 and (line_color == 9 or line_color == 3))) {
    cnt++;
    MiniR4.M1.resetCounter();
  }
}

bool turn() {
  bool check = false;
    if (line_color == 9 and cnt != DOOR) {
      check = true;
    MiniR4.LED.setColor(1, 255,255, 0);
      color_id = camData[0];
      y = camData[2];
      if (y < Y_IGNOR) color_id = 255;

      if (color_id == 255) {
        MiniR4.LED.setColor(1, 255, 0, 255);
        unsigned int timeStart = millis();
        int turn_angle = -45;
        while (line_color != 3 and color_id == 255 and millis() - timeStart < 4000) {
          line_check();

          MiniR4.Vision.Data(camData);

          servoMotor(turn_angle);
          if (turn_angle <= 50) {
            turn_angle += 5;
          }

          color_id = camData[0];
          y = camData[2];
          if (y < Y_IGNOR) color_id = 255;
          if (color_id != 255) {
            MiniR4.LED.setColor(1, 255, 0, 255);
            break;
          } 

          MiniR4.M1.setSpeed(15);
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
  while (!MiniR4.BTN_UP.getState());
  MiniR4.Vision.Data(camData);
}

bool dichuyen_cm(float quang_duong) {
  int startDegrees = abs(MiniR4.M1.getDegrees());
  while(abs(MiniR4.M1.getDegrees() - startDegrees) *19.6 / 360 <= quang_duong) {
    if(turn()) {
      return false;
    }
  }
  return true;
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


  if (camData[0] != 255 and y < 120) {
      MiniR4.LED.setColor(1, 0, 0, 255);
      servoMotor(int(camData[1] - 160) * 0.6, 30);
      MiniR4.M1.setSpeed(30);
      step = 1;
  } else if (camData[0] == 0 && step == 1) {
      // MiniR4.M1.setBrake(true);
      // while(1);

      MiniR4.LED.setColor(1, 0, 255, 0);
      MiniR4.M1.setSpeed(20);
      servoMotor(int(camData[1] - GREEN) * 0.5, 36);
      final = true;
      last_color_id = 0;
  } else if (camData[0] == 1 && step == 1) {
      MiniR4.LED.setColor(1, 255, 0, 0);
      MiniR4.M1.setSpeed(20);
      servoMotor(int(camData[1] - RED) * 0.6, 36);
      final = true;
      last_color_id = 1;
  } else if (camData[0] == 255 and line_color == 10) {
    if (final) {

      // MiniR4.M1.setBrake(true);
      // while(1);
      if (dichuyen_cm(last_color_id == 0 ? 15 : 15) && last_color_id == 1) {
        servoMotor(-50);
        dichuyen_cm(10);
      }
      final = false;
      last_error = 0;
    } // xet trong mau trang
    MiniR4.LED.setColor(1, 0, 0, 0);
    MiniR4.M1.setSpeed(40);
    doduong_laser_trai(60, 1.2, 1);
    step = 0;
  } 
}
