//-----------------------------------------------------Libraries and initialization
#include <synth.h>
synth sinte;
#include "FastLED.h"
#define DATA_PIN 5
CRGB leds[5];
#include <Encoder.h>
Encoder encoder(3,2);
//-----------------------------------------------------Pinouts
//-----------------------------------------------------All digital inputs are pulled high (inverse logic)
const int pushEncoderPin = 4;
const int switchClockPin = 13;
const int switchWaveformPin = 12;
const int pushEnvelope = 10;
const int triggerPin[4] = {6,7,8,9};
const int potInterval[4] = {7,5,3,1};//----------------Analog inputs
const int potNote[4] = {6,2,4,0};
//---------------------------------------------------------------------------Global Variables
const float tunning[2][12]={
  {32.7,34.65,36.71,38.89,41.2,43.65,46.25,49,51.91,55,58.27,61.74},//-------440
  {32.11,34.02,36.04,38.18,40.45,42.86,45.41,48.11,50.97,54,57.21,60.61}//---432
};
//---------------------------------------------------------------------------SCALES
const int  chromatic[12]={0,1,2,3,4,5,6,7,8,9,10,11};//{0,0,0,0,0,0,0,0,0,0,0};
const int  major[7]={0,2,4,5,7,9,11};//{2,2,1,2,2,2,1};
const int  minor[7]={0,2,3,5,7,8,10};//{2,1,2,2,1,2,2};
const int  blues[6]={0,3,5,6,7,10};//{3,2,1,1,3,2};
const int  majorPentatonic[5]={0,2,4,7,9};//{2,2,3,2,3};
const int  minorPentatonic[5]={0,3,5,7,10};//{3,2,2,3,2};
const int  ukranianGypsy[6]={0,2,3,6,7,9};//{2,1,3,1,2,1,2}
//---------------------------------------------------------------------------Functional variables
const int interval[14] = {0,38,77,125,250,500,750,1000,2000,3000,4000,8000,16000,32000};
long previousMillisTrigger[4], previousMillis[4], previousMillisDebounce[4],previousMillisLeds[4];
int note[4], noteInterval[4];
bool pushEncoder, clock, waveform, envelope, trigger[4];
bool toggleEnvelope[4]={1,1,1,1};
int octave[4] = {0,0,0,0};
int ledState[5]={0,0,0,0,0};//-----------------------------------------------LEDs
int R[7]={5,0,5,0,5,0,5};
int G[7]={5,5,0,0,4,5,0};
int B[7]={5,0,0,5,0,5,5};
int scale=0;//---------------------------------------------------------------Encoder variables
int length=80; 
int modulation = 64;
int newPositionScale, newPositionLength, newPositionModulation,toggleEncoder;
int oldPositionScale = -1;
int oldPositionLength = -1;
int oldPositionModulation = -1;

void setup(){
//  Serial.begin(115200);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds,5);
  sinte.begin();
  sinte.setupVoice(0, SINE, note[0], ENVELOPE0, length, modulation);
  sinte.setupVoice(1, SINE, 0, ENVELOPE0, length, modulation);
  sinte.setupVoice(2, SINE, 0, ENVELOPE0, length, modulation);
  sinte.setupVoice(3, SINE, 0, ENVELOPE0, length, modulation);
  encoder.write(320);
  leds[0].setRGB(R[0],G[0],B[0]);
  FastLED.show();
  for(int i=4; i<14; i++){
    if(i!=5&&i!=11)pinMode(i, INPUT_PULLUP);
  }
}

