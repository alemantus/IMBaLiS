#include "arduinoFFT.h"

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

//Example data:
int data[64] = {14, 30, 35, 34, 34, 40, 46, 45, 30, 4,  -26,  -48,  -55,  -49,  -37,
                -28,  -24,  -22,  -13,  6,  32, 55, 65, 57, 38, 17, 1,  -6, -11,  -19,  -34,
                -51,  -61,  -56,  -35,  -7, 18, 32, 35, 34, 35, 41, 46, 43, 26, -2, -31,  -50,
                -55,  -47,  -35,  -27,  -24,  -21,  -10,  11, 37, 58, 64, 55, 34, 13, -1, -7
               };



//---------------------------------------------------------------------------//
byte sine_data [91] =
{
  0,
  4,    9,    13,   18,   22,   27,   31,   35,   40,   44,
  49,   53,   57,   62,   66,   70,   75,   79,   83,   87,
  91,   96,   100,  104,  108,  112,  116,  120,  124,  127,
  131,  135,  139,  143,  146,  150,  153,  157,  160,  164,
  167,  171,  174,  177,  180,  183,  186,  189,  192,  195,       //Paste this at top of program
  198,  201,  204,  206,  209,  211,  214,  216,  219,  221,
  223,  225,  227,  229,  231,  233,  235,  236,  238,  240,
  241,  243,  244,  245,  246,  247,  248,  249,  250,  251,
  252,  253,  253,  254,  254,  254,  255,  255,  255,  255
};

byte isin_data[128] =
{ 0,  1,   3,   4,   5,   6,   8,   9,   10,  11,  13,  14,  15,  17,  18,  19,  20,
  22,  23,  24,  26,  27,  28,  29,  31,  32,  33,  35,  36,  37,  39,  40,  41,  42,
  44,  45,  46,  48,  49,  50,  52,  53,  54,  56,  57,  59,  60,  61,  63,  64,  65,
  67,  68,  70,  71,  72,  74,  75,  77,  78,  80,  81,  82,  84,  85,  87,  88,  90,
  91,  93,  94,  96,  97,  99,  100, 102, 104, 105, 107, 108, 110, 112, 113, 115, 117,
  118, 120, 122, 124, 125, 127, 129, 131, 133, 134, 136, 138, 140, 142, 144, 146, 148,
  150, 152, 155, 157, 159, 161, 164, 166, 169, 171, 174, 176, 179, 182, 185, 188, 191,
  195, 198, 202, 206, 210, 215, 221, 227, 236
};
unsigned int Pow2[14] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
byte RSSdata[20] = {7, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2};

float f_peaks[50]; // top 5 frequencies peaks in descending order
//---------------------------------------------------------------------------//

long double startTime = 0;
long double stopTime = 0;
double totalTimeEasyFFT = 0;
double totalTimeArduinoFFT = 0;
double totalTimeFastFFT = 0;
const uint16_t samples = 256; //This value MUST ALWAYS be a power of 2
const double signalFrequency = 500;
const double samplingFrequency = 5000;
const uint8_t amplitude = 100;
int latch = 1;
int debugMessages = 1;
/*
  These are the input and output vectors
  Input vectors receive computed results from FFT
*/
double vReal[samples];
double vImag[samples];

#define twoPi 6.2831

void setup() {
  Serial.begin(115200);
}


