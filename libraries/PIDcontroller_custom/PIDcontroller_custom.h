/*
  PIDcontroller_custom

  This library creates a PID controller to be used in Arduino programs for controlling systems.

  Initialization:
    PIDcontroller_custom myController([kP,kI,kD],setPoint,dt);
    kP, kI, kD are your PID constants
    setPoint is the target point
    dt is the time increment in ms, default is 10 ms

  Running PID:
    You can implement this PID controller with a loop in your program
    Eg.
      while(currentX < targetX){
        analogWrite(motorPWM,runPID(currentX));
        currentX = readSensor();
      }

  Created 13 March 2022
  By Jatin Soni
*/

class PIDcontroller_custom{
public:
  PIDcontroller_custom(float PIDconsts[3],float _setPoint,float _dt = 10);
  float runPID(float currentPoint);
private:
  float oldPoint = 0.00;
  float errorSum = 0.00;
  float error = 0.00;
  float kP = 0.00;
  float kD = 0.00;
  float kI = 0.00;
  float dt = 0;
  float setPoint = 0.00;
};

PIDcontroller_custom::PIDcontroller_custom(float PIDconsts[3],float _setPoint,float _dt = 10){
  kP = PIDconsts[0];
  kI = PIDconsts[1];
  kD = PIDconsts[2];
  dt = _dt;
  setPoint = _setPoint;
}

float PIDcontroller_custom::runPID(float currentPoint){
  error = setPoint - currentPoint;
  errorSum = errorSum + error*dt;
  float command = kD * (oldPoint-currentPoint)/dt + kP * currentPoint + kI * errorSum;
  oldPoint = currentPoint;
  return command;
}
