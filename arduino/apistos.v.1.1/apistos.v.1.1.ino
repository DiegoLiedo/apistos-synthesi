// Arduino synth library absolute frequency example

//Hardware connections:

//                             +10µF
//PIN 11 ---[470]---[ 1k ]--+---||--->> Audio out
//                 |         |
//                 ===473    === 10nF
//                 |         |
//                GND       GND


#include <synth.h>

synth sinte;    //-Make a synth
const int waveformPin = 13;
const int clockPin = 12;
const int leds[3] = {4, 5, 6};
const int botPin[3] = {8, 9, 10};
const int scalePin[3] = {2, 3, 7};
float freq0, freq1, freq2, pot3, pot4, pot5, pot6, pot7;
const int potesFreq[3]={16,14,18};
const int potesIntervalo[3]={15,17,19};
const int poteModulacion=20;
const int potelength=21;
int intervalo[3]={0,0,0};
long previousMillis[3] = {0, 0, 0};
float freq[3];
float interval[3];
bool scale[3];
bool buttonState[3];
bool waveform=1;
bool Clock = 1;
unsigned int rythm[10] = {0, 77, 125, 250, 500, 1000, 2000, 4000, 8000,16000};
float penta[36] = {65.41, 69.3, 77.78, 98, 103.83, 110, 130.81, 138.59, 155.56, 196, 207.65, 233.08, 261.63, 277.18, 311.13, 392, 415.3, 466.16, 523.25, 554.37, 622.25, 783.99, 830.61, 932.33, 1046.5, 1108.73, 1244.51, 1567.98, 1661.22, 1864.66, 2093, 2217.46, 2489.02, 3135.96, 3322.44, 3729.31};
float cromatica[36] = {65.41, 73.42, 82.41, 87.31, 98, 110, 123.47, 130.81, 146.83, 164.81, 174.61, 196, 220, 246.94, 261.63, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25, 587.33, 659.25, 698.46, 783.99, 880, 987.77, 1046.5, 1174.66, 1318.51, 1396.91, 1567.98, 1760, 1975.53, 2093};
float frecuencia[3];


void setup() {
//  Serial.begin(115200);
  sinte.begin();                                   
  for (int i = 0; i < 3; i++) {
    sinte.setupVoice(i, SINE, 0, ENVELOPE1, 100, 64);//  (número de voz, tipo de onda, frecuencia inicial, envolvente, duración, modulación)
    pinMode(leds[i], OUTPUT);
    pinMode(scale[i], INPUT);
    pinMode(botPin[i], INPUT_PULLUP);
  }
  pinMode(waveformPin, INPUT);
  pinMode(clockPin, INPUT);
}
void loop()
{
  for (int i = 0; i < 3; i++) {
    waveform=digitalRead(waveformPin);
    if(!waveform)sinte.setWave(i,SAW);
    if(waveform)sinte.setWave(i,SINE);
    freq[i] = map(analogRead(potesFreq[i]),1023,0,40,1063);
    pot6 = map(analogRead(6), 1024, 0, 64, 0);
    scale[i] = digitalRead(scalePin[i]) * 3;
    Clock = digitalRead(clockPin);
    int length = map(analogRead(potelength),1023,0,0,127);
    int modulation = map(analogRead(poteModulacion),1023,0,64,0);
    intervalo[i]=map(analogRead(potesIntervalo[i]),1023,0,4000,0);

    if (scale[i]) {
      frecuencia[i] = penta[map(freq[i], 0, 1023, 0, 35)];
      intervalo[i] = rythm[map(analogRead(potesIntervalo[i]), 0, 1023, 0, 10)];
    }
    else {
      frecuencia[i] = cromatica[map(freq[i], 0, 1023, 0, 35)];
      intervalo[i] = rythm[map(analogRead(potesIntervalo[i]), 0, 1023, 0, 10)];
      }
    sinte.setFrequency(i, frecuencia[i]);
    sinte.setMod(i, modulation);
    sinte.setLength(i,length);
    if (!Clock) {
      if (millis() - previousMillis[i] > intervalo[i]) {
        previousMillis[i] = millis();
        digitalWrite(leds[i], HIGH);
        sinte.trigger(i);
      }
    }
  }
    for (int i = 0; i < 3; i++) {
      buttonState[i] = digitalRead(botPin[i]);
      if (!buttonState[i]) {
        sinte.trigger(i);
        digitalWrite(leds[i], HIGH);
      }
    }
  
  for (int i = 0; i < 3; i++) {
    digitalWrite(leds[i], LOW);
  }




}
