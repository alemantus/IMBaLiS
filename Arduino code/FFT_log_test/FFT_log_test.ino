#include "approxFFT.h"

//Example data:
int data1[1024] = {
  0, 60, 36, 36, 68, 19, -43, -19, -9, -40, 5, 74, 57, 44, 77, 40, -34, -23, -6, -41, -14, 59, 52, 27, 57, 34, -47, -51, -26, -60, -49, 28, 35, 4, 31, 24, -55, -72, -40, -65, -67, 11, 37, 5, 27, 38, -35, -65, -30, -43, -56, 18, 61, 30, 43, 66, 2, -44, -11, -16, -39, 23, 78, 50, 50, 78,
  23, -39, -16, -11, -44, 2, 66, 43, 30, 61, 18, -56, -43, -30, -65, -35, 38, 27, 5, 37, 11, -67, -65, -40, -72, -55, 24, 31, 4, 35, 28, -49, -60, -26, -51, -47, 33, 57, 27, 52, 59, -14, -41, -6, -23, -34, 39, 77, 44, 57, 74, 5, -40, -9, -19, -43, 18, 68, 36, 36, 60, 0,
  -60, -36, -36, -68, -19, 43, 19, 9, 40, -5, -74, -57, -44, -77, -40, 34, 23, 6, 41, 14, -59, -52, -27, -57, -34, 47, 51, 26, 60, 49, -28, -35, -4, -31, -24, 55, 72, 40, 65, 67, -11, -37, -5, -27, -38, 35, 65, 30, 43, 56, -18, -61, -30, -43, -66, -2, 44, 11, 16, 39, -22, -78, -50, -50,
  -78, -23, 39, 16, 11, 44, -2, -66, -43, -30, -61, -18, 56, 43, 30, 65, 35, -38, -27, -5, -37, -11, 67, 65, 40, 72, 55, -24, -31, -4, -35, -28, 49, 60, 26, 51, 47, -33, -57, -27, -52, -59, 14, 41, 6, 23, 34, -39, -77, -44, -57, -74, -5, 40, 9, 19, 43, -18, -68, -36, -36, -60, 0, 60,
  36, 36, 68, 19, -43, -19, -9, -40, 5, 74, 57, 44, 77, 40, -34, -23, -6, -41, -14, 59, 52, 27, 57, 34, -46, -51, -26, -60, -49, 28, 35, 4, 31, 24, -55, -72, -40, -65, -67, 11, 37, 5, 27, 38, -35, -65, -30, -43, -56, 17, 61, 30, 43, 66, 2, -44, -11, -16, -39, 22, 78, 50, 50, 78,
  23, -39, -16, -11, -44, 2, 66, 43, 30, 61, 18, -56, -43, -30, -65, -35, 38, 27, 5, 37, 11, -67, -65, -40, -72, -55, 24, 31, 4, 35, 28, -49, -60, -26, -51, -47, 33, 57, 27, 52, 59, -14, -41, -6, -23, -34, 39, 77, 44, 57, 74, 5, -40, -9, -19, -43, 18, 68, 36,
  36, 60, 0, -60, -36, -35, -68, -19, 43, 19, 9, 40, -5, -74, -57, -44, -77, -40, 34, 23, 6, 41, 14, -59, -52, -27, -57, -34, 46, 51, 26, 60, 49, -28, -35, -4, -31, -24, 55, 72, 40, 65, 67, -11, -37, -5, -27, -38, 35, 65, 30, 43, 56, -17, -61, -30, -43, -66, -2, 44, 11, 16, 39, -22, -78, -50, -50,
  -78, -23, 39, 16, 11, 44, -2, -66, -43, -30, -61, -18, 56, 43, 30, 65, 35, -38, -27, -5, -37, -11, 67, 65, 40, 72, 55, -24, -31, -4, -35, -28, 49, 60, 26, 51, 47, -33, -57, -27, -52, -59, 14, 41, 6, 23, 34, -39, -77, -44, -57, -74, -5, 40, 9, 19, 43, -18, -68, -36, -36, -60, 0, 60, 36, 35,
  68, 19, -43, -19, -9, -40, 5, 74, 57, 44, 77, 40, -34, -23, -6, -41, -14, 59, 52, 27, 57, 34, -46, -51, -26, -60, -49, 28, 35, 4, 31, 24, -55, -72, -40, -65, -67, 11, 37, 5, 27, 38, -35, -65, -30, -43, -56, 17, 61, 30, 43, 66, 2, -44, -11, -16, -39, 22, 78, 50, 50, 78, 23, -39, -16, -11, -44, 2, 66, 43, 30, 61, 18, -56, -43, -30, -65, -35, 38, 27,
  5, 37, 11, -66, -65, -40, -72, -55, 24, 31, 4, 35, 28, -49, -60, -26, -51, -47, 33, 57, 27, 52, 59, -14, -41, -6, -23, -34, 39, 77, 44, 57, 74, 5, -40, -9, -19, -43, 18, 68, 36, 36, 60, 0, -60, -36, -35, -68, -19, 43, 19, 9, 40, -5, -74, -57, -44, -77, -40, 34, 23, 6, 41, 14, -59, -52,
  -27, -57, -34, 46, 51, 26, 60, 49, -28, -35, -4, -31, -24, 55, 72, 40, 65, 67, -11, -37, -5, -27, -38, 35, 65, 30, 43, 56, -17, -61, -30, -43, -66, -2, 44, 11, 16, 39, -22, -78, -50, -50, -78, -23, 39, 16, 11, 44, -2, -66, -43, -30, -61, -18, 56, 43, 30, 65, 35, -38, -27, -5, -37, -11, 66, 65, 40, 72, 55,
  -24, -31, -4, -35, -28, 49, 60, 26, 51, 47, -33, -57, -27, -52, -59, 14, 41, 6, 23, 34, -39, -77, -44, -57, -74, -5, 40, 9, 19, 43, -18, -68, -36, -36, -60, 0, 60, 36, 35, 68, 19,
  -43, -19, -9, -40, 5, 74, 57, 44, 77, 40, -34, -23, -6, -41, -14, 59, 52, 27, 57, 34, -46, -51, -26, -60, -49, 28,
  35, 4, 31, 24, -55, -72, -40, -65, -67, 11, 37, 5, 27, 38, -35, -65, -30, -43, -56, 17, 61, 30, 43, 66, 2, -44, -11, -16, -39, 22, 78, 50, 50, 78, 23, -39, -16, -11, -44, 2, 66, 43, 30, 61, 18, -56, -43, -30, -65, -35, 37, 27, 5, 37, 11, -66, -65, -40, -72, -55, 24, 31, 4, 35, 28, -49, -60, -26, -51,
  -47, 33, 57, 27, 52, 59, -14, -41, -6, -23, -34, 39, 77, 44, 57, 74, 5, -40, -9, -19, -43, 18, 68, 36, 36, 60, 0, -60, -36, -35, -68, -19, 43, 19, 9, 40, -5, -74, -57, -44, -77, -40, 34, 23, 6, 41, 14, -59, -52, -27, -57, -34, 46, 51, 26, 60, 49, -28, -35, -4, -31, -24, 54, 72, 40, 65, 67, -11,
  -37, -5, -27, -38, 35, 65, 30, 43, 56, -17, -61, -30, -43, -66, -2, 44, 11, 16, 39, -22, -78, -50, -50, -78, -23, 39, 16, 11, 44, -2, -66, -44, -30, -61, -18, 56, 43, 30, 65, 35, -37, -27, -5, -37, -11, 66, 65, 40, 72, 55, -24, -31, -4, -35, -28, 49, 60, 26, 51, 47, -33, -57, -27, -52, -59, 14,
  41, 6, 23, 34, -39, -77, -44, -57, -74, -5, 40, 9, 19, 43, -18, -68, -36, -36, -60, 0, 60, 36, 35, 68, 19, -43, -19
};



