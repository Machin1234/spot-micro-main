#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>

#define PWM_SERVO_ADDR    0x40
#define L1 111
#define L2 118
#define SyzeBuffer 10

Adafruit_PWMServoDriver PWM = Adafruit_PWMServoDriver(PWM_SERVO_ADDR);
Adafruit_MPU6050 mpu;

typedef enum{
  AVG,AVD,ARG,ARD
}Placement;

typedef struct{
  int x;
  int y;
  int z;
}Coordinate;

typedef struct{
  int StepA1;
  int StepA2;
  int StepA3;
  float DegA1;
  float DegA2;
  float DegA3;
  int X,Y,Z;
  int Cx = 0;
  int Cy = 0;
  int Cz = 0;
  float CStepA1=0;
  float CStepA2=0;
  float CStepA3=0;
  Placement Place;
}Leg;


  Leg LegAVG;
  Leg LegAVD;
  Leg LegARG;
  Leg LegARD;

void setup() {
  Serial.begin(115200);
  Serial.println("debut setup");
  PWM.begin();
  PWM.setPWMFreq(50);
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);
  Serial.println("debut programme");
  LegARG.Place=ARG;
  LegARD.Place=ARD;
  LegAVG.Place=AVG;
  LegAVD.Place=AVD;

  /*LegARG.StepA1=290;
  LegARG.StepA2=335;
  LegARG.StepA3=200;*/
  LegARG.X=0;
  LegARG.Y=-150;
  LegARG.DegA1=65;
  LegARG.CStepA1=-20;
  LegARG.CStepA2=-20;
  LegARG.CStepA3=18;

  LegARD.StepA1=290;
  LegARD.StepA2=200;
  LegARD.StepA3=200;
  LegARD.CStepA1=20;
  LegARD.CStepA2=20;
  LegARD.CStepA3=24;

  LegAVG.StepA1=290;
  LegAVG.StepA2=200;
  LegAVG.StepA3=200;
  LegAVG.CStepA1=20;
  LegAVG.CStepA2=10;
  LegAVG.CStepA3=18;  

  LegAVD.StepA1=290;
  LegAVD.StepA2=200;
  LegAVD.StepA3=200;
  LegAVD.CStepA1=25;
  LegAVD.CStepA2=0;
  LegAVD.CStepA3=33;


  //WritePWM(LegARD);
  //WritePWM(LegAVG);
  //WritePWM(LegAVD);
  CoorToDeg(LegARG);
  DegToStep(LegARG);
  WritePWM(LegARG);
  Serial.println(LegARG.DegA2);
  Serial.println(LegARG.DegA3);
  Serial.println(LegARG.StepA2);
  Serial.println(LegARG.StepA3);
}

void loop() {
  /*LegAVG.X=0;
  LegAVG.Y=ReadInt();
  LegUpdate(LegAVG);
  Serial.println(LegAVG.DegA2);
  Serial.println(LegAVG.DegA3);
  //Serial.println(LegAVG.StepA2);
  //Serial.println(LegAVG.StepA3);
  delay(500);*/
}
//fonction accelerometre 
Coordinate GetAccRota (void){
  Coordinate Coor;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Coor.x = a.acceleration.x;
  Coor.y = a.acceleration.y;
  Coor.z = a.acceleration.z;
  return Coor;
}
Coordinate GetInc (void){
  Coordinate Coor;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Coor.x = a.acceleration.roll;
  Coor.y = a.acceleration.pitch;
  Coor.z = a.acceleration.heading;
  return Coor;
}
void LegUpdate(Leg &CLeg){
  CoorToDeg(CLeg);
  DegToStep(CLeg);
  WritePWM(CLeg);
}
void DegToStep(Leg &CLeg){
  CLeg.StepA1 = abs(CLeg.DegA1/1.19)+200;
  CLeg.StepA2 = abs(CLeg.DegA2/1.19)+200;
  CLeg.StepA3 = abs(CLeg.DegA3/1.19)+200;
}
void WritePWM(Leg &CLeg){
  switch(CLeg.Place){
    case AVG :
      PWM.setPWM(0, 0, CLeg.StepA1+CLeg.CStepA1);
      PWM.setPWM(1, 0, (580-CLeg.StepA2+CLeg.CStepA2));
      PWM.setPWM(2, 0, CLeg.StepA3+CLeg.CStepA3);
    break;

    case AVD :
      PWM.setPWM(3, 0, (580-CLeg.StepA1+CLeg.CStepA1));
      PWM.setPWM(4, 0, CLeg.StepA2+CLeg.CStepA2);
      PWM.setPWM(5, 0, (580-CLeg.StepA3+CLeg.CStepA3));
    break;

    case ARG:
      PWM.setPWM(9, 0, 580-CLeg.StepA1+CLeg.CStepA1);
      PWM.setPWM(10, 0, (580-CLeg.StepA2+CLeg.CStepA2));
      PWM.setPWM(11, 0, (CLeg.StepA3+CLeg.CStepA3));
    break;

    case ARD:
      PWM.setPWM(6, 0, (580-CLeg.StepA1+CLeg.CStepA1));
      PWM.setPWM(7, 0, CLeg.StepA2+CLeg.CStepA2);
      PWM.setPWM(8, 0, 580-CLeg.StepA3+CLeg.CStepA3);
    break;
    default :  Serial.println("default");
  }
}
void CoorToDeg(Leg &CLeg){
  if (CLeg.Place==AVG||CLeg.Place==ARG)
  {
  float y = CLeg.Y+CLeg.Cy;
  float x = CLeg.X+CLeg.Cx;
  float D = (y*y+x*x-L1*L1-L2*L2)/(2*L1*L2);
  float beta = -acos(D);
  float k1 = L1 + L2 * cos(beta);
  float k2 = L2 * sin(beta);
  float sigma = atan2(k1*y-k2*x,k2*y+k1*x);
  CLeg.DegA2 = ((beta/(M_PI/+180.0))+180);
  CLeg.DegA3 = ((sigma/(M_PI/-180.0))+40);
  }
  else {
  float y = CLeg.Y/*+CLeg.Cy*/;
  float x = CLeg.X/*+CLeg.Cx*/;
  float D = (y*y+x*x-L1*L1-L2*L2)/(2*L1*L2);
  float beta = acos(D);
  float k1 = L1 + L2 * cos(beta);
  float k2 = L2 * sin(beta);
  float sigma = atan2(k1*y-k2*x,k2*y+k1*x);
  CLeg.DegA2 = ((beta/(M_PI/180.0))+180);
  CLeg.DegA3 = ((sigma/(M_PI/-180.0))+40);
  }
}
int ReadInt(void){
  char buf[SyzeBuffer];
  if (Serial.available()) {
    Serial.readBytesUntil('\n',(char*)buf,SyzeBuffer);
    return atoi(buf);
  }
}
float ReadFloat(void){
  char buf[SyzeBuffer];
  if (Serial.available()) {
    Serial.readBytesUntil('\n',(char*)buf,SyzeBuffer);
    return atof(buf);
  }
}