void loop(){
 readEncoder();
 waveform=digitalRead(switchWaveformPin);
 leds[0].setRGB(R[scale],G[scale],B[scale]);
 envelope=digitalRead(pushEnvelope);
 for (int i=0; i<4;i++){
  if(waveform)sinte.setWave(i,SINE);
  if(!waveform)sinte.setWave(i,SAW);
  sinte.setLength(i,length);
  sinte.setMod(i,modulation);
  trigger[i]=digitalRead(triggerPin[i]);
  noteInterval[i]=interval[map(analogRead(potInterval[i]),0,1023,0,13)];
  clock = digitalRead(switchClockPin);
  switch (scale){
    case 0:
      note[i]=map(analogRead(potNote[i]),0,1023,71,0);
      if(note[i]<=11){note[i] = note[i]; octave[i]=0;}
      if(note[i]>11&&note[i]<=23){note[i] = note[i]-12; octave[i]=1;}
      if(note[i]>23&&note[i]<=35){note[i] = note[i]-24; octave[i]=2;}
      if(note[i]>35&&note[i]<=47){note[i] = note[i]-36; octave[i]=3;}
      if(note[i]>47&&note[i]<=59){note[i] = note[i]-48; octave[i]=4;}
      if(note[i]>59&&note[i]<=71){note[i] = note[i]-60; octave[i]=5;}
      sinte.setFrequency(i,(tunning[0][chromatic[note[i]]])*pow(2,octave[i]));
      break;
    case 1:
      note[i]=map(analogRead(potNote[i]),0,1023,41,0);
      if(note[i]<=6){;note[i]=note[i];octave[i]=0;}
      if(note[i]>6&&note[i]<=13){;note[i]=note[i]-7;octave[i]=1;}
      if(note[i]>13&&note[i]<=20){;note[i]=note[i]-14;octave[i]=2;}
      if(note[i]>20&&note[i]<=27){;note[i]=note[i]-21;octave[i]=3;}
      if(note[i]>27&&note[i]<=34){;note[i]=note[i]-28;octave[i]=4;}
      if(note[i]>34&&note[i]<=41){;note[i]=note[i]-35;octave[i]=5;}
      sinte.setFrequency(i,(tunning[0][major[note[i]]])*pow(2,octave[i]));    
      Serial.println(major[note[0]]);
      break;
    case 2:
      note[i]=map(analogRead(potNote[i]),0,1023,41,0);
      if(note[i]<=6){;note[i]=note[i];octave[i]=0;}
      if(note[i]>6&&note[i]<=13){;note[i]=note[i]-7;octave[i]=1;}
      if(note[i]>13&&note[i]<=20){;note[i]=note[i]-14;octave[i]=2;}
      if(note[i]>20&&note[i]<=27){;note[i]=note[i]-21;octave[i]=3;}
      if(note[i]>27&&note[i]<=34){;note[i]=note[i]-28;octave[i]=4;}
      if(note[i]>34&&note[i]<=41){;note[i]=note[i]-35;octave[i]=5;}
      sinte.setFrequency(i,(tunning[0][minor[note[i]]])*pow(2,octave[i]));    
      break;
    case 3:
      note[i]=map(analogRead(potNote[i]),0,1023,35,0);
      if(note[i]<=5){;note[i]=note[i];octave[i]=0;}
      if(note[i]>5&&note[i]<=11){;note[i]=note[i]-6;octave[i]=1;}
      if(note[i]>11&&note[i]<=17){;note[i]=note[i]-12;octave[i]=2;}
      if(note[i]>17&&note[i]<=23){;note[i]=note[i]-18;octave[i]=3;}
      if(note[i]>23&&note[i]<=29){;note[i]=note[i]-24;octave[i]=4;}
      if(note[i]>29&&note[i]<=35){;note[i]=note[i]-30;octave[i]=5;}
      sinte.setFrequency(i,(tunning[0][blues[note[i]]])*pow(2,octave[i]));
      break;
    case 4:
      note[i]=map(analogRead(potNote[i]),0,1023,29,0);
      if(note[i]<=4){;note[i]=note[i];octave[i]=0;}
      if(note[i]>4&&note[i]<=9){;note[i]=note[i]-5;octave[i]=1;}
      if(note[i]>9&&note[i]<=14){;note[i]=note[i]-10;octave[i]=2;}
      if(note[i]>14&&note[i]<=19){;note[i]=note[i]-15;octave[i]=3;}
      if(note[i]>19&&note[i]<=24){;note[i]=note[i]-20;octave[i]=4;}
      if(note[i]>24&&note[i]<=29){;note[i]=note[i]-25;octave[i]=5;}
      sinte.setFrequency(i,(tunning[0][majorPentatonic[note[i]]])*pow(2,octave[i]));
      break;
    case 5:
      note[i]=map(analogRead(potNote[i]),0,1023,29,0);
      if(note[i]<=4){;note[i]=note[i];octave[i]=0;}
      if(note[i]>4&&note[i]<=9){;note[i]=note[i]-5;octave[i]=1;}
      if(note[i]>9&&note[i]<=14){;note[i]=note[i]-10;octave[i]=2;}
      if(note[i]>14&&note[i]<=19){;note[i]=note[i]-15;octave[i]=3;}
      if(note[i]>19&&note[i]<=24){;note[i]=note[i]-20;octave[i]=4;}
      if(note[i]>24&&note[i]<=29){;note[i]=minorPentatonic[note[i]-25];octave[i]=5;}
      sinte.setFrequency(i,(tunning[0][minorPentatonic[note[i]]])*pow(2,octave[i]));
      break;
    case 6:
      note[i]=map(analogRead(potNote[i]),0,1023,35,0);
      if(note[i]<=5){;note[i]=note[i];octave[i]=0;}
      if(note[i]>5&&note[i]<=11){;note[i]=note[i]-6;octave[i]=1;}
      if(note[i]>11&&note[i]<=17){;note[i]=note[i]-12;octave[i]=2;}
      if(note[i]>17&&note[i]<=23){;note[i]=note[i]-18;octave[i]=3;}
      if(note[i]>23&&note[i]<=29){;note[i]=note[i]-24;octave[i]=4;}
      if(note[i]>29&&note[i]<=35){;note[i]=note[i]-30;octave[i]=5;}
      sinte.setFrequency(i,(tunning[0][ukranianGypsy[note[i]]])*pow(2,octave[i]));
      break;
    case 7: 
      break;
  }
  if(!trigger[i]){
    sinte.trigger(i);
    previousMillisTrigger[i]=millis();
    leds[i+1].setRGB(R[scale],G[scale],B[scale]);
    if(ledState[i]==0){
      FastLED.show();
      ledState[i]=1;
    }
  }
  if(!envelope&&!trigger[i]){
    if(toggleEnvelope[i]==0){
      sinte.setEnvelope(i,ENVELOPE3);
      if(millis()-previousMillisDebounce[i]>500){
        toggleEnvelope[i]=1;
        previousMillisDebounce[i]=millis();
      }    
    }
    if(toggleEnvelope[i]==1){
      sinte.setEnvelope(i,ENVELOPE1);
      if(millis()-previousMillisDebounce[i]>500){
        toggleEnvelope[i]=0;
        previousMillisDebounce[i]=millis();
      }
    }
  }
  if(clock){
    if(millis()-previousMillisTrigger[i]>noteInterval[i]+(i*i)){
      sinte.trigger(i);
      previousMillisTrigger[i]=millis();
      leds[i+1].setRGB(R[scale],G[scale],B[scale]);
      if(ledState[i]==0){
        FastLED.show();
        ledState[i]=1;}
    }
  }
  if(millis()-previousMillisLeds[i]>90){
    previousMillisLeds[i]=millis();
    leds[i+1].setHSV(0,0,0);
    if(ledState[i]==1&&trigger[i]){
      FastLED.show();
      ledState[i]=0;}
  }
  }
  
 }
