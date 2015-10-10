#include <JeeLib.h> 

// <SleepDuration>
const int sleepDuration = 1000;
// </SleepDuration>

// <SensorPinCount>
const int sensorPinCount = 2;
// </SensorPinCount>

// <SensorPins>
const int sensorPins[] = {A0,A1};
// </SensorPins>

// <SensorPowerPins>
const int sensorPowerPins[] = {2,3};
// </SensorPowerPins>

// <PumpPins>
const int pumpPins[] = {4,5};
// </PumpPins>

// <ThresholdPins>
const int thresholdPins[] = {A4,A5};
// </ThresholdPins>

// <Thresholds>
const int thresholds[] = {30,30};
// </Thresholds>

int moistureLevels[sensorPinCount];

// Sleeper
ISR(WDT_vect) { Sleepy::watchdogEvent(); }

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < sizeof(sensorPins); i++)
    pinMode(pumpPins[i], OUTPUT);
}

void loop() {
  for (int i = 0; i < sizeof(sensorPins); i++)
  {
    double rawReading = getRawReading(i);
   
    moistureLevels[i] = map(rawReading, 1023, 350, 0, 100);
    
    int currentThreshold = thresholds[i];
    
    if (pumpPins[i] > -1)
    {
      if (thresholdPins[i] > -1)
      {
        int rawThresholdReading = analogRead(thresholdPins[i]);
        currentThreshold = map(rawThresholdReading, 1023, 0, 0, 100);
      }
      
      if (moistureLevels[i] < currentThreshold)
      {
        Serial.println("Water on");
        digitalWrite(pumpPins[i], HIGH);
      }
      else
      {
        Serial.println("Water off");
        digitalWrite(pumpPins[i], LOW);
      }
    }
  }
  
  Sleepy::loseSomeTime(sleepDuration); 
}

double getRawReading(int index)
{
    if (sensorPowerPins[index] > -1)
    {
      digitalWrite(sensorPowerPins[index], HIGH);
      delay(500); // TODO: Is this delay necessary?
    }
    
    int rawReading = analogRead(sensorPins[index]);
  
    Serial.print("Raw reading: ");
    Serial.print(index);
    Serial.print(": ");  
    Serial.println(rawReading);
   
    if (sensorPowerPins[index] > -1)
    {
      digitalWrite(sensorPowerPins[index], LOW);
    }
    
    return rawReading;
}
