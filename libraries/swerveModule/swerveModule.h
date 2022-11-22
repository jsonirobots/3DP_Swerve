#include <C:\Users\jatin\Documents\Arduino\libraries\encoder\encoder.h>
class swerveModule{
  public:
    int drive1, drive2, drivePWM, steer1, steer2, steerPWM, driveEncoder1,driveEncoder2,steerEncoder1,steerEncoder2;

    swerveModule(int _drive1, int _drive2, int _drivePWM, int _driveEncoder1, int _driveEncoder2, int _steer1, int _steer2, int _steerPWM, int _steerEncoder1, int _steerEncoder2);
    void runModule(float driveSpeed = 0, float driveDistance = 0, float steerAngle = 0);
    float driveRatio,wheelRadius,steerRatio;
    int steerSpeedMax,driveSpeedMax;

  private:
    void stopModule();
};

swerveModule::swerveModule(int _drive1, int _drive2, int _drivePWM, int _driveEncoder1, int _driveEncoder2, int _steer1, int _steer2, int _steerPWM, int _steerEncoder1, int _steerEncoder2){
  drive1=_drive1;
  drive2=_drive2;
  drivePWM=_drivePWM;
  steer1=_steer1;
  steer2=_steer2;
  steerPWM=_steerPWM;
  driveEncoder1=_driveEncoder1;
  driveEncoder2=_driveEncoder2;
  steerEncoder1=_steerEncoder1;
  steerEncoder2=_steerEncoder2;
  pinMode(drivePWM, OUTPUT);
  pinMode(steerPWM, OUTPUT);
  pinMode(drive1, OUTPUT);
  pinMode(drive2, OUTPUT);
  pinMode(steer1, OUTPUT);
  pinMode(steer2, OUTPUT);

}

void swerveModule::stopModule(){
  digitalWrite(drive1, LOW);
  digitalWrite(drive2, LOW);
  digitalWrite(steer1, LOW);
  digitalWrite(steer2, LOW);
}

void swerveModule::runModule(float driveSpeed = 0, float driveDistance = 0, float steerAngle = 0){
  encoder driveEncoder(driveEncoder1,driveEncoder2);
  encoder steerEncoder(steerEncoder1,steerEncoder2);

  driveEncoder.start();
  steerEncoder.start();

  driveEncoder.reset();
  steerEncoder.reset();
  if(driveSpeed<0){
    digitalWrite(drive1, LOW);
    digitalWrite(drive2, HIGH);
  }
  else if(steerAngle<0){
    digitalWrite(steer1, LOW);
    digitalWrite(steer2, HIGH);
  }
  else{
    digitalWrite(drive1, HIGH);
    digitalWrite(drive2, LOW);
    digitalWrite(steer1, HIGH);
    digitalWrite(steer2, LOW);
  }

  if(driveSpeed!=0&&steerAngle==0){
    float initialDistance = driveEncoder.getDistance(driveRatio,wheelRadius);
    float deltaDistance = 0;
    while(deltaDistance<driveDistance){
      analogWrite(drivePWM,abs(driveSpeed)*driveSpeedMax);
      driveEncoder.update();
      deltaDistance = abs(driveEncoder.getDistance(driveRatio,wheelRadius) - initialDistance);
    }
  }
  if(steerAngle!=0&&driveSpeed==0){
    float initialAngle = steerEncoder.getAngle(steerRatio);
    float deltaAngle = 0;
    while(deltaAngle<abs(steerAngle)){
      analogWrite(steerPWM,steerSpeedMax);
      steerEncoder.update();
      deltaAngle = abs(steerEncoder.getAngle(steerRatio) - initialAngle);
    }
  }
  stopModule();
}
