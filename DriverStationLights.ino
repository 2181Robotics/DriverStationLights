#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 155

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 6
#define BRIGHTNESS 150

// Define the array of leds
CRGB leds[NUM_LEDS];

int remainingLEDS[NUM_LEDS];
int state = 1;
 int Tcorner1 = 31, Tcorner2 = 48, Tcorner3 = 109, Tcorner4 = 126;


void setup() { 
 // Serial.begin(9600) ; used for debugging purposes
 
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // stole this from example code
    //initialize all lights off
    FastLED.clear();

    //create array for random turn on
    for(int i = 0; i<NUM_LEDS; i++){
      remainingLEDS[i] = i;
      }
      
    delay(3000);
    randomToBlue();
    delay(1000);
    //FastLED.clear();
    }

void loop() { 

//state formatting:
// single unit (ex 1, 2, 3, 4, etc...) is intro to pattern
// X2 is actual pattern (ex 12 is the first indexed pattern. Happens after state 1 is finished)
// X3 is outro from pattern (ex 13 is the first indexed pattern outro. Happens after state 12 is finished)
// if a pattern is its own intro and outro, it should stay a single unit state
// Goal is to use a random number generator to pick pattern order
// Valid sequence example (state 3 starts from blue, and goes to blue in its normal pattern):
// 1-12-13-4-42-43-3-7-72-73-10-102-103
// all intros are from blue, all outros are to blue
if(state == 1)

/*{
    leds[Tcorner1].b = 255;
    leds[Tcorner1].g = 255;
    leds[Tcorner1].r = 255;

    leds[Tcorner2].b = 255;
    leds[Tcorner2].g = 255;
    leds[Tcorner2].r = 255;

    leds[Tcorner3].b = 255;
    leds[Tcorner3].g = 255;
    leds[Tcorner3].r = 255;
    
    leds[Tcorner4].b = 255;
    leds[Tcorner4].g = 255;
    leds[Tcorner4].r = 255;
    FastLED.show();
}*/
//fourZones();

snakeFromBlue(50);

  if(state == 12)
  for(int rep = 0; rep < 3; rep++)
  snake(50);
  //fadeLights();

  if(state == 13){
  snakeToBlue();

  state = 2;}

  if(state ==2)
  fadeFromBlue();

  if(state == 22)
  for(int rep = 0; rep < 3; rep++)
  fadeLights();

  if(state ==23){
  fadeToBlue();
  state = 3;}

if(state == 3){
for(int rep = 0; rep < 3; rep++)
  fourZones();
  state =1;
}
  

  //snake(50);
}


void fourZones(){


 int corner1 = 31, corner2 = 48, corner3 = 109, corner4 = 126;

 double shortLongRatio;

 shortLongRatio = (corner3-corner2)/(corner4-corner3);

    for(int i=0;i<corner3-corner2;i++){
    
  leds[(corner4+i)%NUM_LEDS].b = 255;
  leds[(corner4+i)%NUM_LEDS].g = 255;
  leds[(corner4+i)%NUM_LEDS].r = 255;

  leds[((int)((corner3+i/shortLongRatio)))%NUM_LEDS].b = 255;
  leds[((int)((corner3+i/shortLongRatio)))%NUM_LEDS].g = 255;
  leds[((int)((corner3+i/shortLongRatio)))%NUM_LEDS].r = 255;

  leds[(corner2+i)%NUM_LEDS].b = 255;
  leds[(corner2+i)%NUM_LEDS].g = 255;
  leds[(corner2+i)%NUM_LEDS].r = 255;

  leds[((int)((corner1+i/shortLongRatio)))%NUM_LEDS].b = 255;
  leds[((int)((corner1+i/shortLongRatio)))%NUM_LEDS].g = 255;
  leds[((int)((corner1+i/shortLongRatio)))%NUM_LEDS].r = 255;

  FastLED.show();
  delay(10);
}

for(int i=0;i<corner3-corner2;i++){
    
  leds[(corner4+i)%NUM_LEDS].b = 255;
  leds[(corner4+i)%NUM_LEDS].g = 0;
  leds[(corner4+i)%NUM_LEDS].r = 0;

  leds[((int)((corner3+i/shortLongRatio)))%NUM_LEDS].b = 255;
  leds[((int)((corner3+i/shortLongRatio)))%NUM_LEDS].g = 0;
  leds[((int)((corner3+i/shortLongRatio)))%NUM_LEDS].r = 0;

  leds[(corner2+i)%NUM_LEDS].b = 255;
  leds[(corner2+i)%NUM_LEDS].g = 0;
  leds[(corner2+i)%NUM_LEDS].r = 0;

  leds[((int)((corner1+i/shortLongRatio)))%NUM_LEDS].b = 255;
  leds[((int)((corner1+i/shortLongRatio)))%NUM_LEDS].g = 0;
  leds[((int)((corner1+i/shortLongRatio)))%NUM_LEDS].r = 0;

  FastLED.show();
  delay(5);
}
}

