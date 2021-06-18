
/*
  //Example data: peak at 197Hz, 984Hz and 2953Hz
  const uint16_t samples = 1024; //This value MUST ALWAYS be a power of 2
  const double signalFrequency = 500;
  const double samplingFrequency = 10000;
  int vReal[1024] = {
  0, 60, 36, 36, 68, 19, -43, -19, -9, -40, 5, 74, 57, 44, 77, 40, -34, -23, -6, -41, -14,
  59, 52, 27, 57, 34, -47, -51, -26, -60, -49, 28, 35, 4, 31, 24, -55, -72, -40, -65, -67,
  11, 37, 5, 27, 38, -35, -65, -30, -43, -56, 18, 61, 30, 43, 66, 2, -44, -11, -16, -39, 23,
  78, 50, 50, 78, 23, -39, -16, -11, -44, 2, 66, 43, 30, 61, 18, -56, -43, -30, -65, -35,
  38, 27, 5, 37, 11, -67, -65, -40, -72, -55, 24, 31, 4, 35, 28, -49, -60, -26, -51, -47,
  33, 57, 27, 52, 59, -14, -41, -6, -23, -34, 39, 77, 44, 57, 74, 5, -40, -9, -19, -43, 18,
  68, 36, 36, 60, 0, -60, -36, -36, -68, -19, 43, 19, 9, 40, -5, -74, -57, -44, -77, -40,
  34, 23, 6, 41, 14, -59, -52, -27, -57, -34, 47, 51, 26, 60, 49, -28, -35, -4, -31, -24,
  55, 72, 40, 65, 67, -11, -37, -5, -27, -38, 35, 65, 30, 43, 56, -18, -61, -30, -43, -66,
  -2, 44, 11, 16, 39, -22, -78, -50, -50, -78, -23, 39, 16, 11, 44, -2, -66, -43, -30, -61,
  -18, 56, 43, 30, 65, 35, -38, -27, -5, -37, -11, 67, 65, 40, 72, 55, -24, -31, -4, -35,
  -28, 49, 60, 26, 51, 47, -33, -57, -27, -52, -59, 14, 41, 6, 23, 34, -39, -77, -44, -57,
  -74, -5, 40, 9, 19, 43, -18, -68, -36, -36, -60, 0, 60, 36, 36, 68, 19, -43, -19, -9, -40,
  5, 74, 57, 44, 77, 40, -34, -23, -6, -41, -14, 59, 52, 27, 57, 34, -46, -51, -26, -60, -49,
  28, 35, 4, 31, 24, -55, -72, -40, -65, -67, 11, 37, 5, 27, 38, -35, -65, -30, -43, -56, 17,
  61, 30, 43, 66, 2, -44, -11, -16, -39, 22, 78, 50, 50, 78, 23, -39, -16, -11, -44, 2, 66,
  43, 30, 61, 18, -56, -43, -30, -65, -35, 38, 27, 5, 37, 11, -67, -65, -40, -72, -55, 24,
  31, 4, 35, 28, -49, -60, -26, -51, -47, 33, 57, 27, 52, 59, -14, -41, -6, -23, -34, 39, 77,
  44, 57, 74, 5, -40, -9, -19, -43, 18, 68, 36, 36, 60, 0, -60, -36, -35, -68, -19, 43, 19,
  9, 40, -5, -74, -57, -44, -77, -40, 34, 23, 6, 41, 14, -59, -52, -27, -57, -34, 46, 51, 26,
  60, 49, -28, -35, -4, -31, -24, 55, 72, 40, 65, 67, -11, -37, -5, -27, -38, 35, 65, 30, 43,
  56, -17, -61, -30, -43, -66, -2, 44, 11, 16, 39, -22, -78, -50, -50, -78, -23, 39, 16, 11,
  44, -2, -66, -43, -30, -61, -18, 56, 43, 30, 65, 35, -38, -27, -5, -37, -11, 67, 65, 40, 72,
  55, -24, -31, -4, -35, -28, 49, 60, 26, 51, 47, -33, -57, -27, -52, -59, 14, 41, 6, 23, 34,
  -39, -77, -44, -57, -74, -5, 40, 9, 19, 43, -18, -68, -36, -36, -60, 0, 60, 36, 35, 68, 19,
  -43, -19, -9, -40, 5, 74, 57, 44, 77, 40, -34, -23, -6, -41, -14, 59, 52, 27, 57, 34, -46,
  -51, -26, -60, -49, 28, 35, 4, 31, 24, -55, -72, -40, -65, -67, 11, 37, 5, 27, 38, -35, -65,
  -30, -43, -56, 17, 61, 30, 43, 66, 2, -44, -11, -16, -39, 22, 78, 50, 50, 78, 23, -39, -16,
  -11, -44, 2, 66, 43, 30, 61, 18, -56, -43, -30, -65, -35, 38, 27, 5, 37, 11, -66, -65, -40,
  -72, -55, 24, 31, 4, 35, 28, -49, -60, -26, -51, -47, 33, 57, 27, 52, 59, -14, -41, -6, -23,
  -34, 39, 77, 44, 57, 74, 5, -40, -9, -19, -43, 18, 68, 36, 36, 60, 0, -60, -36, -35, -68,
  -19, 43, 19, 9, 40, -5, -74, -57, -44, -77, -40, 34, 23, 6, 41, 14, -59, -52, -27, -57, -34,
  46, 51, 26, 60, 49, -28, -35, -4, -31, -24, 55, 72, 40, 65, 67, -11, -37, -5, -27, -38, 35,
  65, 30, 43, 56, -17, -61, -30, -43, -66, -2, 44, 11, 16, 39, -22, -78, -50, -50, -78, -23,
  39, 16, 11, 44, -2, -66, -43, -30, -61, -18, 56, 43, 30, 65, 35, -38, -27, -5, -37, -11, 66,
  65, 40, 72, 55, -24, -31, -4, -35, -28, 49, 60, 26, 51, 47, -33, -57, -27, -52, -59, 14, 41,
  6, 23, 34, -39, -77, -44, -57, -74, -5, 40, 9, 19, 43, -18, -68, -36, -36, -60, 0, 60, 36,
  35, 68, 19, -43, -19, -9, -40, 5, 74, 57, 44, 77, 40, -34, -23, -6, -41, -14, 59, 52, 27, 57,
  34, -46, -51, -26, -60, -49, 28, 35, 4, 31, 24, -55, -72, -40, -65, -67, 11, 37, 5, 27, 38,
  -35, -65, -30, -43, -56, 17, 61, 30, 43, 66, 2, -44, -11, -16, -39, 22, 78, 50, 50, 78, 23,
  -39, -16, -11, -44, 2, 66, 43, 30, 61, 18, -56, -43, -30, -65, -35, 37, 27, 5, 37, 11, -66,
  -65, -40, -72, -55, 24, 31, 4, 35, 28, -49, -60, -26, -51, -47, 33, 57, 27, 52, 59, -14, -41,
  -6, -23, -34, 39, 77, 44, 57, 74, 5, -40, -9, -19, -43, 18, 68, 36, 36, 60, 0, -60, -36, -35,
  -68, -19, 43, 19, 9, 40, -5, -74, -57, -44, -77, -40, 34, 23, 6, 41, 14, -59, -52, -27, -57,
  -34, 46, 51, 26, 60, 49, -28, -35, -4, -31, -24, 54, 72, 40, 65, 67, -11, -37, -5, -27, -38,
  35, 65, 30, 43, 56, -17, -61, -30, -43, -66, -2, 44, 11, 16, 39, -22, -78, -50, -50, -78, -23,
  39, 16, 11, 44, -2, -66, -44, -30, -61, -18, 56, 43, 30, 65, 35, -37, -27, -5, -37, -11, 66,
  65, 40, 72, 55, -24, -31, -4, -35, -28, 49, 60, 26, 51, 47, -33, -57, -27, -52, -59, 14, 41,
  6, 23, 34, -39, -77, -44, -57, -74, -5, 40, 9, 19, 43, -18, -68, -36, -36, -60, 0, 60, 36,
  35, 68, 19, -43, -19
  };
*/

