#include <avr/sleep.h>

#define FULL_BRIGTHNESS 65535
#define OFF 0
#define ON 1

// init loop variables
int fade;
int engine;
int fade_out;
int has_started;
int volt_lights;
int volt_ignition;

// Init fade variables
const int pwmIntervals = 2000;
float R;

void setup() {
	Serial.begin(9600); //Monitoring
	Serial.println("Starting...");
	engine = OFF;
	fade = OFF;
	fade_out = OFF;
	has_started = OFF;
	volt_lights = OFF;
	volt_ignition = OFF;
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
		has_started = ON;
		engine = ON;
		pwm_16(65535);
  }

  // if not engine signal, lights should be off
  if (volt_ignition < 900) {
    engine = OFF;
    Serial.println("Engine = OFF");
    if (has_started == ON) {
      pwm_16(0);
    }
    delay(250);
    // Pin A5 is taking signal from cars footwell lights
    // that is used to start fade in
    volt_lights = analogRead(A5);
    if (fade == OFF) {
      if (volt_lights > 900) {
        //Ligts are on and engine is off, we should start fading in the angel eyes.
        Serial.println("Fade_in = True");
        fade = ON;
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
		engine = OFF;
		fade = OFF;
		fade_out = OFF;
		has_started = OFF;
		volt_lights = OFF;
		volt_ignition = OFF;
		Serial.println("Resets done");
    	}
  	}
}

void fade_in(void) {
  	R = (pwmIntervals * log10(2)) / (log10(FULL_BRIGTHNESS));
  	int brightness = OFF;

  	for (int interval = 0; interval <= pwmIntervals; interval++) {
		brightness = pow (2, (interval / R)) - 1;
		pwm_16(brightness);
		Serial.println(brightness);
		delay(5);
  	}
}

void fade_down(void) {
	R = (pwmIntervals * log10(2)) / (log10(FULL_BRIGTHNESS));
	int brightness = FULL_BRIGTHNESS;

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
