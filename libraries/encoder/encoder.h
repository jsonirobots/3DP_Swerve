class encoder{
  public:
    int ticks = 0;
    encoder(int pinA,int pinB);
    void update();
    void start();
    void reset();
    float getDistance(float ratio, float radius);
    float getAngle(float ratio);

  private:
    int n = LOW;
    int pinLast = LOW;
    int _pinA;
    int _pinB;
    float ticksPerRev=20.0;
};

encoder::encoder(int pinA,int pinB){
  _pinA=pinA;
  _pinB=pinB;
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
}

void encoder::update(){
  n=digitalRead(_pinA);
  if((pinLast == LOW) && (n == HIGH)) {
    if(digitalRead(_pinB) == LOW) {
      ticks--;
    }
    else{
      ticks++;
    }
  }
  pinLast = n;
}

void encoder::start(){
  Serial.begin (9600);
}
void encoder::reset(){
  ticks=0;
}

float encoder::getDistance(float ratio, float radius){
  return ticks/ticksPerRev * ratio * 2*3.1415*radius;
}

float encoder::getAngle(float ratio){ //in degrees
  return ticks * (360.0/ticksPerRev) * ratio;
}