long double startTime = 0;
long double stopTime = 0;
double totalTimeApproxFFT = 0;
const uint16_t samples = 1024; //This value MUST ALWAYS be a power of 2
const double signalFrequency = 500;
const double samplingFrequency = 10000;
int debugMessages = 0;
int debugGraph = 1;

/*
  These are the input and output vectors
  Input vectors receive computed results from FFT
*/
int data_half[samples];
int data_quarter[samples];
int vReal[samples];
int vImag[samples];
int outbuffer[samples];// / 4];
int fft_result[samples];



/*
  test parameters
*/
uint8_t count = 1;




void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (debugMessages) {
    Serial.println("startup");
  }

}





void loop() {
  /*
    test code
  */
  for (uint16_t i = 0; i < samples; i++) {
    if (count == 1) {
      vReal[i] = data1[i]; // import from data
    }
    //vReal[i] = int((amplitude * (sin((i * (twoPi * cycles)) / samples))) / 2.0);/* Build data with positive and negative values*/
    //vReal[i] = uint8_t((amplitude * (sin((i * (twoPi * cycles)) / samples) + 1.0)) / 2.0);/* Build data displaced on the Y axis to include only positive values*/
    vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  }
  /*
    end of test code
  */





  if (count == 1) {
    startTime = micros();
    Approx_FFT(vReal, samples, fft_result);   //to get top five value of frequencies of X having 64 sample at 100Hz sampling
    binning_4(samples, fft_result, outbuffer);

    for (uint16_t i = 0; i < (samples >> 2); i++) {
      if (debugGraph) {
        Serial.print(outbuffer[i]);
        Serial.println(" ");
      }
    }
    stopTime = micros();
    if (debugMessages) {
      totalTimeApproxFFT = stopTime - startTime;
      Serial.print("approxFFT: Total time is [uS] ");  // debug printing
      Serial.println(totalTimeApproxFFT);
    }

    /*
      Serial.print("outputbuffer contains:");
      for (uint16_t i = 0; i < samples/4; i++) {
      Serial.print(outbuffer[i]);
      Serial.print(" ");
      Serial.print(i);
      //Serial.print(" ");
      //Serial.print(fft_result[i]);
      Serial.println(" ");
      }
    */
    count++;
  }
}
