# ArduinoBatteryLife
How long does the Arduino (compatible) board run on a powersource?

## Usage
Load this code into the Arduino (compatible) board.
Connect to the desired powersource and let it run until the powersource gives up.

The program stores the `micros()` in EEPROM address `0x0010` and the `millis()` in EEPROM address `0x0014` both as a four byte integer with MSB first (big-endian). So, if the powersupply expires within 70 minutes then the number stored in `0x0010` will be the interesting one. If the powersupply stops within 50 days then the number stored in `0x0014` will be useful. 

When the powersource lives past 50 days, you'll need to account for the wraparound of the `millis()` timer ([ref](https://www.arduino.cc/en/Reference/Millis)).

Reconnect and load the [ArduinoEEPROMdump](https://github.com/Mausy5043/ArduinoEEPROMdump) code.
> This reads the chip's EEPROM and dumps it to the serial port.
> You will need to convert the four bytes of interest to a decimal value
> Use the resulting microsecond or millisecond value to determine to lifetime of the powersource.