void loop() {
  if (latch) {
    double cycles = (((samples - 1) * signalFrequency) / samplingFrequency); //Number of signal cycles that the sampling will read
    for (uint16_t i = 0; i < samples; i++) {
      vReal[i] = int((amplitude * (sin((i * (twoPi * cycles)) / samples))) / 2.0);/* Build data with positive and negative values*/
      //vReal[i] = uint8_t((amplitude * (sin((i * (twoPi * cycles)) / samples) + 1.0)) / 2.0);/* Build data displaced on the Y axis to include only positive values*/
      vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
    }


    if (!debugMessages) {
      for (int i; i < samples; i++) {
        Serial.println(int(vReal[i]));
      }
    }
    float f = 0;
    startTime = micros();
    //f = Approx_FFT(int(vReal), 256, samplingFrequency);
    //Serial.println(f);
    stopTime = micros();

    totalTimeFastFFT = stopTime - startTime;
    if (debugMessages) {
      Serial.print("TimeFastFFT: Most dominant frequency: ");
      Serial.println(f);
      Serial.print("TimeFastFFT: Total time is [uS] ");
      Serial.println(totalTimeFastFFT);
    }

    startTime = micros();
    easyFFT(int(vReal), 256, samplingFrequency);      //to get top five value of frequencies of X having 64 sample at 100Hz sampling
    stopTime = micros();

    totalTimeEasyFFT = stopTime - startTime;
    if (debugMessages) {
      Serial.print("EasyFFT: Most dominant frequency: ");
      Serial.println(f_peaks[0] * 2);
      Serial.print("EasyFFT: Total time is [uS] ");
      Serial.println(totalTimeEasyFFT);
    }


    double x = 0;
    startTime = micros();

    //Serial.println("Data:");
    //PrintVector(vReal, samples, SCL_TIME);
    FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
    //Serial.println("Weighed data:");
    //PrintVector(vReal, samples, SCL_TIME);
    FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
    //Serial.println("Computed Real values:");
    //PrintVector(vReal, samples, SCL_INDEX);
    //Serial.println("Computed Imaginary values:");
    //PrintVector(vImag, samples, SCL_INDEX);
    FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
    //Serial.println("Computed magnitudes:");
    //PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
    x = FFT.MajorPeak(vReal, samples, samplingFrequency);

    stopTime = micros();
    totalTimeArduinoFFT = stopTime - startTime;
    if (debugMessages) {
      Serial.print("ArduinoFFT: Most dominant frequency: ");
      Serial.println(x, 6);
      Serial.print("ArduinoFFT: Total time is [uS] ");
      Serial.println(totalTimeArduinoFFT);
    }



    latch = 0;
  }



  /*
    after ruing above FFT(), frequencies available at f_peaks[0],f_peaks[1],f_peaks[2],f_peaks[3],f_peaks[4],
  */
}