/*
  test code

  //These are the input and output vectors
  //Input vectors receive computed results from FFT
  const uint16_t samples = 1024; //This value MUST ALWAYS be a power of 2
  int vReal[samples];
  int outbuffer[samples];// / 4];
  int fft_result[samples];
  Approx_FFT(vReal, samples, samplingFrequency, fft_result);   //to get top five value of frequencies of X having 64 sample at 100Hz sampling
  binning_4(samples, fft_result, outbuffer);

  end of test code
*/

#include "approxFFT.h"



//-----------------------------FFT Function----------------------------------------------//
/*
  Code to perform High speed and Accurate FFT on arduino,
  setup:
  1. in[]     : Data array,
  2. N        : Number of sample (recommended sample size 2,4,8,16,32,64,128,256,512...)
  3. *out_r   : Return data array

  It will by default return magnitudes in out_r array

  If sample size is not in power of 2 it will be clipped to lower side of number.
  i.e, for 150 number of samples, code will consider first 128 sample, remaining sample  will be omitted.
  Code by ABHILASH; modified and sohoted for spesific usecase by Simon Wahlberg
  Contact: abhilashpatel121@gmail.com; s173925@student.dtu.dk
  Documentation & details: https://www.instructables.com/member/abhilash_patel/instructables/

  Update(06/05/21): Correction made for support on Arduino Due
  Update(16/06/21): made shorter for spesific usecase
*/


