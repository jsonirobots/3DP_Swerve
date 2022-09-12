/*
input - 2 sets of vectors: strafe vector (4 modules pointing in a direction with a certain speed on all 4) all 4 dir same, all 4 speed same, speed and directino determine x,y
                           rotation vector (modules at 45, 135, 225, 315), all 4 at same speed, speed determines change in heading
strafe vector -> line from starting point a to point b (distance and angle)
rotation vector-> change in heading from start to finish (gyro angle)

-> these overall robot vectors assign values to module vectors: strafe affects direction and speed equally, rotation affects speed equally
-> pid loops for steering and drive of each module, dictated by pid loops for rotation and strafe targets

-> each main loop checks error from strafe and heading vectors, uses new error values to create vectors for each module to follow in its own pid

*/
#include <Wire.h>

class ams5600_custom {
public:
  ams5600_custom();
  int getMagnetStrength();
  float getTicks();

private:
  int readOneByte();
  int ams5600_address = 0x36;
  int raw_ang_hi = 0x0c;
  int raw_ang_lo = 0x0d;
  int stat = 0x0b;
};

word AMS_5600::getRawAngle(){
  return readTwoBytes(_raw_ang_hi, _raw_ang_lo);
}

int AMS_5600::getMagnetStrength(){
  int magStatus;
  /*0 0 MD ML MH 0 0 0*/
  /* MD high = magnet detected */
  /* ML high = AGC Maximum overflow, magnet to weak*/
  /* MH high = AGC minimum overflow, Magnet to strong*/
  magStatus = readOneByte(_stat);

  if (magStatus & 0x20){
    if (magStatus & 0x10)
      return 1; //too weak
    else if (magStatus & 0x08)
      return 3; //too strong
    else
      return 2; //just right
  }
  else
    return 0;
}
int AMS_5600::readOneByte(int in_adr)
{
  int retVal = -1;
  Wire.beginTransmission(_ams5600_Address);
  Wire.write(in_adr);
  Wire.endTransmission();
  Wire.requestFrom(_ams5600_Address, 1);
  while (Wire.available() == 0)
    ;
  retVal = Wire.read();

  return retVal;
}

word AMS_5600::readTwoBytes(int in_adr_hi, int in_adr_lo)
{
  /* Read 2 Bytes */
  Wire.beginTransmission(_ams5600_Address);
  Wire.write(in_adr_hi);
  Wire.endTransmission();
  Wire.requestFrom(_ams5600_Address, 2);
  while (Wire.available() < 2)
    ;

  int high = Wire.read();
  int low = Wire.read();
  return ( high << 8 ) | low;
}