float Approx_FFT(int in[], int N, float Frequency)
{
  int a, c1, f, o, x, data_max, data_min = 0;
  long data_avg, data_mag, temp11;
  byte scale, check = 0;

  data_max = 0;
  data_avg = 0;
  data_min = 0;

  for (int i = 0; i < 12; i++)          //calculating the levels
  {
    if (Pow2[i] <= N) {
      o = i;
    }
  }
  a = Pow2[o];
  int out_r[a];   //real part of transform
  int out_im[a];  //imaginory part of transform

  for (int i = 0; i < a; i++)         //getting min max and average for scalling
  { out_r[i] = 0; out_r[i] = 0;
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

  for (int i; i < 128; i++)         //scalling data  from +512 to -512

    if (data_mag > 1024)
    { while (temp11 > 1024)
      { temp11 = temp11 >> 1;
        scale = scale + 1;
      }
    }

  if (data_mag < 1024)
  { while (temp11 < 1024)
    { temp11 = temp11 << 1;
      scale = scale + 1;
    }
  }


  if (data_mag > 1024)
  {
    for (int i = 0; i < a; i++)
    { in[i] = in[i] - data_avg;
      in[i] = in[i] >> scale;
    }
    scale = 128 - scale;
  }

  if (data_mag < 1024)
  { scale = scale - 1;
    for (int i = 0; i < a; i++)
    {
      in[i] = in[i] - data_avg;
      in[i] = in[i] << scale;
    }

    scale = 128 + scale;
  }


  x = 0;
  for (int b = 0; b < o; b++)              // bit reversal order stored in im_out array
  {
    c1 = Pow2[b];
    f = Pow2[o] / (c1 + c1);
    for (int j = 0; j < c1; j++)
    {
      x = x + 1;
      out_im[x] = out_im[j] + f;
    }
  }


  for (int i = 0; i < a; i++)     // update input array as per bit reverse order
  {
    out_r[i] = in[out_im[i]];
    out_im[i] = 0;
  }


  int i10, i11, n1, tr, ti;
  float e;
  int c, s, temp4;
  for (int i = 0; i < o; i++)                             //fft
  {
    i10 = Pow2[i];            // overall values of sine/cosine
    i11 = Pow2[o] / Pow2[i + 1]; // loop with similar sine cosine
    e = 1024 / Pow2[i + 1]; //1024 is equivalent to 360 deg
    e = 0 - e;
    n1 = 0;

    for (int j = 0; j < i10; j++)
    {
      c = e * j; //c is angle as where 1024 unit is 360 deg
      while (c < 0) {
        c = c + 1024;
      }
      while (c > 1024) {
        c = c - 1024;
      }

      n1 = j;

      for (int k = 0; k < i11; k++)
      {
        temp4 = i10 + n1;
        if (c == 0)   {
          tr = out_r[temp4];
          ti = out_im[temp4];
        }
        else if (c == 256) {
          tr = -out_im[temp4];
          ti = out_r[temp4];
        }
        else if (c == 512) {
          tr = -out_r[temp4];
          ti = -out_im[temp4];
        }
        else if (c == 768) {
          tr = out_im[temp4];
          ti = -out_r[temp4];
        }
        else if (c == 1024) {
          tr = out_r[temp4];
          ti = out_im[temp4];
        }
        else {
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
      for (int i = 0; i < a; i++)
      {
        out_r[i] = out_r[i] >> 1;
        out_im[i] = out_im[i] >> 1;
      }
      check = 0;
      scale = scale - 1;             // tracking overall scalling of input data
    }

  }


  if (scale > 128)
  { scale = scale - 128;
    for (int i = 0; i < a; i++)
    { out_r[i] = out_r[i] >> scale;
      out_im[i] = out_im[i] >> scale;
    }
    scale = 0;
  }                                                   // revers all scalling we done till here,
  else {
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

  //---> here onward out_r contains amplitude and our_in conntains frequency (Hz)
  int fout, fm, fstp;
  float fstep;
  fstep = Frequency / N;
  fstp = fstep;
  fout = 0; fm = 0;

  for (int i = 1; i < Pow2[o - 1]; i++)      // getting amplitude from compex number
  {
    out_r[i] = fastRSS(out_r[i], out_im[i]);
    // Approx RSS function used to calculated magnitude quickly

    out_im[i] = out_im[i - 1] + fstp;
    if (fout < out_r[i]) {
      fm = i;
      fout = out_r[i];
    }
    /*
      // un comment to print Amplitudes (1st value (offset) is not printed)
      Serial.print(out_r[i]); Serial.print("\t");
      Serial.print("*2^");Serial.println(scale);
    */
  }


  float fa, fb, fc;
  fa = out_r[fm - 1];
  fb = out_r[fm];
  fc = out_r[fm + 1];
  fstep = (fa * (fm - 1) + fb * fm + fc * (fm + 1)) / (fa + fb + fc);

  return (fstep * Frequency / N);
}

//---------------------------------fast sine/cosine---------------------------------------//

int fast_sine(int Amp, int th)
{
  int temp3, m1, m2;
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
  }
  else if (quad == 2) {
    th = th - 512;
  }
  else if (quad == 3) {
    th = 1024 - th;
  }

  temp1 = 0;
  temp2 = 128;    //2 multiple
  m1 = 0;
  m2 = Amp;

  temp3 = (m1 + m2) >> 1;
  Amp = temp3;
  for (int i = 0; i < accuracy; i++)
  { test = (temp1 + temp2) >> 1;
    temp3 = temp3 >> 1;
    if (th > isin_data[test]) {
      temp1 = test;
      Amp = Amp + temp3;
      m1 = Amp;
    }
    else if (th < isin_data[test]) {
      temp2 = test;
      Amp = Amp - temp3;
      m2 = Amp;
    }
  }

  if (quad == 2) {
    Amp = 0 - Amp;
  }
  else if (quad == 3) {
    Amp = 0 - Amp;
  }
  return (Amp);
}

int fast_cosine(int Amp, int th)
{
  th = 256 - th; //cos th = sin (90-th) formula
  return (fast_sine(Amp, th));
}

//--------------------------------------------------------------------------------//


//--------------------------------Fast RSS----------------------------------------//
int fastRSS(int a, int b)
{ if (a == 0 && b == 0) {
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

  if (max > (min + min + min))
  {
    return max;
  }
  else
  {
    temp1 = min >> 3; if (temp1 == 0) {
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
  //-----------------------------FFT Function----------------------------------------------//

  float easyFFT(int in[], int N, float Frequency)
  {
    /*
      Code to perform FFT on arduino,
      setup:
      paste sine_data [91] at top of program [global variable], paste FFT function at end of program
      Term:
      1. in[]     : Data array,
      2. N        : Number of sample (recommended sample size 2,4,8,16,32,64,128...)
      3. Frequency: sampling frequency required as input (Hz)

      If sample size is not in power of 2 it will be clipped to lower side of number.
      i.e, for 150 number of samples, code will consider first 128 sample, remaining sample  will be omitted.
      For Arduino nano, FFT of more than 128 sample not possible due to mamory limitation (64 recomended)
      For higher Number of sample may arise Mamory related issue,
      Code by ABHILASH
      Contact: abhilashpatel121@gmail.com
      Documentation:https://www.instructables.com/member/abhilash_patel/instructables/
      2/3/2021: change data type of N from float to int for >=256 samples
    */

    unsigned int data[13] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};
    int a, c1, f, o, x;
    a = N;

    for (int i = 0; i < 12; i++)          //calculating the levels
    {
      if (data[i] <= a) {
        o = i;
      }
    }

    int in_ps[data[o]] = {};   //input for sequencing
    float out_r[data[o]] = {}; //real part of transform
    float out_im[data[o]] = {}; //imaginory part of transform

    x = 0;
    for (int b = 0; b < o; b++)              // bit reversal
    {
      c1 = data[b];
      f = data[o] / (c1 + c1);
      for (int j = 0; j < c1; j++)
      {
        x = x + 1;
        in_ps[x] = in_ps[j] + f;
      }
    }


    for (int i = 0; i < data[o]; i++)     // update input array as per bit reverse order
    {
      if (in_ps[i] < a)
      {
        out_r[i] = in[in_ps[i]];
      }
      if (in_ps[i] > a)
      {
        out_r[i] = in[in_ps[i] - a];
      }
    }


    int i10, i11, n1;
    float e, c, s, tr, ti;

    for (int i = 0; i < o; i++)                             //fft
    {
      i10 = data[i];            // overall values of sine/cosine  :
      i11 = data[o] / data[i + 1]; // loop with similar sine cosine:
      e = 360 / data[i + 1];
      e = 0 - e;
      n1 = 0;

      for (int j = 0; j < i10; j++)
      {
        c = cosine(e * j);
        s = sine(e * j);
        n1 = j;

        for (int k = 0; k < i11; k++)
        {
          tr = c * out_r[i10 + n1] - s * out_im[i10 + n1];
          ti = s * out_r[i10 + n1] + c * out_im[i10 + n1];

          out_r[n1 + i10] = out_r[n1] - tr;
          out_r[n1] = out_r[n1] + tr;

          out_im[n1 + i10] = out_im[n1] - ti;
          out_im[n1] = out_im[n1] + ti;

          n1 = n1 + i10 + i10;
        }
      }
    }

    /*
      for(int i=0;i<data[o];i++)
      {
      Serial.print(out_r[i]);
      Serial.print("\t");                                     // un comment to print RAW o/p
      Serial.print(out_im[i]); Serial.println("i");
      }
    */


    //---> here onward out_r contains amplitude and our_in conntains frequency (Hz)
    for (int i = 0; i < data[o - 1]; i++)      // getting amplitude from compex number
    {
      out_r[i] = sqrt(out_r[i] * out_r[i] + out_im[i] * out_im[i]); // to  increase the speed delete sqrt
      out_im[i] = i * Frequency / N;
      /*
        Serial.print(out_im[i]); Serial.print("Hz");
        Serial.print("\t");                            // un comment to print freuency bin
        Serial.println(out_r[i]);
      */
    }




    x = 0;     // peak detection
    for (int i = 1; i < data[o - 1] - 1; i++)
    {
      if (out_r[i] > out_r[i - 1] && out_r[i] > out_r[i + 1])
      { in_ps[x] = i;  //in_ps array used for storage of peak number
        x = x + 1;
      }
    }


    s = 0;
    c = 0;
    for (int i = 0; i < x; i++)      // re arraange as per magnitude
    {
      for (int j = c; j < x; j++)
      {
        if (out_r[in_ps[i]] < out_r[in_ps[j]])
        { s = in_ps[i];
          in_ps[i] = in_ps[j];
          in_ps[j] = s;
        }
      }
      c = c + 1;
    }



    for (int i = 0; i < 5; i++) // updating f_peak array (global variable)with descending order
    {
      f_peaks[i] = out_im[in_ps[i]];
    }



  }


  float sine(int i)
  {
    int j = i;
    float out;
    while (j < 0) {
      j = j + 360;
    }
    while (j > 360) {
      j = j - 360;
    }
    if (j > -1   && j < 91) {
      out = sine_data[j];
    }
    else if (j > 90  && j < 181) {
      out = sine_data[180 - j];
    }
    else if (j > 180 && j < 271) {
      out = -sine_data[j - 180];
    }
    else if (j > 270 && j < 361) {
      out = -sine_data[360 - j];
    }
    return (out / 255);
  }

  float cosine(int i)
  {
    int j = i;
    float out;
    while (j < 0) {
      j = j + 360;
    }
    while (j > 360) {
      j = j - 360;
    }
    if (j > -1   && j < 91) {
      out = sine_data[90 - j];
    }
    else if (j > 90  && j < 181) {
      out = -sine_data[j - 90];
    }
    else if (j > 180 && j < 271) {
      out = -sine_data[270 - j];
    }
    else if (j > 270 && j < 361) {
      out = sine_data[j - 270];
    }
    return (out / 255);
  }

  //------------------------------------------------------------------------------------//
