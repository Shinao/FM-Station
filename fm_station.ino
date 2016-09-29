/*
 FM Station 
*/

#include "SevSeg.h"
#include "FMTX.h"

SevSeg        myDisplay;
unsigned long timer;
float         fm_freq = 80;
float         prev_fm_freq = fm_freq;

int potentiometerPin = 0;

char freq_formatted[10] = {'0', '0', '0', '0', 0};

void setup()
{
  Serial.begin(9600);

  int displayType = COMMON_CATHODE; //Your display is either common cathode or common anode
  
  //This pinout is for a regular display
   //Declare what pins are connected to the digits
   int digit1 = 2; //Pin 12 on my 4 digit display
   int digit2 = 3; //Pin 9 on my 4 digit display
   int digit3 = 4; //Pin 8 on my 4 digit display
   int digit4 = 5; //Pin 6 on my 4 digit display
   
   //Declare what pins are connected to the segments
   int segA = 6; //Pin 11 on my 4 digit display
   int segB = 7; //Pin 7 on my 4 digit display
   int segC = 8; //Pin 4 on my 4 digit display
   int segD = 9; //Pin 2 on my 4 digit display
   int segE = 10; //Pin 1 on my 4 digit display
   int segF = 11; //Pin 10 on my 4 digit display
   int segG = 12; //Pin 5 on my 4 digit display
   int segDP= 13; //Pin 3 on my 4 digit display

  int numberOfDigits = 4; //Do you have a 1, 2 or 4 digit display?

  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  myDisplay.SetBrightness(100); //Set the display to 100% brightness level

  timer = millis();
  
  pinMode(potentiometerPin, OUTPUT);

  fmtx_init(fm_freq, EUROPE); 
  Serial.print("FMTX initialized - Channel:");
  Serial.print(fm_freq, 1);
  Serial.println("MHz");
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop()
{
  myDisplay.DisplayString(freq_formatted, 4);
  
  // Only update if frequency changed
  if (prev_fm_freq != fm_freq)
  {
    fmtx_set_freq(ch);
    prev_fm_freq = fm_freq;
  }

  // Check new frequency if 100ms has elapsed to avoid too many calls
  if (millis() - timer >= 100)
  {
    int angle = analogRead(potentiometerPin);
    int freq = map(angle, 0, 1023, 800, 1080); // Convert angle to range 80-108MHz
    
    sprintf(freq_formatted, "%04d", freq); // Lead with one 0 if possible

    // Convert to float for FMTX
    fm_freq = (float) freq / 10.0; 
    
    Serial.println(freq_formatted);
    timer = millis();
  }

  delay(5);
}
