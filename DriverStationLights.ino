#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 154
#define NUM_PATTERNS 5

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 6
#define BRIGHTNESS 125

// Define the array of leds
CRGB leds[NUM_LEDS];

int remainingLEDS[NUM_LEDS];
int patterns[NUM_PATTERNS];
int patternsDone = NUM_PATTERNS-1;
int state = 99;
int incomingByte;
int pointer;
int isBlue, isEnabled, isAuto, isEstopped, lastIsEnabled, currentState = 0, isdsDataValid = 0, readingBus = 0;

int top = 1, bottom = (int)NUM_LEDS/2+1, left = (int)NUM_LEDS/4+1, right = (int)NUM_LEDS/4*3+3;

int dsData[5];
 //int Tcorner1 = 31, Tcorner2 = 48, Tcorner3 = 109, Tcorner4 = 126;


void setup() { 
  dsData[0] = 99;
  Serial.begin(9600) ; //used for debugging purposes
  pinMode(2, INPUT_PULLUP);

 
  FastLED.setBrightness( BRIGHTNESS );

  if(0){//digitalRead(2) == LOW){
  FastLED.addLeds<WS2812B, DATA_PIN, GBR>(leds, NUM_LEDS);  // stole this from example code
  }else{
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // stole this from example code
  }
    //initialize all lights off
    FastLED.clear();

    //create array for random turn on
    for(int i = 0; i<NUM_LEDS; i++){
      remainingLEDS[i] = i;
      }
    patternSetup();
    delay(3000);
    randomToBlue();
    delay(1000);
    state = nextPattern();
    
    }

void loop() { 
  if (Serial.available() > 0)
  serialTest();
  
  if(state == 11)
    snakeFromBlue(50);
    
  if(state == 12)
    snake(50);
    
  if(state == 13){
    state = nextPattern();
    snakeToBlue();
  }

  if(state ==21)
    fadeFromBlue();

  if(state == 22)
    fadeLights();
    
  if(state ==23){
    state = nextPattern();
    fadeToBlue();
  }

  if(state == 31){
    state = nextPattern();
    fourZones();  
  }
  
  if(state == 41){
    state = nextPattern();
    movieTheater();  
  }
  
  if(state == 51){
    state = nextPattern();
    singleChase();  
  }

  if(state == 901){
    //Communication from DS states autonomous mode
    for(int i = 0; i < NUM_LEDS/2+1; i++){
        leds[i].r = 255;
        leds[i].g = 0;
        leds[i].b = 255;
        leds[NUM_LEDS-1-i].r = 255;
        leds[NUM_LEDS-1-i].g = 0;
        leds[NUM_LEDS-1-i].b = 255;
        
        FastLED.show();
    }
  }

  if(state == 902){
    //Communication from DS states enabeled, and on Blue Alliance
    for(int i = 0; i < NUM_LEDS/2+1; i++){
        leds[i].r = 0;
        leds[i].g = 0;
        leds[i].b = 255;
        leds[NUM_LEDS-1-i].r = 0;
        leds[NUM_LEDS-1-i].g = 0;
        leds[NUM_LEDS-1-i].b = 255;
        FastLED.show();
      }
  }
  
  if(state == 903){
    //Communication from DS states enabeled, and on Red Alliance
    for(int i = 0; i < NUM_LEDS; i++){
        leds[i].r = 255;
        leds[i].g = 0;
        leds[i].b = 0;
  
        leds[NUM_LEDS-1-i].r = 255;
        leds[NUM_LEDS-1-i].g = 0;
        leds[NUM_LEDS-1-i].b = 0;
        FastLED.show();
      }
  }
  
  if(state == 904){
    //Communication from DS states disabeled, and on E-Stopped
    for(int i = 0; i < NUM_LEDS; i++){
        leds[i].r = 128;
        leds[i].g = 128;
        leds[i].b = 128;
  
        leds[NUM_LEDS-1-i].r = 128;
        leds[NUM_LEDS-1-i].g = 128;
        leds[NUM_LEDS-1-i].b = 128;
        FastLED.show();
      }
  }
  
  if(state == 99){
    //test state
    singleChase();
  }
}

