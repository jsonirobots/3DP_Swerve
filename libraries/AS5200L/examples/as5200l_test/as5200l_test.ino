/* 
AS5200L magnetic encoder test
Author: Amadeusz Świerk, 08.04.2020


NOTE:

When setting start and stop angle, stop angle has to be minimum 18 degrees (205 steps) greater than start angle.
Otherwise stop angle won't be setted up and it's register will respond with the same value as start angle register.
*/

#include <Wire.h>
//#include <AS5200L.h>

AS5200L magneticEncoderBottom(bottomSensorAddress);
AS5200L magneticEncoderTop(topSensorAddress);
//AS5200L magneticEncoderBottom(D4, D3, bottomSensorAddress);
//AS5200L magneticEncoderTop(D4, D3, topSensorAddress);

uint16_t angleBottom = 0, angleTop = 0;
float startAngle = 64.4, stopAngle = 120.12;

void setup()
{
Serial.begin(115200);
Serial.println("\nAS5200L magnetic encoder test");

uint8_t magnetStatus = 0;
do {
magnetStatus = magneticEncoderBottom.getMagnetStatus();
if (magnetStatus == AS5200L_magnet_notDetected) {
  Serial.println("Could not find magnet.");
  }
else if (magnetStatus == AS5200L_magnet_tooWeak) {
  Serial.println("Magnet is too weak or placed to far from the sensor.");
  }
else if (magnetStatus == AS5200L_magnet_tooStrong) {
  Serial.println("Magnet is too strong or placed too close to the sensor.");
  }
else if (magnetStatus == AS5200L_magnet_correct) {
  Serial.println("Magnet status correct.");
  Serial.printf("Sensor gain: %d\n", magneticEncoderBottom.getGain());
  Serial.printf("Sensor magnitude: %d\n", magneticEncoderBottom.getMagnitude());
  }
delay(1000);
} while (magnetStatus != AS5200L_magnet_correct);

Serial.println(magneticEncoderBottom.getStartPosition());
magneticEncoderBottom.setStartPosition(4096 * (startAngle / 360));
Serial.printf("New start position: %d\n", magneticEncoderBottom.getStartPosition());

Serial.println(magneticEncoderBottom.getStopPosition());
magneticEncoderBottom.setStopPosition(4096 * (stopAngle / 360));
Serial.printf("New stop position: %d\n", magneticEncoderBottom.getStopPosition());

magneticEncoderBottom.setPowerMode(AS5200L_power_mode_normal);
magneticEncoderBottom.setHysteresis(AS5200L_hysteresis_off);
magneticEncoderBottom.setSlowFilter(AS5200L_slow_filter_lsbx16);
magneticEncoderBottom.setFastFilter(AS5200L_fast_filter_lsbx24);
magneticEncoderBottom.setLPMTimer(true);
}
 
void loop()
{
angleBottom = magneticEncoderBottom.getAngle();
angleTop = magneticEncoderTop.getAngle();
Serial.printf("%d, %d\n", angleBottom, angleTop);
delay(100);
}
