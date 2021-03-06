/* *****************************************************************************

                  Arduino Battery Life

(c) 2016, Maurice Hendrix  (mausy5043)
***************************************************************************** */

/*
 *
 * BN: Nano Compatible ATMEGA328p
 * VID: 1A86
 * PID: 7523
 * 2300mAh / 47h = 48mA
 * 2300mAh / 43h = 53mA
 */

/*
 * EXAMPLE CALCULATION *
 * Output from EEPROM dumper:
 * =-EEPROM DUMP-= 
 * 0000  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
 * 0010  9F 7C E9 54 09 60 1F F9 00 00 00 00 00 00 00 00 .|.T.`..........
 *                  |----ms-----|
 * 0x09601FF9 convert to decimal = 157'294'585 ms
 * divide by 1000 to get seconds = 157'294 s
 * divide by 3600 to get hours   = 43 h
 */

#include <EEPROM.h> 

long addr = 0x10;
long utime = 0;
long mtime = 0;

void setup()
{
  delay(1000);
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port...
  }
  pinMode(13, OUTPUT);
  Serial.println("Interrupt normal operation within 60sec to avoid losing EEPROM data");
  delay(60000);  // wait now to allow re-programming without resetting EEPROM contents
}

void loop() 
{
  Serial.print("Time: ");
  utime = micros();
  mtime = millis();
  //prints time since program started
  Serial.print(utime, HEX);
  Serial.print(" us (");
  EEPROMWritelong(addr, utime);
  // addr += 4;
  Serial.print(")   ");
  Serial.print(mtime, HEX);
  Serial.print(" ms (");
  EEPROMWritelong(addr + 4, mtime);
  
  Serial.println(")   ");
  // wait a second so as not to send massive amounts of data
  delay(10000);
  digitalWrite(13, !digitalRead(13));   // turn the LED on (HIGH is the voltage level)
}

// This function will write a 4 byte (32bit) long to the EEPROM at
// the specified address u/i address + 3.
void EEPROMWritelong(int address, long value)
{
  // Decomposition from a long to 4 bytes by using bitshift.
  // (addr) = Most significant -> (addr+3) = Least significant byte
  byte four  = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two   = ((value >> 16) & 0xFF);
  byte one   = ((value >> 24) & 0xFF);

  // Write the 4 bytes into the EEPROM memory.
  EEPROM.update(address, one);
  EEPROM.update(address + 1, two);
  EEPROM.update(address + 2, three);
  EEPROM.update(address + 3, four);
}

// This function will return a 4 byte (32bit) long from the EEPROM
// at the specified address u/i address + 3.
long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the EEPROM memory.
  // (addr) = Most significant -> (addr+3) = Least significant byte
  long one   = EEPROM.read(address);
  long two   = EEPROM.read(address + 1);
  long three = EEPROM.read(address + 2);
  long four  = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

