#define CHANNELS 8
#define FILTER 10

byte LED_PIN = PC13;
byte PPM_PIN = PB9;

int channel[CHANNELS];

volatile int lastTime = 0;
volatile int timeSince = 0;
volatile int pulseFlag = 0;

void ppmTimer() {
  timeSince = micros() - lastTime;
  lastTime = micros();
  pulseFlag = 1;
}


void setup() {
  //Serial.begin(9600);

  timeSince = 0;
  lastTime = micros();
  
  pinMode(PPM_PIN, INPUT);  // ppm input stream
  pinMode(LED_PIN, OUTPUT); // output led

  attachInterrupt(PPM_PIN, ppmTimer, RISING);
}


void loop() {

  if(timeSince > 4000) { // end of framespace
    for(int i = 0; i < CHANNELS; i++) {
      while(!pulseFlag);
      pulseFlag = 0;

      channel[i] = timeSince;
      if(timeSince > 4000) // oops, went into framespace
        return;
    }
    
    Joystick.X(map(channel[0], 1000, 2000, 1000, 0));
    Joystick.Y(map(channel[1], 1000, 2000, 1000, 0));

    Joystick.button(1, channel[4] < 1500);
  }
  


 /* Test code 
  delay(500);
  Joystick.X(1000);
  delay(500);
  Joystick.Y(1000);
  delay(500);
  Joystick.X(0);
  delay(500);
  Joystick.Y(0);
 */
}
