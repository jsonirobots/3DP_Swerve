/*#include <PIDcontroller_custom.h>*/

#include <Servo.h>

#include <Wire.h>
#include <AS5600.h>
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
  #define SYS_VOL   3.3
#else
  #define SERIAL Serial
  #define SYS_VOL   5
#endif

Servo drive_esc;     /* create servo object to control the ESC*/
AMS_5600 steer_enc;
AMS_5600 drive_enc;

/* Arduino NANO pins
int sma1 = 7;
int sma2 = 6;
int smb1 = 8;
int smb2 = 9;
int smap = 5;
int smbp = 10;

int escPin = 11;*/

/*Arduino Mega Pins*/
int sma1 = 22;
int sma2 = 24;
int smb1 = 26;
int smb2 = 28;
int smap = 3;
int smbp = 4;

int escPin = 2;

void setup() {
  pinMode(sma1,OUTPUT);
  pinMode(smb1,OUTPUT);
  pinMode(sma2,OUTPUT);
  pinMode(smb2,OUTPUT);
  pinMode(smap,OUTPUT);
  pinMode(smbp,OUTPUT);
  Serial.begin(115200);
  Wire.begin();
  //testEncoder();
  
  //drive_esc.write(90);
  delay(4000);
  Serial.println("Angle,Goal,Input");
  
  //driveFT();
  //armESC();
  //testModule();
  
  /*Wire.begin();
  if(steer_enc.detectMagnet() == 0 ){
    while(1){
        if(steer_enc.detectMagnet() == 1 ){
            SERIAL.print("Current Magnitude: ");
            SERIAL.println(steer_enc.getMagnitude());
            delay(100);
        }
        else{
            SERIAL.println("Can not detect magnet");
            delay(10);
        }
        delay(1000);
    }
  }
  delay(3000);*/
}

float convertRawAngleToDegrees(word newAngle)
{
  return float(newAngle) * 360.0000/4095.0000;
  
}
float convertRawAngleToRadians(word newAngle)
{
  return float(newAngle * (TWO_PI)/4095.0000);
  
}

int ii=0;
bool steer = false;
void loop() { 
  //testEncoder(); 
  /*if(steer){*/
    if(ii>=5){
      Serial.println("End");
      delay(10000);
    }
    steerWithPid();
    delay(1500);
    ii=ii+1;
  /*}
  else{
    driveFT();
  }*/
}

void chooseI2Cbus(int bus){
  if(bus>=0 && bus<=7){
    
    Wire.beginTransmission(byte(0x70)); //address for I2C multiplexer
    Wire.write(1 << bus);
    Wire.endTransmission();
  }
}

int steerMotor(int motorSpeed){
  if(abs(motorSpeed) < 45 && abs(motorSpeed) > 0){
    motorSpeed = 45 * ((motorSpeed>0) ? 1 : -1);
  }
  else if(abs(motorSpeed) > 254){
    motorSpeed = 254 * ((motorSpeed>0) ? 1 : -1);
  }
  if(motorSpeed == 0){
    digitalWrite(sma1,LOW);
    digitalWrite(sma2,LOW);
    digitalWrite(smb1,LOW);
    digitalWrite(smb2,LOW);
    analogWrite(smap,0);
    analogWrite(smbp,0);
  }
  else if(motorSpeed > 0){
    digitalWrite(sma1,HIGH);
    digitalWrite(sma2,LOW);
    digitalWrite(smb1,HIGH);
    digitalWrite(smb2,LOW);
    analogWrite(smap,motorSpeed);
    analogWrite(smbp,motorSpeed);
  }
  else{
    digitalWrite(sma1,LOW);
    digitalWrite(sma2,HIGH);
    digitalWrite(smb1,LOW);
    digitalWrite(smb2,HIGH);
    analogWrite(smap,-1*motorSpeed);
    analogWrite(smbp,-1*motorSpeed);
  }
  return motorSpeed;
}