void fadeFromBlue(){
//transition from full blue to every other light fading blue and white
for(int j = 0; j<255;j++){
  for(int i = 0; i < NUM_LEDS; i++){
    if(i % 2==0){
  leds[i].r = j;
  leds[i].g = j;
  leds[i].b = 255;
    }else{
  
    }
    
  

  }

  FastLED.show();
  delay(3);
  }
  state = 22;
}

void fadeToBlue(){
//transition from every other light fading blue and white to full blue
for(int j = 0; j<255;j++){
  for(int i = 0; i < NUM_LEDS; i++){
    if(leds[i].r != 0){
  leds[i].r -= 1;
  leds[i].g -= 1;
  leds[i].b = 255;
    }else{
  
    }
    
  

  }

  FastLED.show();
  delay(1);
  }
 
}

void randomToBlue(){
//randomly turns lights blue until they are all on
  int i;
  int LEDCheck = 0;
    
  for(int j = 0; j < NUM_LEDS;j++){
  i = random(NUM_LEDS-j);

  leds[remainingLEDS[i]].b = 255;
  for(int k = i; k < NUM_LEDS-j-1;k++){
    remainingLEDS[k] = remainingLEDS[k+1];
  }
  FastLED.show();

  for(i=0; i<NUM_LEDS;i++){
    LEDCheck += leds[i]/255;
    
  }
  }
  
  
  
}

void fadeLights(){
  //every other light is blue, otherwise white. Fades inbetween the 2 colors

  for(int j = 0; j<255;j++){
  for(int i = 0; i < NUM_LEDS; i++){
    if(i % 2){
  leds[i].r = j;
  leds[i].g = j;
  leds[i].b = 255;
    }else{
  leds[i].r = 255-j;
  leds[i].g = 255-j;
  leds[i].b = 255;
    }
    
  
 
 
  }

  FastLED.show();
  delay(1);
  }

  for(int j = 255; j>0; j--){
  for(int i = 0; i < NUM_LEDS; i++){
    if(i % 2){
  leds[i].r = j;
  leds[i].g = j;
  leds[i].b = 255;
    }else{
  leds[i].r = 255-j;
  leds[i].g = 255-j;
  leds[i].b = 255;
    }
    
  
}

  FastLED.show();
  delay(1);
  }
  state = 23;
}

void snakeFromBlue(int len){
  for(int i = 0; i < NUM_LEDS-len; i++){
//    if(i == NUM_LEDS - len){
//      leds[i].b = 255;
//    }else{
      leds[i].b = 0;
//    }
    FastLED.show();
    delay(10);
  }

   // for(int i = NUM_LEDS-1;i<NUM_LEDS;i++){
      for(int j = NUM_LEDS-len;j<NUM_LEDS;j++){
        leds[j].b = ((255)/(len-1))*(j-NUM_LEDS+len);
        FastLED.show();
        delay(10);
      }
      //leds[i].b = 255;
     //FastLED.show();
    //delay(10);
   // }
  state = 12;
}

void snake(int len){
int blueFade = 0;
  for(int i = NUM_LEDS; i < NUM_LEDS+NUM_LEDS; i++){
    for(int light = 0; light < NUM_LEDS; light++){

leds[light].b = 0;
leds[light].g = 0;
leds[light].r = 0;
      
    }
blueFade = 0;
  

    for(int j = 0; j < len; j++){

    leds[(i-j)%NUM_LEDS].b = (255)-blueFade;


   /* 
    *********Keeping in case I want to re-introduce white snake
    *********Need white snake in intro to accomplish
    leds[NUM_LEDS-((i-j)%NUM_LEDS)].b += (255/2)-blueFade;
    leds[NUM_LEDS-((i-j)%NUM_LEDS)].g += (255/2)-blueFade;
    leds[NUM_LEDS-((i-j)%NUM_LEDS)].r += (255/2)-blueFade;
    */


    blueFade = ((255)/(len-1))*j;
    }
    
    FastLED.show();
  delay(10);
    
  }
  state = 13;
}

void snakeToBlue(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i].b = 255;
    FastLED.show();
    delay(10);
  }
}