void Approx_FFT(int in[], int N, int *out_r) {
  int a, c1, f, o, x, data_max, data_min = 0;
  long data_avg, data_mag, temp11;
  byte scale, check = 0;

  data_max = 0;
  data_avg = 0;
  data_min = 0;

  for (int i = 0; i < 12; i++) {         //calculating the levels
    if (Pow2[i] <= N) {
      o = i;
    }
  }
  a = Pow2[o];
  //int out_r[a];   //real part of transform
  int out_im[a];  //imaginory part of transform

  for (int i = 0; i < a; i++) {         //getting min max and average for scalling
    out_r[i] = 0; out_im[i] = 0;
    data_avg = data_avg + in[i];
    if (in[i] > data_max) {
      data_max = in[i];
    }
    if (in[i] < data_min) {
      data_min = in[i];
    }
  }

  data_avg = data_avg >> o;
  scale = 0;
  data_mag = data_max - data_min;
  temp11 = data_mag;

  //scalling data  from +512 to -512

  if (data_mag > 1024) {
    while (temp11 > 1024) {
      temp11 = temp11 >> 1;
      scale = scale + 1;
    }
    for (int i = 0; i < a; i++) {
      in[i] = in[i] - data_avg;
      in[i] = in[i] >> scale;
    }
    scale = 128 - scale;
  } else if (data_mag < 1024) {
    while (temp11 < 1024) {
      temp11 = temp11 << 1;
      scale = scale + 1;
    }
    scale = scale - 1;
    for (int i = 0; i < a; i++) {
      in[i] = in[i] - data_avg;
      in[i] = in[i] << scale;
    }
    scale = 128 + scale;
  }


  x = 0;
  for (int b = 0; b < o; b++) {              // bit reversal order stored in im_out array
    c1 = Pow2[b];
    f = Pow2[o] / (c1 + c1);
    for (int j = 0; j < c1; j++) {
      x++;
      out_im[x] = out_im[j] + f;
    }
  }

  for (int i = 0; i < a; i++) {    // update input array as per bit reverse order
    out_r[i] = in[out_im[i]];
    out_im[i] = 0;
  }

  int i10, i11, n1, tr, ti;
  float e;
  int c, s, temp4;
  for (int i = 0; i < o; i++) {                            //fft
    i10 = Pow2[i];            // overall values of sine/cosine
    i11 = Pow2[o] / Pow2[i + 1]; // loop with similar sine cosine
    e = 1024 / Pow2[i + 1]; //1024 is equivalent to 360 deg
    e = 0 - e;
    n1 = 0;

    for (int j = 0; j < i10; j++) {
      c = e * j; //c is angle as where 1024 unit is 360 deg
      while (c < 0) {
        c = c + 1024;
      }
      while (c > 1024) {
        c = c - 1024;
      }

      n1 = j;

      for (int k = 0; k < i11; k++) {
        temp4 = i10 + n1;
        if (c == 0) {
          tr = out_r[temp4];
          ti = out_im[temp4];
        } else if (c == 256) {
          tr = -out_im[temp4];
          ti = out_r[temp4];
        } else if (c == 512) {
          tr = -out_r[temp4];
          ti = -out_im[temp4];
        } else if (c == 768) {
          tr = out_im[temp4];
          ti = -out_r[temp4];
        }
        else if (c == 1024) {
          tr = out_r[temp4];
          ti = out_im[temp4];
        } else {
          tr = fast_cosine(out_r[temp4], c) - fast_sine(out_im[temp4], c);      //the fast sine/cosine function gives direct (approx) output for A*sinx
          ti = fast_sine(out_r[temp4], c) + fast_cosine(out_im[temp4], c);
        }

        out_r[n1 + i10] = out_r[n1] - tr;
        out_r[n1] = out_r[n1] + tr;
        if (out_r[n1] > 15000 || out_r[n1] < -15000) {
          check = 1; //check for int size, it can handle only +31000 to -31000,
        }

        out_im[n1 + i10] = out_im[n1] - ti;
        out_im[n1] = out_im[n1] + ti;
        if (out_im[n1] > 15000 || out_im[n1] < -15000) {
          check = 1;
        }

        n1 = n1 + i10 + i10;
      }
    }

    if (check == 1) {                                         // scalling the matrics if value higher than 15000 to prevent varible from overflowing
      for (int i = 0; i < a; i++) {
        out_r[i] = out_r[i] >> 1;
        out_im[i] = out_im[i] >> 1;
      }
      check = 0;
      scale = scale - 1;             // tracking overall scalling of input data
    }
  }

  if (scale > 128) {
    scale = scale - 128;
    for (int i = 0; i < a; i++) {
      out_r[i] = out_r[i] >> scale;
      out_im[i] = out_im[i] >> scale;
    }
    scale = 0;
  } else {                                              // revers all scalling we done till here,
    scale = 128 - scale; // in case of nnumber getting higher than 32000, we will represent in as multiple of 2^scale
  }

  /*
    for(int i=0;i<a;i++)
    {
    Serial.print(out_r[i]);Serial.print("\t");                    // un comment to print RAW o/p
    Serial.print(out_im[i]);
    Serial.print("i");Serial.print("\t");
    Serial.print("*2^");Serial.println(scale);
    }
  */

  //---> here onward out_r contains amplitude
  for (int i = 1; i < Pow2[o - 1]; i++) {      // getting amplitude from compex number
    out_r[i] = fastRSS(out_r[i], out_im[i]);
    // Approx RSS function used to calculated magnitude quickly

    // un comment to print Amplitudes (1st value (offset) is not printed)
    //Serial.print(out_im[i]); Serial.print("\t"); Serial.println(out_r[i]);
    //Serial.print("*2^"); Serial.println(scale);
  }
}
//--------------------------------------------------------------------------------//


