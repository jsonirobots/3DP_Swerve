#include <C:\Users\jatin\Documents\Arduino\libraries\encoder\encoder.h>

class swerveDrive{
public:
  //0-d1, 1-d2, 2-dpwm, 3-s1, 4-s2, 5-spwm, 6-denc1, 7-denc2, 8-senc1,9-senc2
  swerveDrive(int _swervePins[4][10]);
  void strafe(float driveSpeed,float distance,float steerAngle);
  float driveRatio,wheelRadius,steerRatio,driveToSteerGearRatio;
  int steerSpeedMax,steerSpeedMin,driveSpeedMax,driveSpeedMin;

private:
  int swervePins[4][10];
  void setupDrive(boolean forwards=true);
  void setupSteer(boolean clockwise=true);
  void stopDrive();
  void stopSteer(int module);
};

swerveDrive::swerveDrive(int _swervePins[4][10]){
  for(int i=0;i<4;i++){
    for(int j=0;j<6;j++){
      pinMode(_swervePins[i][j], OUTPUT);
    }
  }
  for(int i=0;i<4;i++){
    for(int j=0;j<10;j++){
      swervePins[i][j]=_swervePins[i][j];
    }
  }
}

void swerveDrive::stopDrive(){
  for(int i=0;i<4;i++){
    digitalWrite(swervePins[i][0], LOW);
    digitalWrite(swervePins[i][1], LOW);
  }
}
void swerveDrive::stopSteer(int module){
  digitalWrite(swervePins[module][3], LOW);
  digitalWrite(swervePins[module][4], LOW);
}

void swerveDrive::setupDrive(boolean forwards=true){
  if(forwards){
    for(int i=0;i<4;i++){
      digitalWrite(swervePins[i][0], LOW);
      digitalWrite(swervePins[i][1], HIGH);
    }
  }
  else{
    for(int i=0;i<4;i++){
      digitalWrite(swervePins[i][0], HIGH);
      digitalWrite(swervePins[i][1], LOW);
    }
  }
}

void swerveDrive::setupSteer(boolean clockwise=true){
  if(clockwise){
    for(int i=0;i<4;i++){
      digitalWrite(swervePins[i][3], HIGH);
      digitalWrite(swervePins[i][4], LOW);
    }
  }
  else{
    for(int i=0;i<4;i++){
      digitalWrite(swervePins[i][3], LOW);
      digitalWrite(swervePins[i][4], HIGH);
    }
  }
}

