// Simple MP3 player example
//
// Requires the audio shield:
//   http://www.pjrc.com/store/teensy3_audio.html
//
// This example code is in the public domain.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <play_sd_mp3.h>
#include "sequence.h"

#define TRIGGER_PIN 2
#define STATUS_PIN 13
// GUItool: begin automatically generated code
AudioPlaySdMp3           playMp31;       //xy=154,78
AudioOutputI2S           i2s1;           //xy=334,89
AudioConnection          patchCord1(playMp31, 0, i2s1, 0);
AudioConnection          patchCord2(playMp31, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=240,153
// GUItool: end automatically generated code
//int steps = 880;
void setup() {
  DDRB = 0xF;
  DDRC = 0xF;
  Serial.begin(9600);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(STATUS_PIN, OUTPUT);
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(5);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);

  //SPI.setMOSI(7);
  //SPI.setSCK(14);
  if (!(SD.begin(BUILTIN_SDCARD))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  playFile("ambient.mp3");
}

void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playMp31.play(filename);

  // Simply wait for the file to finish playing.
 }
char buf[100];
void loop()
{
  static int loops = 0;
  boolean trigger;
  static boolean playing =false;
  static int count = 0;
  int step;
  char b;
  char c;
  trigger = digitalRead(TRIGGER_PIN);
  if(trigger==false)
  { 
    count++;
  }
  else count = 0;
  if(count > 1000)
  {
    playMp31.stop();
    playFile("scare.mp3"); 
    digitalWrite(STATUS_PIN,true);
    playing = true;
    Serial.println("Playing scare");
    count = 0;
  }
  if(playing == true)
  {
    step = sequence[loops];
    c = (step  & 0x0F) | (PINC & 0xF0);
    b = ((step >> 4) & 0x0F) | (PINB & 0xF0);
    PORTC = c;
    PORTB = b;
    sprintf(buf,"out = %.2X  %.2X", PINB & 0xf , PINC & 0xF);
    Serial.println(buf);
    delay(50);
    loops++;
    if(loops%10 == 0)
    {
      Serial.print("Playing step");
      Serial.println(loops);
    }
    
    if(loops > steps)
    {
      playing = false;
      playMp31.stop();
      playFile("SDTEST1.mp3");
      loops = 0;
      digitalWrite(STATUS_PIN,false);
      Serial.println("Playing ambient");

    }
  }
  if(playing == false)
  {
    //Serial.println("Playing is false");
    if(!playMp31.isPlaying())
    {
      Serial.println("Replaying");
      playFile("ambient.mp3"); 
    }
    //delay (250);
  }

}
