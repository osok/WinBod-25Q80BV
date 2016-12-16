/**
 * WBFlash.cpp - Library for easy interface to the WinBond 25Q80BV
 *              flash chip.  Could work with others, just not tested.
 * Created by osok
 * github https://github.com/osok/WinBod-25Q80BV
 * The Library is licensed under a Creative Commons Attribution 3.0 License
 * https://creativecommons.org/licenses/by/3.0/). 
 */
 
#include "WBFlash.h"

WBFlash::WBFlash(int pin){
  pinMode(pin, INPUT);
  flashPin = pin;
  setupFlash();
}



void WBFlash::setupFlash(){

  prl("[1] setupFlash");
  
  buffer_pos = 0;
  
  if (!(flashCapacity = flash.begin())){
    prl("    Flash not intialized");
    return;
  }

  pr("    Capacity = ");
  prl(flashCapacity);
  
  prl("[2] Flash Initialized ");
}

void WBFlash::eraseFlash(){
  prl("[M1] erasing flash");
  if(!flash.eraseChip()){
    prl("      Error while erasing chip...");
  }else{
    flashAddy = 0x00;
    buffer_pos = 0;
  }
  
  prl("     flash erased");
}

void WBFlash::writeFlash(){
  uint8_t buf[FLASHPAGE];
  uint8_t input[4];
  int startPage = 0;
  int maxPage = flashCapacity/FLASHPAGE;
  int inputCnt = 0;
  int bufSize = 0;
  
  prl("[M2] write flash");

  // Get the Start page
  pr("Start Page [0-");
  pr(maxPage - 1);
  pr("] >");
  while(!Serial.available());
  inputCnt = Serial.readBytes(input, 4);
  for(int x=0;x<inputCnt;x++){
    startPage *= 10;
    startPage += input[x]-'0';
  }
    
  // Bounds checking
  if(startPage < 0 || startPage >= maxPage){
    pr("Must be between 0 and ");
    prl(maxPage);
    return;
  }
  prl(startPage);  
  
  pr("Enter Data to Write to flash (max length ");
  pr(FLASHPAGE);
  pr(") > ");
  while(!Serial.available());
  prl("");
  
  while(Serial.available()){
    bufSize = Serial.readBytes(buf, FLASHPAGE);
    pr("Input was ");
    pr(bufSize);
    prl(" bytes long.");
    
    for(int p = 0; p < bufSize; p++){
      
      if (buffer_pos == FLASHPAGE){
        flash.writePage(FLASHPAGE * startPage, buffer);
        startPage++;
        buffer_pos = 0;
        for(int i = 0; i < FLASHPAGE; i++){
          buffer[i] = 0xFF;
        }
        pr("New Flash Address position = ");
        pr2(startPage*FLASHPAGE, HEX);
      }
      buffer[buffer_pos++] = buf[p];
   }
  }
  
  // Basically flush the current buffer with an extra null, 
  // then if more data is written later the same buffer will 
  // be written to the buffer and the same data will still
  // be in the buffer to be written again.
 flash.writePage(FLASHPAGE * startPage, buffer);

  prl("");
  prl("Exiting Write Flash");
}


void  WBFlash::printPage(uint8_t page[FLASHPAGE]){
  uint8_t hexData[FLASHPAGE/LINE_SIZE][LINE_SIZE];
  int lineCount = FLASHPAGE/LINE_SIZE;
  
  for(int sp = 0; sp < lineCount; sp++){
    for(int i=0; i < LINE_SIZE; i++){
      hexData[sp][i] = page[sp*LINE_SIZE+i];
    }//end for i
  }//end for sp

  for(int line=0; line < lineCount; line++){
    pr("[");
    for(int i=0; i < LINE_SIZE; i++){
      pr2(hexData[line][i], HEX);
      pr(' ');
    }//end for i
    pr("]  [");
    for(int i=0; i < LINE_SIZE; i++){
      pr((char)hexData[line][i]);
    }//end for i
    prl("]");
  }//end for line

}

void  WBFlash::readFlash(){
  uint8_t page[FLASHPAGE];
  
  prl("[M3] read flash");

  int startPage = 0;
  int pageCount = 0;
  int maxPage = flashCapacity/FLASHPAGE;
  uint8_t input[4];
  int inputCnt = 0;
 
  // Get the Start page
  pr("Start Page [0-");
  pr((maxPage-1));
  pr("] > ");
  while(!Serial.available());
  inputCnt = Serial.readBytes(input, 4);
  for(int x=0;x<inputCnt;x++){
    startPage *= 10;
    startPage += input[x]-'0';
  }
    
  // Bounds checking
  if(startPage < 0 || startPage >= maxPage){
    pr("Must be between 0 and ");
    prl(maxPage);
    return;
  }
  prl(startPage);  
  
  // Get the page count
  pr("Page Count [1-");
  pr(maxPage-startPage);
  pr("] > ");
  
  while(!Serial.available());
  inputCnt = Serial.readBytes(input, 4);
  for(int x=0;x<inputCnt;x++){
    pageCount *= 10;
    pageCount += input[x]-'0';
  }
   
  // Bounds checking
  if(pageCount < 1 || pageCount > maxPage-startPage){
    pr("Must be between 1 and ");
    prl(maxPage-startPage);
    return;
  }
  prl(pageCount);

  //TODO: I'm not sure why start page of 3000+ fails,
  //      but start page 0 & pageCount 4095 works
  while(!flash.beginRead(startPage * FLASHPAGE)){
    pr("Error Reading from flash.");
    return;
  }
  for(int p = startPage; p < startPage+pageCount; p++){
    for(int i=0; i < FLASHPAGE;i++){
      page[i] = flash.readNextByte();
    }
    pr("Page [");
    pr(p);
    prl("]");
    printPage(page);
  }
  flash.endRead();
  prl("Done Reading Flash");
}

void WBFlash::printMenu(){
  prl("Main Menu");
  prl("  1 - Erase Flash");
  prl("  2 - Write Flash");
  prl("  3 - Read Flash");
  prl("  4 - Read Raw");
  prl("  0 - Exit Application");
  prl("Enter Command > ");
}

int WBFlash::getState(){
  return digitalRead(flashPin);
}

