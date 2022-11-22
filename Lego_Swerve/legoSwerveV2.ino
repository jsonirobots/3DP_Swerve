#include <swerveDrive.h>

int swervePins[4][10]={{32,33,7,30,31,6,52,53,50,51},{24,25,3,22,23,2,48,49,46,47},{36,37,9,34,35,8,44,45,42,43},{28,29,5,26,27,4,40,41,38,39}};
swerveDrive legoSwerve(swervePins);

const float steerRatio = 20.0/26.0 * 8.0/24.0 * 12.0/24.0;
const float driveRatio = 12.0/20.0;
const float legoRadius = 62.4/2.0/25.4; //lego wheel diameter in mm converted to inches
const float driveToSteerGearRatio = (10.0/24.0 * 1.0/24.0 * 12.0/20.0)/(10.0/24.0 * 1.0/24.0 * 8.0/24.0 * 20.0/26.0);

void setup() {
  legoSwerve.steerRatio = steerRatio;
  legoSwerve.driveRatio = driveRatio;
  legoSwerve.wheelRadius = legoRadius;
  legoSwerve.driveToSteerGearRatio = driveToSteerGearRatio;
  legoSwerve.driveSpeedMax = 240;
  legoSwerve.steerSpeedMax = 170;
  legoSwerve.driveSpeedMin = 70;
  legoSwerve.steerSpeedMin = 60;

  Serial.begin (2000000);

  delay(4000);
}

void loop() {
  //legoSwerve.strafe(0.85,10,0);
  legoSwerve.strafe(0.99,13,90);
  delay(4500);
  //legoSwerve.strafe(0,0,90);
  delay(150);
}



/*
int frontLeftDrive1 = 32;
int frontLeftDrive2 = 33;
int frontLeftDrivePWM = 7;
int frontLeftSteer1 = 30;
int frontLeftSteer2 = 31;
int frontLeftSteerPWM = 6;
int frontLeftDriveEncoder1 = 52;
int frontLeftDriveEncoder2 = 53;
int frontLeftSteerEncoder1 = 50;
int frontLeftSteerEncoder2 = 51;

int frontRightDrive1 = 24;
int frontRightDrive2 = 25;
int frontRightDrivePWM = 3;
int frontRightSteer1 = 22;
int frontRightSteer2 = 23;
int frontRightSteerPWM = 2;
int frontRightDriveEncoder1 = 48;
int frontRightDriveEncoder2 = 49;
int frontRightSteerEncoder1 = 46;
int frontRightSteerEncoder2 = 47;

int backLeftDrive1 = 36;
int backLeftDrive2 = 37;
int backLeftDrivePWM = 9;
int backLeftSteer1 = 34;
int backLeftSteer2 = 35;
int backLeftSteerPWM = 8;
int backLeftDriveEncoder1 = 44;
int backLeftDriveEncoder2 = 45;
int backLeftSteerEncoder1 = 42;
int backLeftSteerEncoder2 = 43;

int backRightDrive1 = 28;
int backRightDrive2 = 29;
int backRightDrivePWM = 5;
int backRightSteer1 = 26;
int backRightSteer2 = 27;
int backRightSteerPWM = 4;
int backRightDriveEncoder1 = 40;
int backRightDriveEncoder2 = 41;
int backRightSteerEncoder1 = 38;
int backRightSteerEncoder2 = 39;*/
