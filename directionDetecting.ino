#include <NewPing.h>

#include "FastLED.h"

#define TRIGGER_PIN_0  2
#define ECHO_PIN_0  3

#define TRIGGER_PIN_1  5
#define ECHO_PIN_1  6

#define NUM_LEDS 360
#define DATA_PIN 7

uint8_t gHue = 0; 

#define MAX_DISTANCE 500

NewPing sonar_0(TRIGGER_PIN_0, ECHO_PIN_0, MAX_DISTANCE);

NewPing sonar_1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);

CRGB leds[NUM_LEDS];

boolean lightOn = false;

long detectM;

int vertical[] = {0,1,2,4,6,8,9};

#define FRAMES_PER_SECOND  120

class MoveDetector {

  private:
  boolean state_0;
  boolean state;
  int range_0;
  int range_1;
  long t;
  
  public: 
  Movement(){
    state_0 = false;
    state = false;
    range_0 = 0;
    range_1 = 0;
    t = 0;
  }

  boolean startDetecting(){
    sonarsListener();
    return stateSwitch();
  }

  void sonarsListener(){
    //delay(50);
  
    range_0 = (sonar_0.ping() / US_ROUNDTRIP_CM); 
    if (range_0 > 0) {
      Serial.print("0 ");
      Serial.print(range_0);
      Serial.println(" cm");
    }
  
    range_1 = (sonar_1.ping() / US_ROUNDTRIP_CM); 
    if (range_1 > 0) {
      Serial.print("1 ");
      Serial.print(range_1);
      Serial.println(" cm");
    }
  }

  boolean stateSwitch(){
    
    if(state_0 == true){
      if(millis() - t > 1000){
        state_0 =false;
        t = 0;
        Serial.println("State 0 Expired");
      }else if(range_1 > 0 && range_1 <400){
        //state = true;
        state_0 = false;
        Serial.println("Movement Detected");
        return true;
      }
    }
    
    if(range_0 > 0 && range_0 < 400){
      t = millis();
      state_0 =true;
      Serial.println("Sonar 0 Detected");
    }

    return false;
  }

  
};

MoveDetector md;

//void fadeall() { 
//  
//  for(int i = 0; i < NUM_LEDS; i++) { 
//    
//    leds[i].nscale8(250); 
//  } 
//}

void setup() {
  
  Serial.begin(9600);

  delay(2000);

  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  LEDS.setBrightness(80);
    


}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
    FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

void juggle() {
  Serial.println("juggle");
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

void bpm()
{
  Serial.println("bpm");
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
  
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

void loop() {

  //bpm();

  //juggle();

    //EVERY_N_MILLISECONDS( 20 ) { gHue++;}

    //rainbow();
  if(md.startDetecting()){
    
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(0, 100, 0);
    }

    
    
    LEDS.setBrightness(50);
    
  
    for(int i = 0; i < 120; i++) {
      leds[i] = CHSV(200, 255, 255);
      FastLED.show(); 
      fadeToBlackBy(leds, 120, 20);
      delay(5);
    }

    for(int i = 0; i < 120; i ++){
      //Serial.println(i);
      leds[i] = CHSV(0, 100, 0);
    }
    FastLED.show();
    
    for(int i = 0; i < 7; i ++){
      
      //Serial.println(vertical[i]);
      
      for(int j = 0; j < 24; j ++){
        //Serial.println(120 + 24 * vertical[i] + j);
        leds[120 + 24 * vertical[i] + j] = CHSV(200, 255, 255);
        
       FastLED.show();
       //delay(10);
      }
    }
    delay(100);

    int breathTimes = 3;
    
    while(breathTimes > 0){
      breathTimes --;
      
      for(int breath = 80; breath >= 10; breath --){
        FastLED.setBrightness(breath);
        FastLED.show(); 
        delay(10);
      }
      for (int breath = 10; breath < 80; breath ++ ){
        FastLED.setBrightness(breath);
        FastLED.show(); 
        delay(5);
      }
    }
    for(int breath = 80; breath >= 0; breath --){
        FastLED.setBrightness(breath);
        FastLED.show(); 
        delay(5);
    }
  }

}