//---------------------------------fast sine/cosine-------------------------------//
int fast_sine(int Amp, int th) {
  int temp3; //, m1, m2;
  byte temp1, temp2, test, quad, accuracy;
  accuracy = 5;  // set it value from 1 to 7, where 7 being most accurate but slowest
  // accuracy value of 5 recommended for typical applicaiton
  while (th > 1024) {
    th = th - 1024; // here 1024 = 2*pi or 360 deg
  }
  while (th < 0) {
    th = th + 1024;
  }
  quad = th >> 8;

  if (quad == 1) {
    th = 512 - th;
  } else if (quad == 2) {
    th = th - 512;
  } else if (quad == 3) {
    th = 1024 - th;
  }

  temp1 = 0;
  temp2 = 128;    //2 multiple
  //m1 = 0;
  //m2 = Amp;

  temp3 = (Amp) >> 1; //(m1 + m2) >> 1;
  Amp = temp3;
  for (int i = 0; i < accuracy; i++) {
    test = (temp1 + temp2) >> 1;
    temp3 = temp3 >> 1;
    if (th > isin_data[test]) {
      temp1 = test;
      Amp = Amp + temp3;
      //m1 = Amp;
    } else if (th < isin_data[test]) {
      temp2 = test;
      Amp = Amp - temp3;
      //m2 = Amp;
    }
  }

  if (quad == 2) {
    Amp = 0 - Amp;
  } else if (quad == 3) {
    Amp = 0 - Amp;
  }
  return (Amp);
}

