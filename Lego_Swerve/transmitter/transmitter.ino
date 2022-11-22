#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <SPI.h>
#include <RF24_config.h>

#include <Servo.h>

Servo mys;
void setup() 
{
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  mys.attach(10);
  /*pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);  
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);*/
  Serial.begin(9600);
}

void loop() 
{
  //double xspeed=0;
  if(Serial.available()){
    /*xspeed=codeToSpeed(Serial.read());
    //Serial.println(xspeed);
    /*if(xspeed<-0.25){
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      analogWrite(11,100);
      delay(400);
    }*/
    int val = Serial.read();
    if(val==1){
      //digitalWrite(10, LOW);
      //digitalWrite(9, HIGH);
      //analogWrite(11,230);
      //delay(1000);
      mys.write(90);
      digitalWrite(13,HIGH);
      //delay(2000);
      //digitalWrite(13,LOW);

    }
    delay(10);
    /*if(Serial.read() == 's'){
      digitalWrite(13,HIGH);
      delay(2000);
    }
    else{
      digitalWrite(13,LOW);
    }*/
  } 
  else{
    digitalWrite(13,LOW);
    /*
      digitalWrite(10, LOW);
      digitalWrite(9, LOW);*/
  } 
}

int codeToSpeed(char code){
  switch(code) {
    case 'a': return 2;
    case 'b': return 6;
    case 'c': return 10;
    case 'd': return 14;
    case 'e': return 18;
    case 'f': return 22;
    case 'g': return 26;
    case 'h': return 30;
    case 'i': return 34;
    case 'j': return 38;
    case 'k': return 42;
    case 'l': return 46;
    case 'm': return 50;
    case 'n': return 54;
    case 'o': return 58;
    case 'p': return 62;
    case 'q': return 66;
    case 'r': return 70;
    case 's': return 74;
    case 't': return 78;
    case 'u': return 82;
    case 'v': return 86;
    case 'w': return 90;
    case 'x': return 94;
    case 'y': return 98;
    case 'z': return 0;
    default: return 0;
  }
}
