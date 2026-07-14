#include "MatrixMiniR4.h"
#include <algorithm> 

unsigned int camData[10];
unsigned int redData[10];
unsigned int greenData[10];
int camStatus;
double last_error = 0;
double I = 0;
unsigned int mode = 0;
unsigned int change_count[3] = {0, 0, 0};




int current_frame = -1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  MiniR4.begin();
  MiniR4.M1.setReverse(false);
  // MiniR4.DriveDC.begin(1, 2, false, true);
  MiniR4.RC1.begin();
  MiniR4.RC1.setHWDir(false);
  MiniR4.I2C1.MXLaserV2.begin();
  MiniR4.I2C2.MXLaserV2.begin();

  MiniR4.OLED.clearDisplay(); 
  MiniR4.OLED.setTextSize(1); 
  MiniR4.OLED.setTextColor(SSD1306_WHITE); 
  MiniR4.Vision.Begin();
}

void doduong_laser_trai(double khoang_cach, double kp, double ki, double kd, double tocdo) {
  MiniR4.M1.setSpeed(tocdo);
  double error = khoang_cach - MiniR4.I2C1.MXLaserV2.getDistance() / 10;
  double D = last_error - error;
  I = I + error;
  double PID = error * kp + I * ki + D * kd;
  if (abs(PID) > 53) PID = 53 * (PID / abs(PID));
  MiniR4.RC1.setAngle(90 - PID); 
  last_error = error;
}

void doduong_laser_phai(double khoang_cach, double kp, double ki, double kd) {
  double error = MiniR4.I2C2.MXLaserV2.getDistance() / 10  - khoang_cach;
  double D = last_error - error;
  I = I + error;
  double PID = error * kp + I * ki + D * kd;
  if (abs(PID) > 53) PID = 53 * (PID / abs(PID));
  MiniR4.RC1.setAngle(90 - PID); 
  last_error = error;
}

void doduong_laser_giua(double kp, double ki, double kd) {
  double error = MiniR4.I2C2.MXLaserV2.getDistance() / 10  - MiniR4.I2C1.MXLaserV2.getDistance() / 10;
  double D = last_error - error;
  I = I + error;
  double PID = error * kp + I * ki + D * kd;
  if (abs(PID) > 53) PID = 53 * (PID / abs(PID));
  MiniR4.RC1.setAngle(90 - PID); 
  last_error = error;
}

void tracking() {
  if (MiniR4.Vision.SmartCamReader(camData, 50) > 0) {
    Serial.println(camData[2]);
    if (camData[0] == 1 && camData[2] > 140) {
      change_mode(1);
    }
  }
  Serial.print("Red: ");
  Serial.println(redData[1]);
  Serial.print("Green: ");
  Serial.println(greenData[1]);
}

void change_mode(int new_mode) {
  if (mode != new_mode) {
    last_error = 0;
    I = 0;
    mode = new_mode;
  }
}

void loop() {
  //   MiniR4.RC1.setAngle(0); 
  // // put your main code here, to run repeatedly:
  while(1) {
      if (MiniR4.BTN_UP.getState()) {
        break;
      }
  }
  while (1) {
    tracking();
    switch (mode) {
    case 1:
      MiniR4.LED.setColor(1, 0, 50, 0);
      MiniR4.M1.resetCounter();
      while (MiniR4.M1.getDegrees() * 19.6 / 360 < 30) {
        doduong_laser_trai(30, 1.8, 0, 8, 60);
      }
      change_mode(0);
      break;
    case 2:
      MiniR4.LED.setColor(1, 50, 0, 0);
      break;     
    default: 
      doduong_laser_trai(50, 1.2, 0, 3, 40);
      MiniR4.LED.setColor(1, 0, 0, 50);
      break;     
    }
  }
  MiniR4.M1.setBrake(true);
}