void singleChase(){
//Actually Double chase. Starts from Blue, goes back to blue
  for(int i = top; i<bottom;i++){
    //Left Side
    for(int j = bottom; j>=i; j--){
      if (Serial.available() > 0){
        if(serialTest())
        return;
      }else{
        leds[j] = CRGB(128,128,128);
        if(j!=bottom){
          leds[j+1] = CRGB(0,0,255);
        }
        leds[(NUM_LEDS+top-(1+j))%NUM_LEDS] = CRGB(0,0,255);
        leds[(NUM_LEDS-j+1)%NUM_LEDS] = CRGB(128,128,128);
        FastLED.show();
      }
    }
  }
  
  for(int i = top; i<bottom;i++){
    //Right side
    for(int j = bottom; j>=i; j--){
      if (Serial.available() > 0){
        if(serialTest())
        return;
      }else{
        leds[j] = CRGB(0,0,255);
        if(j!=bottom){
          leds[j+1] = CRGB(128,128,128);
        }    
        leds[(NUM_LEDS+top-(1+j))%NUM_LEDS] = CRGB(128,128,128);
        leds[(NUM_LEDS-j+1)%NUM_LEDS] = CRGB(0,0,255);
        FastLED.show();
      }
    }
  }

}


void movieTheater(){
  leds[top] =  CRGB( 255, 255, 255);
  leds[bottom]=  CRGB( 255, 255, 255);
  if(isEnabled == 0 && isEstopped ==0){
    FastLED.show();
  }
  for(int k = 0; k < 1; k++){
    for(int i = NUM_LEDS; i < NUM_LEDS*2; i++){
  
  
      for(int j = left-top; j > 0; j--){
        if (Serial.available() > 0){
          if(serialTest())
          return;
        }else{
          leds[(NUM_LEDS+bottom+j)%NUM_LEDS] = leds[(NUM_LEDS+bottom+j-1)%NUM_LEDS];
          leds[(NUM_LEDS+bottom-j)%NUM_LEDS] = leds[(NUM_LEDS+bottom-j+1)%NUM_LEDS];
          leds[(NUM_LEDS+top+j)%NUM_LEDS] = leds[(NUM_LEDS+top+j-1)%NUM_LEDS];
          leds[(NUM_LEDS+top-j)%NUM_LEDS] = leds[(NUM_LEDS+top-j+1)%NUM_LEDS];
          }
        }
      leds[bottom].b = 128 + 127*(int)((i%11)>5);
      leds[bottom].r = 128*(int)((i%11)<=5);
      leds[bottom].g = 128*(int)((i%11)<=5);
  
      leds[top].b = 128+ 127*(int)((i%11)>5);
      leds[top].r = 128*(int)((i%11)<=5);
      leds[top].g = 128*(int)((i%11)<=5);
      
      if(isEnabled == 0 && isEstopped ==0){
        FastLED.show();
      }
      delay(100);
      
      }
    }
  
    for(int i = 0; i < left-top; i++){
      for(int j = left-top; j > 0; j--){
        if (Serial.available() > 0){
          if(serialTest())
            return;
        }else{
          leds[(NUM_LEDS+bottom+j)%NUM_LEDS] = leds[(NUM_LEDS+bottom+j-1)%NUM_LEDS];
          leds[(NUM_LEDS+bottom-j)%NUM_LEDS] = leds[(NUM_LEDS+bottom-j+1)%NUM_LEDS];
      
          
          leds[(NUM_LEDS+top+j)%NUM_LEDS] = leds[(NUM_LEDS+top+j-1)%NUM_LEDS];
          leds[(NUM_LEDS+top-j)%NUM_LEDS] = leds[(NUM_LEDS+top-j+1)%NUM_LEDS];
      
        }
      }
      leds[bottom].b = 255;
      leds[bottom].r = 0;
      leds[bottom].g = 0;
  
      leds[top].b = 255;
      leds[top].r = 0;
      leds[top].g = 0;
  
    if(isEnabled == 0 && isEstopped ==0){
    FastLED.show();
  }
      delay(100);
    
    }

}

