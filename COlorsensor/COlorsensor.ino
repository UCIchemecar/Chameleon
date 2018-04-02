#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */
   
/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(9600);
  
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  
  // Now we're ready to get readings!
}

void loop(void) {
  
  uint16_t r, g, b, c, colorTemp, lux;
  uint16_t rl, gl, bl, cl;
  static int dr, dg, db, dc;
  static unsigned long tInjection, TFinish1, TFinish2,TFinish3, Ttotal1 = 0, Ttotal2, Ttotal3;
  static boolean Inject, Finish;
  static int dcount , fcount1, fcount2, fcount3 = 0;
  rl = r;
  bl = b;
  gl = g;
  cl = c;

  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

    dr = r - rl;
  dg = g - gl;
  db = b - bl;
  dc = c - cl;
  


  if(!Inject && dr < -50)
  {
    tInjection = millis();
    Inject = true;
    dcount ++;
    //if(dcount > 5)
    //{
    //  tInjection = millis();
    //  Inject = true;
    //}
  }
  if(!Inject && colorTemp < 14000)
  {
    dcount = 0;
  }
  if(Inject && dr < 1 && dr > -1 && dg > -1 && dg < 1 && Ttotal1 < 2000)
  {
    //TFinish = millis();
    //Ttotal = (TFinish - tInjection);
    fcount1 ++;
    TFinish1 = millis();
    if(fcount1 > 4)
    {
      
      Ttotal1 = (TFinish1 - tInjection);
    }
  }else{
    fcount1 = 0;
  }
  if(Inject && db < 1 && db > -1 && Ttotal2 < 2000)
  {
    fcount2 ++;
    TFinish2 = millis();
    if(fcount2 > 4)
    {
      
      Ttotal2 = (TFinish2 - tInjection);
    }
  }else{
    fcount2 = 0;
  }
if(Inject && dg < 1 && dg > -1 && Ttotal2 < 2000)
  {
    fcount3 ++;
    TFinish2 = millis();
    if(fcount3 > 4)
    {
      
      Ttotal3 = (TFinish3 - tInjection);
    }
  }else{
    fcount3 = 0;
  }
    Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
//  Serial.println(" ");

//
//  Serial.print("RL: "); Serial.print(rl, DEC); Serial.print(" ");
//  Serial.print("GL: "); Serial.print(gl, DEC); Serial.print(" ");
//  Serial.print("BL: "); Serial.print(bl, DEC); Serial.print(" ");
//  Serial.print("CL: "); Serial.print(cl, DEC); Serial.print(" ");
//  Serial.println(" ");


  Serial.print("DR: "); Serial.print(dr, DEC); Serial.print(" ");
  Serial.print("DG: "); Serial.print(dg, DEC); Serial.print(" ");
  Serial.print("DB: "); Serial.print(db, DEC); Serial.print(" ");
  Serial.print("DC: "); Serial.print(dc, DEC); Serial.print(" ");
  Serial.print("Inject: "); Serial.print(tInjection); Serial.print(" Total: "); Serial.print(Ttotal1); Serial.print(" Total2: "); Serial.print(Ttotal2);
  Serial.print(" Total3: "); Serial.print(Ttotal3);
  Serial.println(" ");
}
