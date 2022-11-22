#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include <RF24_config.h>

int esc = 3;
int leftServo = 6;
int rightServo = 5;

struct controls{
  int throttle;
  float leftAileron;
  float rightAileron;
};
controls plane = {0,0.00,0.00};

RF24 radio(10,9);
const byte address[6] = "00001";

Servo ESC;
Servo leftAileron;
Servo rightAileron;

void setup(){
  //Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  ESC.attach(esc,1000,2000);
  rightAileron.attach(rightServo);
  leftAileron.attach(leftServo);
}

void loop() {
  if (radio.available()) {
    radio.read(&plane,sizeof(plane));
    leftAileron.write(plane.leftAileron);
    rightAileron.write(plane.rightAileron);
    ESC.write(plane.throttle);
    delay(15);
    //Serial.println(plane.rightAileron);
  }
}
