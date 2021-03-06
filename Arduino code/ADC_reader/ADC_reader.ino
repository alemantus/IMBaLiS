/*
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/e519a107-7052-4304-8878-2a0e46a0c9ce

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  int rmsValue;
  bool iotChangeMode;
  bool iotStartStop;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"

// Build in libraries
#include <Timer5.h>
#include <LiquidCrystal.h>
#include <ArduinoGraphics.h> // Arduino_MKRRGB depends on ArduinoGraphics
#include <Arduino_MKRRGB.h>
#include <Button.h>

// 'Local' library
#include "approxFFT.h"

// Constants
#define SAMPLES 256
#define SMOOTHING 10
#define COLUMNS 12
#define LEDOFFSET 500
#define LEDMAX 800

#define BTTNPIN1 9
#define BTTNPIN2 10
#define AUDIOSENSEPIN A1
#define LEDOUTPUTPIN A0

// Variables
LiquidCrystal lcd(0, 1, 2, 3, 4, 5);

uint16_t adcPosition = 0;
bool activeADC = false;
bool newADC = false;
uint8_t blinkCnt = 0;

int adc0[SAMPLES] = {};
int adc1[SAMPLES] = {};
int dataOut[SAMPLES] = {};
uint16_t ledBuffer[COLUMNS] = {};
int rgb[] = {20, 50, 100};

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

void setVerticalBar(int heightOfBar, int barNum, int lightPower[]) {
  MATRIX.beginDraw();
  MATRIX.stroke(lightPower[0], lightPower[1], lightPower[2]);
  MATRIX.line(barNum, 0, barNum, heightOfBar - 1);
  MATRIX.endDraw();
}

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(256000);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you???ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Setup of non auto generated
  AdcBooster(); // Configure ADC to be fast.
  
  MyTimer5.begin(10000); // Begin timer with 10000 Hz sampling
  MyTimer5.attachInterrupt(updateADC); // Attach interrupt.

  rmsSmoothing.begin(); // Begin the averaging filter
  
  lcd.begin(16,2); // Start LCD with 16 columns and 2 rows.
  
  pinMode(BTTNPIN1, INPUT);
  pinMode(BTTNPIN2, INPUT);
  
  // Set DAC resolution
  analogWriteResolution(10);
  
  MATRIX.begin(); // Start the RGB matrix
  MATRIX.brightness(1); // Set the brightness, due to power concerns keep it low.
}

// Finds something similar to the RMS value of the audio signal.
// Only works for arrays of 2^n length.
uint16_t findRMS(int *rmsArray, int lengthOfArray) {
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

void resetRGBShield() {
  MATRIX.beginDraw();
  MATRIX.clear(); 
  MATRIX.endDraw();
  memset(ledBuffer,0,sizeof(ledBuffer)*COLUMNS);
}

void loop() {
  ArduinoCloud.update();
  
  // Your code here
  static bool bttn1State = false;
  static bool bttn1StateOld = false;
  static bool bttn1StateLatch = false;
  static bool bttn2State = false;
  static bool bttn2StateOld = false;
  static bool bttn2StateLatch = false;
  static bool modeState = false;
  static uint16_t prevRMSValue = 0; 
  uint16_t latestRMSValue = 0;
  uint16_t ledDACValue = 0;
  
  if (newADC) { // A new measurement of SAMPLES length is ready.
    if (activeADC) { // Check which array is currently not being updated
      latestRMSValue = findRMS(adc0, SAMPLES);
      latestRMSValue = (latestRMSValue + prevRMSValue) / 2;
      prevRMSValue = latestRMSValue;

      if (bttn1StateLatch) {
        Approx_FFT(adc0, SAMPLES, dataOut);
        
        resetRGBShield();
        
        binning_12_16(SAMPLES, dataOut, ledBuffer);
        
        for (int i = 0; i < COLUMNS; i++) {
          setVerticalBar(map(ledBuffer[i], 0, 15000, 0, 7), i, rgb);
        }
      } else {
        resetRGBShield();
      }
    } else {
      latestRMSValue = findRMS(adc1, SAMPLES);
      latestRMSValue = (latestRMSValue + prevRMSValue) / 2;
      prevRMSValue = latestRMSValue;

      if (bttn1StateLatch) {
        Approx_FFT(adc1, SAMPLES, dataOut);
      
        resetRGBShield();
        
        binning_12_16(SAMPLES, dataOut, ledBuffer);
          
        for (int i = 0; i < COLUMNS; i++) {
          setVerticalBar(map(ledBuffer[i], 0, 15000, 0, 7), i, rgb);
        }
      } else {
        resetRGBShield();
      }
    }
    
    rmsValue = (int)latestRMSValue;
    ledDACValue = map(rmsValue, 350, LEDMAX, LEDOFFSET, 1023);
    analogWrite(LEDOUTPUTPIN, ledDACValue);
    
    lcd.setCursor(0,0);
    lcd.print("RMS: ");
    if (rmsValue == 0) {
      lcd.print("000");
    } else {
      lcd.print(rmsValue);
    }

    bttn1State = digitalRead(BTTNPIN1);
    bttn2State = digitalRead(BTTNPIN2);
    
    if (bttn1State != bttn1StateOld) {
      bttn1StateOld = bttn1State;
      if (bttn1State == HIGH) {
        bttn1StateLatch = !bttn1StateLatch;
      }
    }
    
    if (bttn2State != bttn2StateOld) {
      bttn2StateOld = bttn2State;
      if (bttn2State == HIGH) {
        bttn2StateLatch = !bttn2StateLatch;
      }
    }
    
    lcd.setCursor(0,1);
    lcd.print("MODE:      ");
    lcd.setCursor(6,1);
    if (bttn1StateLatch) {
      lcd.print("ON/");
    } else {
      lcd.print("OFF/");
    }
    if (modeState) {
      lcd.print("1");
    } else {
      lcd.print("2");
    }
    
    if (bttn2StateLatch) {
      if (rgb[0] == 20) {
        rgb[0] = 100;
        rgb[2] = 20;
        modeState = true;
      } else {
        rgb[0] = 20;
        rgb[2] = 100;
        modeState = false;
      }
      ArduinoCloud.printDebugInfo();
      bttn2StateLatch = false;
    }
  
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




void onIotChangeModeChange() {
  // Do something
}


void onIotStartStopChange() {
  // Do something
}
