/**
 * OneMegFlash.ino - is an example of using the WBFlash class.
 * Created by osok
 * github https://github.com/osok/WinBod-25Q80BV
 * The Library is licensed under a Creative Commons Attribution 3.0 License
 * https://creativecommons.org/licenses/by/3.0/). 
 */
 
#include "WBFlash.h"


#define FLASH_PIN 2 // Flash Menu or Other 
// Serial baudrate
#define BAUDRATE 115200

WBFlash wbflash(FLASH_PIN);

void setup() {
  Serial.begin(BAUDRATE);
}

void loop() {
  int input = 0;
  int goodCommand = 0; 
  int buttonState = 0;
  boolean buttonPressed = false; 
  
  while(true){
    buttonState = wbflash.getState();

    if(buttonState == 1 && !buttonPressed){
      pr("Button is Pressed ");
      buttonPressed = true;
    }else if(buttonState == 1 && buttonPressed){
      //Do nothing
    }else{
      buttonPressed = false;
      wbflash.printMenu();
      goodCommand = 0; 
      while(goodCommand==0){
        if(Serial.available()){
          input = Serial.read();
          prl((char)input);
          if(input == 49){ //1 - ERASE
            goodCommand = 1; 
            wbflash.eraseFlash();
          }else if(input == 50){ //2 - WRITE
            goodCommand = 1; 
            wbflash.writeFlash();
          }else if(input == 51){ //3 - READ
            goodCommand = 1; 
            wbflash.readFlash();
          }else if(input == 52){ //4 - READ Raw
            goodCommand = 1; 
            wbflash.readFlash();
          }else if(input == 48){ //0 - EXIT
            goodCommand = 1; 
            prl("[X] System Exit");
            return;
          }else{
            prl("Invalid Entry...");
            pr("Enter Command > ");
          }//end nested if
        }//end serial available
      }// end goodCommand while
    }// end if buttonState
  }// End outer while

}
