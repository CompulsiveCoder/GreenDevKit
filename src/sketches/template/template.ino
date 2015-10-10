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

//const int latchPin = 9;
//Pin connected to clock pin (SH_CP) of 74HC595
//const int clockPin = 10;
////Pin connected to Data in (DS) of 74HC595
//const int dataPin = 8;

const int threshold = 2; // (0 to 8)

/*byte ledGraphPatterns[] = {
  B00000001,
  B00000011,
  B00000111,
  B00001111,
  B00011111,
  B00111111,
  B01111111,
  B11111111,
};*/

int index = 0;
//int totalPatternCount = sizeof(ledGraphPatterns);

// Sleeper
ISR(WDT_vect) { Sleepy::watchdogEvent(); }

void setup() {
  Serial.begin(9600);
  //pinMode(latchPin, OUTPUT);
  //pinMode(clockPin, OUTPUT);
  //pinMode(dataPin, OUTPUT);
  for (int i = 0; i < sizeof(sensorPins); i++)
  {
    pinMode(pumpPins[i], OUTPUT);
  }
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
  
  /*int ledGraphLevel = map(moistureReading, 1023, 350, 0, totalPatternCount);
  
  Serial.print("LED graph level: "); 
  Serial.println(ledGraphLevel);
  
  if (ledGraphLevel < threshold)
  {
    Serial.println("Water on");
    digitalWrite(signalPin, HIGH);
  }
  else
  {
    Serial.println("Water off");
    digitalWrite(signalPin, LOW);
  }
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, ledGraphPatterns[ledGraphLevel]);
  digitalWrite(latchPin, HIGH);
  */
  
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