void dsActive(){



  if(isEstopped){
    state = 904;
    //Serial.write("To E-stop");
    currentState = 4;
    return;
  }else if(isAuto){
    state = 901;
    
    currentState = 3;
    //Serial.write("To Auto");
    
    return;
  }else if(isBlue){
    state = 902;
    currentState = 2;
    //Serial.write("ToBlue");
    return;
    
  }else {
    state = 903;
    //Serial.write("To Red");
    currentState = 1;
    return;
  
  }
  
}

int serialTest(){

  
//int alliance;
//int enable;

//Serial.write("CheckSerial\n");

do {
  if (Serial.available() > 0) {
    
    // read the incoming byte:
    
    char incomingByte = Serial.read();
    readingBus = 1;
    
    //Serial.write(incomingByte);
    //Serial.write('\n');
    

if(incomingByte>=48){
  switch(incomingByte){
    
  case 83:

    //FastLED.clear();
    dsData[0] = 1;
    pointer = 1;
    break;
  
  case 69:
    isdsDataValid = 1;
    dsData[0] = 0;
    readingBus = 1;
    
    while(Serial.available() > 0) {
      char t = Serial.read();
    }

    if(pointer !=5){
      
    Serial.write('0');
    dsData[0] = 99;
    }
    pointer = 0;
    break;
    
  default:
    dsData[pointer] = incomingByte-48;
    pointer++;
    break;
  }
}
  }
  delay(10);
} while (Serial.available() > 0);

  if(dsData[0]==0){

              dsData[0] = 99;

          if(dsData[1]){
            isBlue = 0;
          }else{
            isBlue = 1;
          }

              lastIsEnabled = isEnabled;
              isEnabled = dsData[2];
              isAuto = dsData[3];
              isEstopped = dsData[4];
//              Serial.write("isBlue ");
//              Serial.write(isBlue+48);
//              Serial.write('\n');
//              
//              Serial.write("isEnabled ");
//              Serial.write(isEnabled+48);
//              Serial.write('\n');
//              
//              Serial.write("isAuto ");
//              Serial.write(isAuto+48);
//              Serial.write('\n');
//              
//              Serial.write("isEstopped ");
//              Serial.write(isEstopped+48);
//              Serial.write('\n');
              if(isEnabled||isEstopped)
              dsActive();

              if(isEnabled == 0 && lastIsEnabled == 1 && isEstopped == 0){
                
                transitionToPatterns();
                return;
              }

              
}
return isEnabled||isEstopped;
}

    
  
void transitionToPatterns(){

for(int i = 0; i < NUM_LEDS/2+1; i++){
      leds[i].r = 0;
      leds[i].g = 0;
      leds[i].b = 255;
      leds[NUM_LEDS-1-i].r = 0;
      leds[NUM_LEDS-1-i].g = 0;
      leds[NUM_LEDS-1-i].b = 255;
      FastLED.show();
    }

    state = nextPattern();
  
}


  


void fourZones(){


 int corner1 = 31, corner2 = 49, corner3 = 109, corner4 = 126;

 double shortLongRatio;

 shortLongRatio = (corner3-corner2)/(corner4-corner3);
for(int rep = 0; rep < 3; rep++){
    for(int i=0;i<corner3-corner2;i++){
      if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
    
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
if(isEnabled == 0 && isEstopped ==0){
  FastLED.show();
}
  delay(10);
}
    }
for(int i=0;i<corner3-corner2;i++){
  if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
    
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
if(isEnabled == 0 && isEstopped ==0){
  FastLED.show();
}
  delay(5);
}
}
}
}

