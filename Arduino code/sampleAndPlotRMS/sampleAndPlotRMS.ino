// Build-in libraries
#include <Timer5.h>

// Constants
#define SAMPLES 256
#define RMSFLOOR 388

#define AUDIOSENSEPIN A1

uint16_t adcPosition = 0;
bool activeADC = false;
bool newADC = false;
bool adcFlag = false;
uint8_t blinkCnt = 0;

int adc0[SAMPLES] = {};
int adc1[SAMPLES] = {};

// Boost the ADC read times.
void AdcBooster() {
  ADC->CTRLA.bit.ENABLE = 0; // Disable ADC
  while( ADC->STATUS.bit.SYNCBUSY == 1 ); // Wait for synchronization
  ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV16 | // Divide Clock by 16.
  ADC_CTRLB_RESSEL_10BIT; // Result on 10 bits
  ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1 | // 1 sample
  ADC_AVGCTRL_ADJRES(0x00ul); // Adjusting result by 0
  ADC->SAMPCTRL.reg = 0x00; // Sampling Time Length = 0
  ADC->CTRLA.bit.ENABLE = 1; // Enable ADC
  while( ADC->STATUS.bit.SYNCBUSY == 1 ); // Wait for synchronization
}

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(256000);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  AdcBooster();
  
  MyTimer5.begin(10000); // Begin timer with 10000 Hz sampling
  MyTimer5.attachInterrupt(adcInterrupt); // Attach interrupt.
}

// Finds something similar to the RMS value of the audio signal.
// Only works for arrays of 2^n length.
uint16_t findRMS(int *rmsArray, int lengthOfArray) {
  uint16_t returnVal = 0;
  uint32_t tempVal = 0;

  for (int i = 0; i < lengthOfArray; i++) {
    tempVal+=sq(rmsArray[i]-RMSFLOOR); // Square the value
  }
  while (lengthOfArray != 1) {
    tempVal = tempVal >> 1; // Divide by two
    lengthOfArray = lengthOfArray >> 1; // Divide by two
  }

  returnVal = (uint16_t)sqrt(tempVal);
  
  return returnVal;
}

void adcInterrupt() {
  adcFlag = true;
}

// Interrupt used to take ADC measurements
void updateADC() {
  if(adcPosition == SAMPLES) {
    activeADC = !activeADC;
    adcPosition = 0;
    newADC = true;
  }
  if(!activeADC) {
    adc0[adcPosition] = analogRead(AUDIOSENSEPIN);
    adcPosition++;
  } else {
    adc1[adcPosition] = analogRead(AUDIOSENSEPIN);
    adcPosition++;
  }
}

void loop() {
  if (adcFlag) {
    updateADC();
    adcFlag = false;
  }
  // Your code here
  static uint16_t prevRMSValue = 0; 
  static int rmsValue;
  uint16_t latestRMSValue = 0;
  
  if (newADC) { // A new measurement of SAMPLES length is ready.
    if (activeADC) { // Check which array is currently not being updated
      latestRMSValue = findRMS(adc0, SAMPLES);
      latestRMSValue = (latestRMSValue + prevRMSValue) / 2;
      prevRMSValue = latestRMSValue;
    } else {
      latestRMSValue = findRMS(adc1, SAMPLES);
      latestRMSValue = (latestRMSValue + prevRMSValue) / 2;
      prevRMSValue = latestRMSValue;
    }
    
    rmsValue = (int)latestRMSValue;
    float rmsFloat = rmsValue/1023.0*3.3;
    //Serial.print("\n");
    Serial.println(rmsValue);
  
    if (blinkCnt == 20) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else if (blinkCnt == 39) {
      digitalWrite(LED_BUILTIN, LOW);
      blinkCnt = 0;
    }
    blinkCnt++;
    
    newADC = false;
  }
}
