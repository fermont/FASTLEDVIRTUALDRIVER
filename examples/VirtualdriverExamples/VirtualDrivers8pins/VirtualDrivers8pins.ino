#define ESP32_VIRTUAL_DRIVER true //To enable virtual pin driver for esp32
#define ESP_VIRTUAL_DRIVER_8 1 //to use the 8:1 ratio
#define NBIS2SERIALPINS 8
#define NUM_LEDS_PER_STRIP 256 //the length of your strip if you have different strips size put here the longuest strip

/*
This needs to be declared before #include "FastLed.h"
NBIS2SERIALPINS represents the number of pins used by the esp value, it ranges from 1->15
NUM_LEDS_PER_STRIP the length of your strip if you have different strips size put here the longuest strip
for each pin you would be able to drive 8 virtuals pins
i.e for
#define NBIS2SERIALPINS 8
You will be able to drive up to 8*8=64 pins
If you want to have a number of strips which is not a multiple of 8 no issue
if you want to drive 63 pins you would need #define NBIS2SERIALPINS 8
if you want to drive 17 pins you would need #define NBIS2SERIALPINS 3

ATTENTION : compare to the other drivers the order of the strip ot of the 74HC595 is mixed
Q0/PIN 15 => VIRTUAL PIN 7
Q1/PIN 1  => VIRTUAL PIN 8
Q2/PIN 2  => VIRTUAL PIN 5
Q3/PIN 3  => VIRTUAL PIN 6
Q4/PIN 4  => VIRTUAL PIN 3
Q5/PIN 5  => VIRTUAL PIN 4
Q6/PIN 6  => VIRTUAL PIN 1
Q7/PIN 7  => VIRTUAL PIN 2

*/
#include "FastLED.h"
#define LATCH_PIN 13
#define CLOCK_PIN 27 //for a reason I don't know, the CLOCK_PIN needs to be >=16


#define NUM_STRIPS NBIS2SERIALPINS * 8
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS
CRGB leds[NUM_LEDS];

int Pins[NBIS2SERIALPINS]={12,14,26,25,33,32,15,18};

void setup() {
    Serial.begin(115200);
    /*
    * Define the esp pins you are using
    * pin 12 will drive strip 1->8
    * pin 14 will drive strip 9->16
    * pin 26 will drive strip 17->24
    * pin 25 will drive strip 25->32
    * pin 33 will drive stip  33->40
    * pin 32 will drive strip 41->48
    * pin 15 will drive strip 49->56
    * pin 18 will drive strip 57->64
    */



    FastLED.addLeds<VIRTUAL_DRIVER,Pins,CLOCK_PIN, LATCH_PIN>(leds,NUM_LEDS_PER_STRIP);
    FastLED.setBrightness(64);

}
int start=0;
void loop() {
    fill_solid(leds, NUM_LEDS, CRGB(15,15,15));
    /*
    * this code will create a snake on each strip where the length is the strips number
    */
    for(int i=0;i<NUM_STRIPS;i++)
    {
        int offset=i*NUM_LEDS_PER_STRIP;   //this is the offest of the strip number i
        for(int k=0;k<i+1;k++)
        {
            leds[(start+k)%NUM_LEDS_PER_STRIP+offset]=CHSV(i*255/NUM_STRIPS,255,255);
        }

    }
    long     lastHandle = __clock_cycles();
    FastLED.show();
    long   lasthandle2=__clock_cycles();
    Serial.printf("FPS fastled: %f \n", (float) 240000000L/(lasthandle2 - lastHandle));
    start++;
    delay(30);

}