int fast_cosine(int Amp, int th) {
  th = 256 - th; //cos th = sin (90-th) formula
  return (fast_sine(Amp, th));
}
//--------------------------------------------------------------------------------//


//--------------------------------Fast RSS----------------------------------------//
int fastRSS(int a, int b) {
  if (a == 0 && b == 0) {
    return (0);
  }
  int min, max, temp1, temp2;
  byte clevel;
  if (a < 0) {
    a = -a;
  }
  if (b < 0) {
    b = -b;
  }
  clevel = 0;
  if (a > b) {
    max = a;
    min = b;
  } else {
    max = b;
    min = a;
  }

  if (max > (min + min + min)) {
    return max;
  } else {
    temp1 = min >> 3;
    if (temp1 == 0) {
      temp1 = 1;
    }
    temp2 = min;
    while (temp2 < max) {
      temp2 = temp2 + temp1;
      clevel = clevel + 1;
    }
    temp2 = RSSdata[clevel]; temp1 = temp1 >> 1;
    for (int i = 0; i < temp2; i++) {
      max = max + temp1;
    }
    return (max);
  }
}
//--------------------------------------------------------------------------------//


//--------------------------------binning-----------------------------------------//
void binning_4(int N, int *dataIn, int *dataOut) {
  for (uint16_t i = 0; i < (N >> 2); i++) {
    if (i > (N >> 2)) {                                 // Average 8 datapoints
      dataOut[i] = (dataIn[4 * i - (N >> 4) * 7] +
                    dataIn[4 * i - (N >> 4) * 7 + 1] +
                    dataIn[4 * i - (N >> 4) * 7 + 2] +
                    dataIn[4 * i - (N >> 4) * 7 + 3] +
                    dataIn[4 * i - (N >> 4) * 7 + 4] +
                    dataIn[4 * i - (N >> 4) * 7 + 5] +
                    dataIn[4 * i - (N >> 4) * 7 + 6] +
                    dataIn[4 * i - (N >> 4) * 7 + 7]) >> 3;
      //Serial.print(4 * i - (N >> 4)*7 + 7);
    } else if (i > (N >> 3)) {                          // Average 4 datapoints
      dataOut[i] = (dataIn[3 * i - (N >> 4) * 3] +
                    dataIn[3 * i - (N >> 4) * 3 + 1] +
                    dataIn[3 * i - (N >> 4) * 3 + 2] +
                    dataIn[3 * i - (N >> 4) * 3 + 3]) >> 2;
      //Serial.print(3 * i - (N >> 4)*3 + 3);
    } else if (i > (N >> 4)) {                          // Average 2 datapoints
      dataOut[i] = (dataIn[2 * i - (N >> 4)] +
                    dataIn[2 * i - (N >> 4) + 1]) >> 1;
      //Serial.print(2 * i - (N >> 4) + 1);
    } else {                                            // "Average" 1 datapoint
      dataOut[i] = dataIn[i];
      //Serial.print(i);
    }
    //Serial.print(" ");
    //Serial.print(dataOut[i]);
    //Serial.println(" ");
  }
}


void binning_9(int N, int *dataIn, int *dataOut) {
  memset(dataOut,0,sizeof(dataOut));
  for (uint16_t i = 0; i < 9; i++) {
    for (uint8_t j = 0; j <= i; j++){                                // Average 8 datapoints
        dataOut[i] =+ dataIn[(1<<i)-1+j];
        //Serial.print(4 * i - (N >> 4)*7 + 7);
    }
    dataOut[i] >> i;
      //Serial.print(" ");
      //Serial.print(dataOut[i]);
      //Serial.println(" ");
    }
}
//--------------------------------------------------------------------------------//
