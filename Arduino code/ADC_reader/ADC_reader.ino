#include <Timer5.h>
#include <movingAvg.h>

#define SAMPLES 256
#define SMOOTHING 20

volatile boolean adcFlag = true;

uint16_t adcPosition = 0;
boolean activeADC = false;
boolean newADC = false;

uint16_t adc0[SAMPLES] = {};
uint16_t adc1[SAMPLES] = {};

movingAvg rmsSmoothing(SMOOTHING);

void setup() {
  Serial.begin(115200);
  delay(1500); // Wait for serial to be ready.
  
  AdcBooster(); // Configure ADC to be fast.
  
  MyTimer5.begin(10000); // Begin timer with 10000 Hz sampling
  MyTimer5.attachInterrupt(adcControl); // Attach interrupt.

  rmsSmoothing.begin(); // Begin the averaging filter
}

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

void adcControl() {
  adcFlag = true;
  //updateADC(); // Should run significantly faster than the timer.
}

uint16_t findRMS(uint16_t *rmsArray, uint16_t lengthOfArray) {
  uint16_t returnVal = 0;
  uint32_t tempVal = 0;

  for (int i = 0; i < lengthOfArray; i++) {
    tempVal+=sq(rmsArray[i]); // Square the value
  }
  while (lengthOfArray != 1) {
    tempVal = tempVal >> 1; // Divide by two
    lengthOfArray = lengthOfArray >> 1; // Divide by two
  }

  returnVal = (uint16_t)sqrt(tempVal);
  
  return returnVal;
}

void updateADC() {
  if(adcPosition == SAMPLES) {
    activeADC = !activeADC;
    adcPosition = 0;
    newADC = true;
  }
  if(!activeADC) {
    adc0[adcPosition] = analogRead(A1);
    adcPosition++;
  } else {
    adc1[adcPosition] = analogRead(A1);
    adcPosition++;
  }
}

void loop() {
  if (adcFlag) {
    updateADC();
    adcFlag = false;
  }
  if (newADC) { // A new measurement of SAMPLES length is ready.
    if (activeADC) { // Check which array is currently not being updated
      rmsSmoothing.reading(findRMS(adc0, SAMPLES)); // Add RMS measurement to moving average.
    } else {
      rmsSmoothing.reading(findRMS(adc1, SAMPLES)); // Add RMS measurement to moving average.
    }
    Serial.println(rmsSmoothing.getAvg()); // Print avg RMS
    newADC = false;
  }
}
