void readEncoder(){
 pushEncoder=digitalRead(pushEncoderPin);
 envelope=digitalRead(pushEnvelope);

  if(pushEncoder&&envelope){
    if(toggleEncoder==1||toggleEncoder==2){
      encoder.write(length*4);
      toggleEncoder=0;
    }
    newPositionLength = encoder.read();
    if (newPositionLength != oldPositionLength) {
      if(newPositionLength>508){encoder.write(508);}
      if(newPositionLength<0){encoder.write(0);}
      oldPositionLength = newPositionLength;
      length=newPositionLength/4;
      Serial.println(length);
    }
  }
  if(!pushEncoder&&envelope){
    if(toggleEncoder==0||toggleEncoder==2){
      encoder.write(scale*8);
      toggleEncoder=1;
    }
    newPositionScale = encoder.read();
    if (newPositionScale != oldPositionScale) {
      if(newPositionScale>55){encoder.write(55);}
      if(newPositionScale<0){encoder.write(0);}
      oldPositionScale = newPositionScale;
      scale=newPositionScale/8;
      Serial.println(scale);
      FastLED.show();
    }
  }
      if(!envelope&&!pushEncoder){
      modulation=64;
      encoder.write(modulation*4);
      toggleEncoder=2;
    }
  if(pushEncoder&&!envelope){
    newPositionModulation=encoder.read();

    if(toggleEncoder==0||toggleEncoder==1){
      encoder.write(modulation*4);
      toggleEncoder=2;
    }
    if(newPositionModulation!=oldPositionModulation){
      if(newPositionModulation>508){encoder.write(508);}
      if(newPositionModulation<0){encoder.write(0);}
      oldPositionModulation = newPositionModulation;
      modulation = newPositionModulation/4;
      }
    
  }
}
