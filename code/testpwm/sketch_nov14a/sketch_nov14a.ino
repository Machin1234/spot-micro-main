/***************************************************
  Contrôle de 3 servomoteurs avec le module pour servos PCA9685.
  Utilise la bibliothèque PMW Servo Driver d'Adafruit:
  https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
  Pour plus d'informations:
  https://electroniqueamateur.blogspot.com/2021/01/controler-plusieurs-servomoteurs-avec.html
  
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// valeurs approximatives qui peuvent varier d'un servomoteur à l'autre:
#define SERVOMIN 160     // Position minimale
#define SERVOMAX 450     // Position maximale
#define INTERVALLE 2000  // pour le 3e servomoteur

unsigned long previousMillis = 0;

// positions initiales
float position0 = SERVOMIN, position1 = SERVOMIN, position2 = SERVOMIN;

// sens initial du mouvement
int sens0 = 0, sens1 = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  pwm.begin();

  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);  // fréquence de 50 Hz pour les servomoteurs

  randomSeed(analogRead(0));  // pour la fonction random

  delay(10);
  position0 = 400;
}


void loop() {
  Serial.println(position0);
  pwm.setPWM(0, 0, 290);
  pwm.setPWM(1, 0, 320);
  pwm.setPWM(2, 0, 200);
  pwm.setPWM(3, 0, 380);
  delay(1000);
  // Canal 0 (premier servomoteur)
  // aller-retour très lent

  /*if (sens0) {
    position0 = position0 - 0.05;
    if (position0 < SERVOMIN) {
      sens0 = !sens0;  // changement de direction
      position0 = SERVOMIN;
    }
  }
  else {
    position0 = position0 + 0.05;
    if (position0 > SERVOMAX) {
      sens0 = !sens0; // changement de direction
      position0 = SERVOMAX;
    }
  }

  pwm.setPWM(0, 0, position0);  // servomoteur branché au canal 0

  // Canal 1 (deuxième servomoteur)
  // aller à vitesse moyenne, retour rapide

  if (sens1) {
    position1 = position1 - 0.3;
    if (position1 < SERVOMIN) {
      sens1 = !sens1;  // chagement de direction
      position1 = SERVOMIN;
    }
  }
  else {
    position1 = position1 + 1;
    if (position1 > SERVOMAX) {
      sens1 = !sens1; // changement de direction
      position1 = SERVOMAX;
    }
  }

  pwm.setPWM(1, 0, position1);  // servomoteur branché au canal 1

  // Canal 2 (troisième servomoteur)
  // position aléatoire maintenue pendant 2 secondes

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= INTERVALLE) {
    previousMillis = currentMillis;
    position2 = random(SERVOMIN, SERVOMAX);
    pwm.setPWM(2, 0, position2);  // servomoteur branché au canal 2
  }*/
}