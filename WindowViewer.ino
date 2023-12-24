#include <avr/sleep.h>

// Define variables
unsigned long startTime;
const unsigned long windowOpenDuration = 1 * 10 * 1000; // time in milliseconds

unsigned long previousMillis = 0;  // Time of the last update
const int sensorUpdateInterval = 50;  //update of distanceSensor

const int speakerPin = 6;
const int distanceSensorPin = A2;

const int numReadings = 5; // Number of readings to consider for the running mean

int readings[numReadings];  // Array to store sensor readings
int index = 0;  // Index for the current reading in the array
int total = 0;  // Running total of the last numReadings readings

bool windowOpened = 0;
int distanceClosed = 400;   //volts, higher voltage means smaller distance -> window closed
int distanceOpen = 150;

void setup() {
  // put your setup code here, to run once:
  pinMode(speakerPin, OUTPUT);
  digitalWrite(speakerPin, LOW);
  Serial.begin(9600);

  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {

  
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= sensorUpdateInterval){
    int sensorValue = analogRead(analogInputPin);

    total = total - readings[index] + sensorValue;
    
    // Update the array with the new reading
    readings[index] = sensorValue;
    
    // Move to the next index, and wrap around if necessary
    index = (index + 1) % numReadings;
    
    // Calculate the running mean
    int mean = total / numReadings;
    
    Serial.print("Analog Value on A2: ");
    Serial.println(sensorValue);
    if(mean <= distanceOpen && !windowOpened){
      startTimer();
    }
    if(timerExpired(currentMillis) && windowOpened){
      if(mean >= distanceClosed){
        windowOpened = 0;
        digitalWrite(digitalPin, LOW);
      }else{
        digitalWrite(digitalPin, HIGH);
      }
    }
  }
  
}

void startTimer() {
  startTime = millis(); // Record the current time
  windowOpened = 1;
}

bool timerExpired(unsigned long currentTime) {
  return (currentTime - startTime >= windowOpenDuration);
}
