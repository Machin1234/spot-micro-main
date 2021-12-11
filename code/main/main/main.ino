#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>

#define PWM_SERVO_ADDR    0x40
#define L1 111
#define L2 118

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
  //int Cx = 0;
  //int Cy = 0;
  //int Cz = 0;
  float CDegA1=0;
  float CDegA2=0;
  float CDegA3=0;
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
  LegARD.Place=ARD;
  LegARG.X=-175;
  LegARG.Y=-150;
  /*DegToStep(LegARG);
  WritePWM(LegARG);*/
  LegUpdate(LegARG);
  LegARG.Y=-125;
Serial.println(LegARG.Y);
LegUpdate(LegARG);
LegARG.Y=-100;
Serial.println(LegARG.Y);
LegUpdate(LegARG);
LegARG.Y=-75;
Serial.println(LegARG.Y);
LegUpdate(LegARG);
LegARG.Y=-50;
Serial.println(LegARG.Y);
LegUpdate(LegARG);
LegARG.Y=-25;
Serial.println(LegARG.Y);
LegUpdate(LegARG);
  LegARG.Y=25;
Serial.println(LegARG.Y);
LegUpdate(LegARG);
LegARG.Y=50;
Serial.println(LegARG.Y);
LegUpdate(LegARG);
LegARG.Y=75;
Serial.println(LegARG.Y);
LegUpdate(LegARG);
LegARG.Y=100;
Serial.println(LegARG.Y);
LegUpdate(LegARG);
LegARG.Y=125;
Serial.println(LegARG.Y);
LegUpdate(LegARG);
}

void loop() {

}

//fonction accelerometre 
Coordinate GetAccRota (void)
{
  Coordinate Coor;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Coor.x = a.acceleration.x;
  Coor.y = a.acceleration.y;
  Coor.z = a.acceleration.z;
  return Coor;
}

Coordinate GetInc (void)
{
  Coordinate Coor;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Coor.x = a.acceleration.roll;
  Coor.y = a.acceleration.pitch;
  Coor.z = a.acceleration.heading;
  return Coor;
}

void LegUpdate(Leg &CLeg)
{
  CoorToDeg(CLeg);
  DegToStep(CLeg);
  WritePWM(CLeg);
}


void DegToStep(Leg &CLeg)
{
  CLeg.StepA1 = CLeg.DegA1*2+140;
  CLeg.StepA2 = CLeg.DegA2+200;
  CLeg.StepA3 = CLeg.DegA3+200;
}


void WritePWM(Leg &CLeg){
  switch(CLeg.Place){
    case AVG :
      PWM.setPWM(0, 0, CLeg.StepA1);
      PWM.setPWM(1, 0, CLeg.StepA2);
      PWM.setPWM(2, 0, CLeg.StepA3);
    break;

    case AVD :
      PWM.setPWM(3, 0, CLeg.StepA1);
      PWM.setPWM(4, 0, CLeg.StepA2);
      PWM.setPWM(5, 0, CLeg.StepA3);
    break;

    case ARG:
      PWM.setPWM(6, 0, CLeg.StepA1);
      PWM.setPWM(7, 0, CLeg.StepA2);
      PWM.setPWM(11, 0, CLeg.StepA3);
    break;

    case ARD:
      PWM.setPWM(9, 0, CLeg.StepA1);
      PWM.setPWM(10, 0, CLeg.StepA2);
      PWM.setPWM(9, 0, CLeg.StepA3);
    break;
    default :  Serial.println("default");
  }
}

void CoorToDeg(Leg &CLeg){
  /*// Merci Julien Marchand et François Tièche !!!
  float r, t, alpha, beta, gamma,  halfPerimeter, angleR1, angleR2;
  bool lefty = false;   // pas utile avec la config mécanique choisie

  float old1 = CLeg.DegA2;
  float old2 = CLeg.DegA3;

  //CLeg.X+=CLeg.Cx;
  //CLeg.Y+=CLeg.Cy;    // Application correctifs translation

  r = sqrt(CLeg.X*CLeg.X+CLeg.Y*CLeg.Y);  
  alpha = atan2(CLeg.Y, CLeg.X);
  halfPerimeter = (L1+L2+r)/2;

  // sin(beta/2)
  t = (halfPerimeter-L1)*(halfPerimeter-r)/(L1*r);
  if (t < 0.0) {
    Serial.println("INACESSIBLE");
  }

  beta = asin(sqrt(t))*2.0;

  // sin(gamma/2)
  t = (halfPerimeter-L1)*(halfPerimeter-L2)/(L1*L2);
  if (t < 0.0) {
     Serial.println("INACCESSIBLE");
  }

  gamma = asin(sqrt(t))*2.0;
  gamma -= M_PI;

  angleR1 = alpha + (lefty ? beta : -beta);
  angleR2 = (lefty ? gamma : -gamma);

  CLeg.DegA2 = angleR1 * 180 / M_PI;
  CLeg.DegA3 = angleR2 * 180 / M_PI;
  CLeg.DegA2+=CLeg.CDegA2;
  CLeg.DegA3+=CLeg.CDegA3;            // Application correctifs angles
  Serial.println("Angles des moteurs");
  Serial.println(CLeg.DegA2);
  Serial.println(CLeg.DegA3);*/


  
}

int ComSerial(void){

}