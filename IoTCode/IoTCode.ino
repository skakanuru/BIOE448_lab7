/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/dcf2dc39-2cb5-4271-a6da-a1c3f4b9ab2a 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float bPM;
  float refreshRate;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
int counter = 0;
unsigned long pulse_period = 0;

const int sensor_pin = A0;
int pulse_signal = 0;
//float BPM = 0.0;

int upper_threshold = 860;
int lower_threshold = 800;

bool any_peak_detected = false;
bool first_peak_detected = false;
unsigned long first_pulse_time = 0;
unsigned long second_pulse_time = 0;

void setup() {
  Serial.begin(9600);
  delay(1500);
  initProperties();
  //Connect to cloud and get info/errors
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  //Wait for cloud connection
  while (ArduinoCloud.connected() != 1) {
    ArduinoCloud.update();
    delay(500);
  }
}

void loop() {
  pulse_signal = analogRead(sensor_pin);
  //Serial.println(pulse_signal);

  // Find leading edge of heart beat
  if (pulse_signal > upper_threshold && any_peak_detected == false) {
    if (first_peak_detected == false) {
      first_pulse_time = millis();
      first_peak_detected = true;
    } else {
      second_pulse_time = millis();
      pulse_period = second_pulse_time - first_pulse_time;
      first_pulse_time = second_pulse_time;
    }
    any_peak_detected = true;
  }

  //Find trailing edge
  if (pulse_signal < lower_threshold) {
    any_peak_detected = false;
  }

  bPM = (1.0 / pulse_period) * 60.0 * 1000;
  Serial.println(bPM);

  delay(50);
  counter++;
  if (counter > (refreshRate * 20.0)){
    ArduinoCloud.update();
    Serial.println(bPM);
    counter = 0;
  }
  
}

/*
  Since RefreshRate is READ_WRITE variable, onRefreshRateChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onRefreshRateChange()  {
  // Add your code here to act upon RefreshRate change
}