void swerveDrive::strafe(float driveSpeed,float driveDistance,float steerAngle){
  encoder frontLeftDriveEncoder(swervePins[0][6],swervePins[0][7]);
  encoder frontLeftSteerEncoder(swervePins[0][8],swervePins[0][9]);
  encoder frontRightDriveEncoder(swervePins[1][6],swervePins[1][7]);
  encoder frontRightSteerEncoder(swervePins[1][8],swervePins[1][9]);
  encoder backLeftDriveEncoder(swervePins[2][6],swervePins[2][7]);
  encoder backLeftSteerEncoder(swervePins[2][8],swervePins[2][9]);
  encoder backRightDriveEncoder(swervePins[3][6],swervePins[3][7]);
  encoder backRightSteerEncoder(swervePins[3][8],swervePins[3][9]);
  if(driveSpeed!=0&&steerAngle==0){
    if(driveSpeed>0){setupDrive();}
    else{setupDrive(false);}
    float initialDistance = (0.25*(frontLeftDriveEncoder.getDistance(driveRatio,wheelRadius)+frontRightDriveEncoder.getDistance(driveRatio,wheelRadius)+backLeftDriveEncoder.getDistance(driveRatio,wheelRadius)+backRightDriveEncoder.getDistance(driveRatio,wheelRadius)));
    float deltaDistance = 0;
    while(deltaDistance<driveDistance){
      for(int i=0;i<4;i++){
        analogWrite(swervePins[i][2],abs(driveSpeed)*driveSpeedMax);
      }
      frontLeftDriveEncoder.update();frontRightDriveEncoder.update();backLeftDriveEncoder.update();backRightDriveEncoder.update();
      deltaDistance = abs((0.25*(frontLeftDriveEncoder.getDistance(driveRatio,wheelRadius)+frontRightDriveEncoder.getDistance(driveRatio,wheelRadius)+backLeftDriveEncoder.getDistance(driveRatio,wheelRadius)+backRightDriveEncoder.getDistance(driveRatio,wheelRadius))) - initialDistance);
    }
    stopDrive();
  }
  if(steerAngle!=0&&driveSpeed==0){
    if(steerAngle>0){setupSteer();}
    else{setupSteer(false);}
    float fLInitialAngle = frontLeftSteerEncoder.getAngle(steerRatio);
    float fRInitialAngle = frontRightSteerEncoder.getAngle(steerRatio);
    float bLInitialAngle = backLeftSteerEncoder.getAngle(steerRatio);
    float bRInitialAngle = backRightSteerEncoder.getAngle(steerRatio);

    float fLDeltaAngle = 0, fRDeltaAngle = 0, bLDeltaAngle = 0, bRDeltaAngle = 0;
    boolean fLDone=false, fRDone=false, bLDone=false, bRDone=false;

    while(!fLDone||!fRDone||!bLDone||!bRDone){
      analogWrite(swervePins[0][5],steerSpeedMax);
      analogWrite(swervePins[1][5],steerSpeedMax);
      analogWrite(swervePins[2][5],steerSpeedMax);
      analogWrite(swervePins[3][5],steerSpeedMax);
      frontLeftSteerEncoder.update();frontRightSteerEncoder.update();backLeftSteerEncoder.update();backRightSteerEncoder.update();
      fLDeltaAngle = abs(frontLeftSteerEncoder.getAngle(steerRatio) - fLInitialAngle);
      fRDeltaAngle = abs(frontRightSteerEncoder.getAngle(steerRatio) - fRInitialAngle);
      bLDeltaAngle = abs(backLeftSteerEncoder.getAngle(steerRatio) - bLInitialAngle);
      bRDeltaAngle = abs(backRightSteerEncoder.getAngle(steerRatio) - bRInitialAngle);
      if(fLDeltaAngle>=abs(steerAngle)){fLDone=true;stopSteer(0);}
      if(fRDeltaAngle>=abs(steerAngle)){fRDone=true;stopSteer(1);}
      if(bLDeltaAngle>=abs(steerAngle)){bLDone=true;stopSteer(2);}
      if(bRDeltaAngle>=abs(steerAngle)){bRDone=true;stopSteer(3);}
    }
  }
  else if(driveSpeed!=0&&steerAngle!=0){
    float initialDistance = frontLeftDriveEncoder.getDistance(driveRatio,wheelRadius);
    float deltaDistance = 0;

    float fLInitialAngle = frontLeftSteerEncoder.getAngle(steerRatio);
    float fRInitialAngle = frontRightSteerEncoder.getAngle(steerRatio);
    float bLInitialAngle = backLeftSteerEncoder.getAngle(steerRatio);
    float bRInitialAngle = backRightSteerEncoder.getAngle(steerRatio);
    float fLDeltaAngle = 0, fRDeltaAngle = 0, bLDeltaAngle = 0, bRDeltaAngle = 0;
    boolean fLDone=false, fRDone=false, bLDone=false, bRDone=false;
    boolean steeringSwerve = true, drivingSwerve = true;

    if(driveSpeed>0){setupDrive();}
    else{setupDrive(false);}
    if(steerAngle>0){setupSteer();}
    else{setupSteer(false);}

    float steerSpeed = abs(steerAngle)/driveDistance * abs(driveSpeed)*driveSpeedMax * 2*3.1415*wheelRadius/360.0 * driveToSteerGearRatio;
    if(steerSpeed>steerSpeedMax){steerSpeed=steerSpeedMax;}else if(steerSpeed<steerSpeedMin){steerSpeed=steerSpeedMin;}
    driveSpeed = abs(driveSpeed)*driveSpeedMax;
    if(driveSpeed>driveSpeedMax){driveSpeed=driveSpeedMax;}else if(driveSpeed<driveSpeedMin){driveSpeed=driveSpeedMin;}

    while(drivingSwerve||steeringSwerve){
      if(drivingSwerve){
        for(int i=0;i<4;i++){
          analogWrite(swervePins[i][2],driveSpeed);
        }
        frontLeftDriveEncoder.update();//frontRightDriveEncoder.update();backLeftDriveEncoder.update();backRightDriveEncoder.update();
        deltaDistance = abs(frontLeftDriveEncoder.getDistance(driveRatio,wheelRadius) - initialDistance);
      }
      if(steeringSwerve){
        analogWrite(swervePins[0][5],steerSpeed);
        analogWrite(swervePins[1][5],steerSpeed);
        analogWrite(swervePins[2][5],steerSpeed);
        analogWrite(swervePins[3][5],steerSpeed);
        frontLeftSteerEncoder.update();frontRightSteerEncoder.update();backLeftSteerEncoder.update();backRightSteerEncoder.update();
        fLDeltaAngle = abs(frontLeftSteerEncoder.getAngle(steerRatio) - fLInitialAngle);
        fRDeltaAngle = abs(frontRightSteerEncoder.getAngle(steerRatio) - fRInitialAngle);
        bLDeltaAngle = abs(backLeftSteerEncoder.getAngle(steerRatio) - bLInitialAngle);
        bRDeltaAngle = abs(backRightSteerEncoder.getAngle(steerRatio) - bRInitialAngle);
      }
      if(deltaDistance>=driveDistance){
        drivingSwerve=false;
        stopDrive();
      }
      if(fLDeltaAngle>=abs(steerAngle)){fLDone=true;stopSteer(0);}
      if(fRDeltaAngle>=abs(steerAngle)){fRDone=true;stopSteer(1);}
      if(bLDeltaAngle>=abs(steerAngle)){bLDone=true;stopSteer(2);}
      if(bRDeltaAngle>=abs(steerAngle)){bRDone=true;stopSteer(3);}
      if(fLDone&&fRDone&&bLDone&&bRDone){steeringSwerve=false;}
    }
  }
}
