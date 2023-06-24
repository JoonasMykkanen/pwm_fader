#include <avr/sleep.h>

int volt_lights;
int volt_ignition;
int engine;
int fade;
int fade_out;
int has_started;

const int pwmIntervals = 2000;
float R;
  
void setup() {
  pinMode(ResetPin, OUTPUT);
  Serial.begin(9600); //Monitoring
  Serial.println("Starting...");
  engine = 0;
  fade = 0;
  fade_out = 0;
  has_started = 0;
  volt_lights = 0;
  volt_ignition = 0;
  pinMode(9, OUTPUT); //Inner lights
  pinMode(10, OUTPUT); //Outter ligts
  pinMode(A0, INPUT); //Input from ignition
  pinMode(A5, INPUT); //Input from door lights
  TCCR1A = (TCCR1A & B00111100) | B10000010;   //Phase and frequency correct, Non-inverting mode, TOP defined by ICR1
  TCCR1B = (TCCR1B & B11100000) | B00010001;   //No prescale
  ICR1 = 0xFFFF;
}
 
void loop() {
  //First things first, Check if engine is on.
  volt_ignition = analogRead(A0);
  delay(250);

  // Check for engine signal
  //Car is running lights should be on all the time
  if (volt_ignition > 900) {
    Serial.println("Engine = ON");
    has_started = 1;
    engine = 1;
    pwm_16(65535);
  }

  // if not engine signal, lights should be off
  if (volt_ignition < 900) {
    engine = 0;
    Serial.println("Engine = OFF");
    if (has_started < 1) {
      pwm_16(0);
    }
    delay(250);
    // Pin A5 is taking signal from cars footwell lights
    // that is used to start fade in
    volt_lights = analogRead(A5);
    if (fade < 1) {
      if (volt_lights > 900) {
        //Ligts are on and engine is off, we should start fading in the angel eyes.
        Serial.println("Fade_in = True");
        fade = 1;
        fade_in();
        pwm_16(65535);
        delay(1000);
      }
    }
    if (volt_lights < 900) {
      Serial.println("Footwell = OFF");
      Serial.println("has_started = ON");
      fade_down();
      delay(1000);
      engine = 0;
      fade = 0;
      fade_out = 0;
      has_started = 0;
      volt_lights = 0;
      volt_ignition = 0;
      Serial.println("Resets done");
    }
  }
}

// Fade fuctions
void fade_in() {
  R = (pwmIntervals * log10(2)) / (log10(65535));
  int brightness = 0;
  for (int interval = 0; interval <= pwmIntervals; interval++) {
    brightness = pow (2, (interval / R)) - 1;
    pwm_16(brightness);
    Serial.println(brightness);
    delay(5);
  }
}

void fade_down() {
  R = (pwmIntervals * log10(2)) / (log10(65535));
  int brightness = 65535;
  for (int interval = 2000; interval >= 0; interval--) {
    brightness = pow (2, (interval / R)) - 1;
    pwm_16(brightness);
    Serial.println(brightness);
    delay(5);
  }
}

// Sending out signal for current brigtness
void pwm_16(int val){
  OCR1A = val;
}
