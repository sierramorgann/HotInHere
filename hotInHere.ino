/*************************************************** 
Arduino Mega with an adafruit music maker and an ambient heat sensor 
When the heat sensor detects the room to be hotter than 81 degrees 
the music maker plays "It's getting hot in here" the loop runs every 
half-hour.  
 ****************************************************/
#include <SPI.h>             
#include "Adafruit_VS1053.h"
#include <SD.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

int track1, track2, track3;
const int pingPin = 8;
const int echoPin = 9;

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS      7      // VS1053 chip select pin (output)
#define SHIELD_DCS     6      // VS1053 Data/command select pin (output)

#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  track2=0;
  pinMode(8,OUTPUT);
  pinMode(9,INPUT);
  Serial.begin(9600);

  Serial.println("Music maker with sensor test"); 
  
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
  SD.begin(CARDCS);    // initialise the SD card
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(40,40); 

  mlx.begin();  
}

void heat() {
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");

  Serial.print(" ");
}

void loop() {
  int temp = mlx.readAmbientTempF();
   heat();
   if (temp >= 81) {
      musicPlayer.playFullFile("track001.mp3");
      Serial.println("Done playing music");
      delay(1800000); 
    }
    
  delay(500);
}
