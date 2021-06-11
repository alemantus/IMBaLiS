#include <Timer5.h>

volatile boolean adcFlag = true;

void setup() {
  // put your setup code here, to run once:
  AdcBooster();
  delay(100);
  
  MyTimer5.begin(10000); // Frequency of interrupt
  MyTimer5.attachInterrupt(AdcControl);
 
  Serial.begin(500000);
  while(!Serial) {}
  
  Serial.println("Init done.");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (adcFlag) {
    Serial.println(analogRead(A1));
    adcFlag = false;
  }  
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
} // AdcBooster

void AdcControl() {
  adcFlag = true;
}
