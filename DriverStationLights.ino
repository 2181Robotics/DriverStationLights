#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 117
#define NUM_PATTERNS 5

/*                         
                         
                         //     |     \\
                        //      |      \\
          _______  ____//_      |       \\    _______          
|\     /|\__   __/(  ____ \|\     /||\   \\/|(  ___  )|\     /|
| )   ( |   ) (   | (    \/| )   ( || )   ( || (   ) |( \   / )
| (___) |   | |   | |      | (___) || | _ | || (___) | \ (_) / 
|  ___  |   | |   | | ____ |  ___  || |( )| ||  ___  |  \   /  
| (   ) |   | |   | | \_  )| (  |) || || || || (   ) |   ) (   
| )   ( |___) (___| (___) || )  |( || () () || )   ( |   | |   
|/     \|\_______/(_______)|/   | \|(_______)|/     \|   \_/   
               //               |               \\                  
              //    _________ __|____            \\              
             //     \__   __/(  ___  )            \\             
            //         ) (   | (   ) |             \\            
           //          | |   | |   | |              \\           
          //           | |   | |   | |               \\          
         //            | |   | |   | |                \\         
        //             | |   | (___) |                 \\        
       //              )_(   (_______)                  \\       
      //                        |                        \\      
     //     _______  _______  _ |      ______   _______   \\     
  |\//   /|(  ___  )(  ____ )( \      (  __  \ (  ____ \   \\    
  | )   ( || (   ) || (    )|| (      | (  \  )| (    \/    \\   
  | | _ | || |   | || (____)|| |      | |   ) || (_____      \\  
  | |( )| || |   | ||     __)| |      | |   | |(_____  )      \\ 
  | || || || |   | || (\ (   | |      | |   ) |      ) |       \\
  | () () || (___) || ) \ \__| (____/\| (__/  )/\____) |        \\
  (_______)(_______)|/   \__/(_______/(______/ \_______)         \\




*/
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\\|||||
// _____  __   _____  __    _____   _____   ___   ______             |
/// __  \/  | |  _  |/  |  |  __ \ |  ___| / _ \  | ___ \            |
//`' / /'`| |  \ V / `| |  | |  \/ | |__  / /_\ \ | |_/ /  ___       |
//  / /   | |  / _ \  | |  | | __  |  __| |  _  | |    /  / __|      |
//./ /____| |_| |_| |_| |_ | |_\ \_| |____| | | |_| |\ \ _\__ \      |
//\_____/\___/\_____/\___/  \____(_)____(_)_| |_(_)_| \_(_)___/      |
//                                                                   |
//-------------------------------------------------------------------+                                                               
//______      _                  _____ _        _   _                |
//|  _  \    (_)                /  ___| |      | | (_)               |
//| | | |_ __ ___   _____ _ __  \ `--.| |_ __ _| |_ _  ___  _ __     |
//| | | | '__| \ \ / / _ \ '__|  `--. \ __/ _` | __| |/ _ \| '_ \    |
//| |/ /| |  | |\ V /  __/ |    /\__/ / || (_| | |_| | (_) | | | |   |
//|___/ |_|  |_| \_/ \___|_|    \____/ \__\__,_|\__|_|\___/|_| |_|   |
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 6
#define BRIGHTNESS 125

// Define the array of leds
CRGB leds[NUM_LEDS];



const byte numChars = 6;
char receivedChars[numChars];

boolean newData = false;


int remainingLEDS[NUM_LEDS];
int patterns[NUM_PATTERNS];
int patternsDone = NUM_PATTERNS-1;
int state = 99;
int incomingByte;
int pointer;
int isBlue, isEnabled, isAuto, isEstopped, lastIsEnabled, currentState = 0, isdsDataValid = 0, readingBus = 0;

int isChanged = 0, redRight, estopCount = 0;


//These would likely change if the lights were re-done
int top = 1, bottom = (int)NUM_LEDS/2+1, left = (int)NUM_LEDS/4+1, right = (int)NUM_LEDS/4*3+3;

int dsData[5];

void setup() { 
  dsData[0] = 99;
  Serial.begin(2400);
  
  
  //Serial.begin(9600) ; //used for debugging purposes
  pinMode(2, INPUT_PULLUP);

 
  FastLED.setBrightness( BRIGHTNESS );

  if(0){
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
    


    
    }

void loop() { 

 for (int i = 0; i <= NUM_LEDS; i++) {

    leds[i].r = 0;
  leds[i].g = 0;
  leds[i].b = 255;

  FastLED.show();
  }
  delay(10);
  for (int i = 0; i <= NUM_LEDS; i++) {

    leds[i].r = 255;
  leds[i].g = 255;
  leds[i].b = 255;

  FastLED.show();
}
delay(10);
}

