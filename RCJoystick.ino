// channel count
#define CHANNELS 8

// channel assignments
#define CHAN_AILE 1
#define CHAN_ELEV 2
#define CHAN_COLL 3
#define CHAN_RUDD 4
#define CHAN_BUTT 5
#define CHAN_THRO 8

byte LED_PIN = PC13;
byte PPM_PIN = PB9;

// timing data
int channel[CHANNELS];

volatile int lastTime = 0;
volatile int timeSince = 0;
volatile int pulseFlag = 0;


// interrupt routine, return time since last pulse
// triggers on rising edge using STM pin interrupt
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

    // aileron
    Joystick.X(map(channel[CHAN_AILE-1], 1000, 2000, 1000, 0));

    // elevator
    Joystick.Y(map(channel[CHAN_ELEV-1], 1000, 2000, 1000, 0));

    // collective
    Joystick.Z(map(channel[CHAN_COLL-1], 1000, 2000, 0, 1000));

    // rudder
    Joystick.Zrotate(map(channel[CHAN_RUDD-1], 1000, 2000, 0, 1000));

    // throttle
    Joystick.slider(map(channel[CHAN_THRO-1], 1000, 2000, 0, 1000));

    // button
    Joystick.button(1, channel[CHAN_BUTT-1] < 1500);
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
