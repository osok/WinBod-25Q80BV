/**
 * WBFlash.h - Library for easy interface to the WinBond 25Q80BV
 *              flash chip.  Could work with others, just not tested.
 * Created by osok
 * github https://github.com/osok/WinBod-25Q80BV
 * The Library is licensed under a Creative Commons Attribution 3.0 License
 * https://creativecommons.org/licenses/by/3.0/). 
 */

#ifndef WBFlash_h
#define WBFlash_h

#include <Adafruit_TinyFlash.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <EEPROM.h>

#define LINE_SIZE 16

// location in atmega eeprom to store last flash write address
#define E_FLASH_ADDY 0x00 // 4 bytes
#define E_SETUP      0x04 // 1 byte [could be bit]
#define FLASHSETUP 0xBA

// 256 bytes per page of flash
#define FLASHPAGE 256

#define PKT_SIZE 16
#define MS_PER_SCAN 500

/**
 * I"m not sure why this will not work. 
 * Itt worked under C but when I converted
 * to C++ the F() stopped working.
 * I found some reference to the way the underlying
 * code was implementing casts.
 * I figure there has to be a way to do this in C++
 * The F() function writes the strings to flash (of which there is ~32KB)
 * instead of RAM (which there is ~2K) 
 * Reference: http://playground.arduino.cc/Learning/Memory
 */
//#define pr(a) Serial.print(F(a))
//#define pr2(a,b) Serial.print(F(a),b)
//#define prl(a) Serial.println(F(a))
//#define prl2(a,b) Serial.println(F(a),b)

#define pr(a) Serial.print(a)
#define pr2(a,b) Serial.print(a,b)
#define prl(a) Serial.println(a)
#define prl2(a,b) Serial.println(a,b)


class WBFlash{
  private:
    void flashSetup();
    uint8_t buffer[FLASHPAGE];
    uint32_t flashAddy;
    uint32_t buffer_pos = 0;
    uint32_t flashCapacity = 0;
    uint32_t flashByte = 0;
    int flashPin;
    Adafruit_TinyFlash flash;
    void setupFlash();
    void printPage(uint8_t page[FLASHPAGE]);
    
    
  public:
    WBFlash(int pin);
    void eraseFlash();
    void writeFlash();
    void readFlash();
    void printMenu();
    int getState();
};

#endif