void fadeFromBlue(){
//transition from full blue to every other light fading blue and white
for(int j = 0; j<255;j++){
  for(int i = 0; i < NUM_LEDS; i++){
    if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
    if(i % 2==0){
  leds[i].r = j;
  leds[i].g = j;
  leds[i].b = 255;
    }else{
  
    }
    
  
}
  }
if(isEnabled == 0 && isEstopped ==0){
  FastLED.show();
}
  delay(3);
  }
  state = 22;
}

void fadeToBlue(){
//transition from every other light fading blue and white to full blue
for(int j = 0; j<255;j++){
  for(int i = 0; i < NUM_LEDS; i++){
    if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
    if(leds[i].r != 0){
  leds[i].r -= 1;
  leds[i].g -= 1;
  leds[i].b = 255;
    }else{
  
    }
    
  
  }
  }
if(isEnabled == 0 && isEstopped ==0){
  FastLED.show();
}
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
    if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
    remainingLEDS[k] = remainingLEDS[k+1];
  }
  }
  if(isEnabled == 0 && isEstopped ==0){
  FastLED.show();
  }

  for(i=0; i<NUM_LEDS;i++){
    if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
    LEDCheck += leds[i]/255;
    
  }
  }
  }
  
  
  
}

void fadeLights(){
  state = 23;
  //every other light is blue, otherwise white. Fades inbetween the 2 colors
for(int rep = 0; rep < 3; rep++){
  for(int j = 0; j<255;j++){
  for(int i = 0; i < NUM_LEDS; i++){
    if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
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
  }
if(isEnabled == 0 && isEstopped ==0){
  FastLED.show();
}
  delay(1);
  }

  for(int j = 255; j>0; j--){
  for(int i = 0; i < NUM_LEDS; i++){
    if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
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
  }
if(isEnabled == 0 && isEstopped ==0){
  FastLED.show();
}
  delay(1);
  }
}
}

void snakeFromBlue(int len){
  for(int i = 0; i < NUM_LEDS-len; i++){
    if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
//    if(i == NUM_LEDS - len){
//      leds[i].b = 255;
//    }else{
      leds[i].b = 0;
//    }
if(isEnabled == 0 && isEstopped ==0){
    FastLED.show();
}
    delay(10);
  }
}

   // for(int i = NUM_LEDS-1;i<NUM_LEDS;i++){
      for(int j = NUM_LEDS-len;j<NUM_LEDS;j++){
        if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
        leds[j].b = ((255)/(len-1))*(j-NUM_LEDS+len);
        if(isEnabled == 0 && isEstopped ==0){
        FastLED.show();
        }
        delay(10);
      }
}
      //leds[i].b = 255;
     //FastLED.show();
    //delay(10);
   // }
  state = 12;
}

void snake(int len){
int blueFade = 0;
for(int rep = 0; rep < 3; rep++){
  for(int i = NUM_LEDS; i < NUM_LEDS+NUM_LEDS; i++){
    for(int light = 0; light < NUM_LEDS; light++){
      if (Serial.available() > 0){
  if(serialTest())
  return;
}else{

leds[light].b = 0;
leds[light].g = 0;
leds[light].r = 0;
      
    }
  }
blueFade = 0;
  

    for(int j = 0; j < len; j++){
      if (Serial.available() > 0){
  if(serialTest())
  return;
}else{

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
  }
    if(isEnabled == 0 && isEstopped ==0){
    FastLED.show();
    }
  delay(10);
    
  }
}
  state = 13;
}

void snakeToBlue(){
  for(int i = 0; i < NUM_LEDS; i++){
    if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
    leds[i].b = 255;
    if(isEnabled == 0 && isEstopped ==0){
    FastLED.show();
    }
    delay(10);
  }
}
}

void patternSetup(){
for(int i = 0; i < NUM_PATTERNS; i++){
  if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
  patterns[i] = i +1;
}
}

}

int nextPattern(){

  patternsDone++;

  if(patternsDone == NUM_PATTERNS){
patternsDone = 0;
shuffle(patterns, NUM_PATTERNS);
    
  }
  

  return patterns[patternsDone]*10+1;

}

void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
      if (Serial.available() > 0){
  if(serialTest())
  return;
}else{
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
    }
}