void steerWithPid(){
  chooseI2Cbus(0);
  float testDelta = PI/2;
  float thetaNow = convertRawAngleToRadians(steer_enc.getRawAngle());
  float thetaFinal = (thetaNow<=3.0000*HALF_PI) ? thetaNow+testDelta : testDelta-(TWO_PI-thetaNow);
  bool compensate = false;
  if(testDelta > PI/2){
    compensate = true;
  }

  float xtr = cos(thetaFinal);
  float ytr = sin(thetaFinal);

  float deltaT = 1;
  long currT = micros();
  long prevT = currT;
  
  float margin = 0.5000*PI/360.0000;
  float errorNow = cos(thetaNow)*ytr - xtr*sin(thetaNow);
  float errorPrev = errorNow;
  float errorInit = errorNow;

  float iTerm = 0.0000;
  float dTerm = 0.0000;
  float kP = 0.35;
  float kI = 0;
  float kD = 0;
  float motorInput = 0.0;
  float oldAngle = thetaNow;
  float xnw =0;float ynw=0;

  int reachedGoal = 0;
  int reportSpeed=0;
  while(reachedGoal < 4){  
    thetaNow = convertRawAngleToRadians(steer_enc.getRawAngle());
    currT = micros();
    deltaT = ((float) (currT - prevT))/( 1.0e6 );
    prevT = currT;

    if(fabs(thetaFinal - thetaNow)<=margin){
      reachedGoal +=1;
    }
    
    xnw = cos(thetaNow);
    ynw = sin(thetaNow);
    errorNow = xnw*ytr - xtr*ynw;
    if(compensate && reachedGoal<1 && abs(errorNow)>abs(errorPrev)){
      errorNow = 1;
    }
    if(fabs(errorNow/errorInit) > 0.92){
      kP = 0.45;
      kD = 0.0003;
    }
    dTerm = (errorNow - errorPrev)/deltaT;
    oldAngle = thetaNow;
    iTerm += errorNow * deltaT;
  
    motorInput = kP * errorNow + kI * iTerm + kD * dTerm;
    
    reportSpeed=steerMotor(int(motorInput*240)); 
    errorPrev = errorNow;
    //Serial Monitor Formatted Outputs
    /*SERIAL.print("Input: ");SERIAL.print(motorInput*240);
    SERIAL.print(" | Error: ");SERIAL.print(errorNow);
    SERIAL.print(" | Angle: ");SERIAL.print(thetaNow*180.00/PI);
    SERIAL.print(" | Goal: ");SERIAL.print(thetaFinal*180.00/PI);
    SERIAL.print(" | D: ");SERIAL.println(dTerm*kD);*/

    //Serial Plotter Formatted Outputs
    /*SERIAL.print(thetaFinal);SERIAL.print(",");
    SERIAL.println(thetaNow);*/

    //CSV file format outputs
    Serial.print(thetaNow*180.00/PI);Serial.print(",");
    Serial.print(thetaFinal*180.00/PI);Serial.print(",");
    Serial.println(map(reportSpeed,-254,254,-100,100));//Serial.print(",");
    //Serial.println(motorInput*240);
  }
  steerMotor(0);
  delay(2000);
  Serial.print(convertRawAngleToRadians(steer_enc.getRawAngle())*180.00/PI);
  Serial.print(",");
  Serial.print(thetaFinal*180.00/PI);Serial.print(",");
  Serial.println(0);
  //SERIAL.println((thetaFinal - convertRawAngleToRadians(steer_enc.getRawAngle()))*180.00/PI);
}

void armESC(){
  drive_esc.attach(escPin);
  long now = millis();
  while(millis()<now+5000){
    drive_esc.writeMicroseconds(1000);
    delay(200);
  }
}

void driveFT(){
  drive_esc.write(90);
  delay(3000);
  for(int slow=90;slow>=0;slow-=5){
    drive_esc.write(slow);
    delay(100);
  }
}

void testEncoder(){
  chooseI2Cbus(0);
  Serial.println("Steer Encoder");
  long now = millis();
  while(millis()<=now+5000){
    Serial.println(convertRawAngleToDegrees(steer_enc.getRawAngle()));
  }
  delay(2000);
  chooseI2Cbus(1);
  Serial.println("Drive Encoder");
  now=millis();
  while(millis()<=now+5000){
    Serial.print("Angle: ");
    Serial.println(convertRawAngleToDegrees(drive_enc.getRawAngle()));
  }
}

void testModule(){
  /*check steering motors 1 at a time, and read encoder values to serial*/
  /*
  chooseI2Cbus(0);

  Serial.println("Steering Motor A----------------------------------");
  digitalWrite(sma1,HIGH);
  digitalWrite(sma2,LOW);
  long now = millis();
  while(millis()<=now+5000){
    analogWrite(smap,110);
    Serial.print("Angle: ");
    Serial.println(convertRawAngleToDegrees(steer_enc.getRawAngle()));
  }
  analogWrite(smap,0);
  delay(2000);
  digitalWrite(sma1,LOW);
  digitalWrite(sma2,LOW);
  delay(200);
  Serial.println("_____________________");
  Serial.println("Steering Motor B----------------------------------");
  digitalWrite(smb1,HIGH);
  digitalWrite(smb2,LOW);
  now = millis();
  while(millis()<=now+5000){
    analogWrite(smbp,110);
    Serial.println(convertRawAngleToDegrees(steer_enc.getRawAngle()));
  }
  analogWrite(smbp,0);
  delay(2000);
  digitalWrite(smb1,LOW);
  digitalWrite(smb2,LOW);
  delay(4000);
  */
  
  //Check both steering motors at the same time and send encoder value to serial
  /*
  chooseI2Cbus(0);
  Serial.println("Steering Motors A and B----------------------------------");
  digitalWrite(sma1,HIGH);
  digitalWrite(sma2,LOW);
  digitalWrite(smb1,HIGH);
  digitalWrite(smb2,LOW);
  long now = millis();
  while(millis()<=now+7000){
    analogWrite(smap,110);
    analogWrite(smbp,110);
    Serial.print("Angle: ");
    Serial.println(convertRawAngleToDegrees(steer_enc.getRawAngle()));
  }
  analogWrite(smap,0);
  analogWrite(smbp,0);
  delay(2000);
  digitalWrite(sma1,LOW);
  digitalWrite(sma2,LOW);
  digitalWrite(smb1,LOW);
  digitalWrite(smb2,LOW);
  delay(4000);
  */

  //check driving motor and send encoder value to serial
  
  chooseI2Cbus(1);
  Serial.println("Driving Motor ------------------------------------");
  long now = millis();
  drive_esc.writeMicroseconds(map(20,0,100,1300,2000));

  while(millis()<=now+5000){
    Serial.println(convertRawAngleToDegrees(drive_enc.getRawAngle()));
  }
  for(int slow=20;slow>=0;slow-=5){
    drive_esc.writeMicroseconds(map(slow,0,100,1300,2000));
    Serial.println(convertRawAngleToDegrees(drive_enc.getRawAngle()));
    delay(200);
  }
}
