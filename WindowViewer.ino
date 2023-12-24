#include <avr/sleep.h>

// Define variables
unsigned long startTime;
const unsigned long timerDuration = 1 * 10 * 1000; // 5 minutes in milliseconds

unsigned long previousMillis = 0;  // Time of the last update
const int updateInterval = 50;

const int digitalPin = 6;
const int analogInputPin = A2;

const int numReadings = 5; // Number of readings to consider for the running mean

int readings[numReadings];  // Array to store sensor readings
int index = 0;  // Index for the current reading in the array
int total = 0;  // Running total of the last numReadings readings

bool windowOpened = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(digitalPin, OUTPUT);
  digitalWrite(digitalPin, LOW);
  Serial.begin(9600);

  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {

  
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= updateInterval){
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
    if(mean <= 150 && !windowOpened){
      startTimer();
    }
    if(timerExpired(currentMillis) && windowOpened){
      if(mean >= 400){
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
  return (currentTime - startTime >= timerDuration);
}
