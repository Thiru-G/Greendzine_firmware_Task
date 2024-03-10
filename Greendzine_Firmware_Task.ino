
#define LM35_PIN A0
const int LED_PIN = 13; // #define LED_PIN 13
const int interval = 500; // Time interval in milliseconds
// Define the threshold temperature
#define Thres_temp 30
#define Below_thres 250
#define Above_thres 500
int temp_adc_val;
  float temp_val;
  int temperature;
  volatile int dutyCycle = 0; // Duty cycle variable

void setup() {
  pinMode(LM35_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Set up Timer1
  cli(); // Disable interrupts

  // Set Timer1 in CTC mode with prescaler of 64
  TCCR1A = 0; // Clear control register A
  TCCR1B = 0; // Clear control register B
  TCNT1 = 0; // Clear Timer1 counter
  OCR1A = 125 - 1; // Set compare match register for desired interval (125 for 500ms)
  TCCR1B |= (1 << WGM12); // Configure Timer1 for CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10); // Set prescaler to 64 and start Timer1
  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt
  sei(); // Enable interrupts
  
}

void loop() {

  temp_adc_val = analogRead(LM35_PIN);	/* Read Temperature */
  temp_val = (temp_adc_val * 4.88);	/* Convert adc value to equivalent voltage */
  temperature = (temp_val/10);  
  
  // Check if the temperature is below the threshold
  if (temperature < Thres_temp) {
    // Blink the LED with 250ms interval
    blinkLED(Below_thres);
  } 
  // Check if the temperature is above the threshold
  else if (temperature >= Thres_temp) {
    // Blink the LED with 500ms interval
    blinkLED(Above_thres);
  }}

// Timer1 compare match interrupt service routine
void blinkLED(int interval) {
  static unsigned long previousMillis = 0;
  static int intervalCount = 0;

  unsigned long currentMillis = (intervalCount * 125) / 16; // Calculate current time in milliseconds (125 for 500ms)

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    intervalCount = 0;
  } else {
    intervalCount++;
  }

  // Calculate duty cycle based on current time interval
  dutyCycle = (currentMillis % interval) * 255 / interval;

  // Write duty cycle to PWM pin
  analogWrite(LED_PIN, dutyCycle);
}
