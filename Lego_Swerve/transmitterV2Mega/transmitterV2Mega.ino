#include <encoder.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include <RF24_config.h>

float refAngle = 90.0;

RF24 radio(8,7);
const byte address[6] = "00001";

struct controls{
  int throttle;
  float leftAileron;
  float rightAileron;
};

controls plane = {0,refAngle,refAngle};

void setup(){
  Serial.begin(38400);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  if(Serial.available()){
    char cmd = Serial.read();
    //plane.throttle=60;
    //plane.leftAileron = refAngle + float(percentAileron(cmd))/100.0 * 60.0;
    if(cmd=='$'){//||cmd=='#'){
      plane.throttle = 0;
    }
    if(cmd=='!'||cmd=='@'){
      plane.leftAileron = refAngle;
      plane.rightAileron = refAngle;  
    }
    else if(65<=cmd<=90){
      float aileronPercent = float(percentAileron(cmd));
      if(aileronPercent<0){
        plane.leftAileron = refAngle + aileronPercent/100.0 * 80.0;
        plane.rightAileron = refAngle;
      }
      else{
        plane.rightAileron = refAngle + aileronPercent/100.0 * 80.0;
        plane.leftAileron = refAngle;
      }
    }
    else if(48<=cmd<=57){
      plane.throttle = int(percentThrottle(cmd)/100.0 * 70);
    }
    /*else if(97<=cmd<=122){
      plane.leftAileron = refAngle + float(percentElevator(cmd))/100.0 * 60.0;
      plane.rightAileron = refAngle - float(percentElevator(cmd))/100.0 * 60.0;;
    }
    /*if(cmd == 's'){
      plane.rightAileron = 130.0;
    }
    if(cmd == 'x'){
      plane.rightAileron = 30.0;
    }*/
  }
  radio.write(&plane,sizeof(plane));
}
float percentThrottle(char cmd){
  switch(cmd){
    case '0': return 5.0;
    case '1': return 15.0;
    case '2': return 25.0;
    case '3': return 35.0;
    case '4': return 45.0;
    case '5': return 55.0;
    case '6': return 65.0;
    case '7': return 75.0;
    case '8': return 85.0;
    case '9': return 95.0;
    default: return 0.0;
  }
}
int percentAileron(char cmd){
  switch(cmd){
    case 'A': return 4;
    case 'B': return 12;
    case 'C': return 20;
    case 'D': return 28;
    case 'E': return 36;
    case 'F': return 44;
    case 'G': return 52;
    case 'H': return 60;
    case 'I': return 68;
    case 'J': return 76;
    case 'K': return 84;
    case 'L': return 92;
    case 'M': return 100;
    case 'N': return -4;
    case 'O': return -12;
    case 'P': return -20;
    case 'Q': return -28;
    case 'R': return -36;
    case 'S': return -44;
    case 'T': return -52;
    case 'U': return -60;
    case 'V': return -68;
    case 'W': return -76;
    case 'X': return -84;
    case 'Y': return -92;
    case 'Z': return -100;
    default: return 0;
  }
}
int percentElevator(char cmd){
  switch(cmd){
    case 'a': return 4;
    case 'b': return 12;
    case 'c': return 20;
    case 'd': return 28;
    case 'e': return 36;
    case 'f': return 44;
    case 'g': return 52;
    case 'h': return 60;
    case 'i': return 68;
    case 'j': return 76;
    case 'k': return 84;
    case 'l': return 92;
    case 'm': return 100;
    case 'n': return -4;
    case 'o': return -12;
    case 'p': return -20;
    case 'q': return -28;
    case 'r': return -36;
    case 's': return -44;
    case 't': return -52;
    case 'u': return -60;
    case 'v': return -68;
    case 'w': return -76;
    case 'x': return -84;
    case 'y': return -92;
    case 'z': return -100;
    default: return 0;
  }
}
