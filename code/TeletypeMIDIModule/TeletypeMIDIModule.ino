/*
 * Teletype MIDI Module
 * 
 * Adapted from Brendon Cassidy's TxO module
 * MIT License
 */
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);


// i2c Wire Library (for Teensy)
#include <i2c_t3.h>

// eeprom library
#include <EEPROM.h>

// support libraries
#include "telex.h"
#include "TxHelper.h"

/*
 * Ugly Globals
 */

// i2c pullup setting
bool enablePullups = false;


// write timer and its local variables
IntervalTimer writeTimer;
int readerTemp = 0;
int p = 0;

// iterator values
int i = 0;
int q = 0;




/*
 * Setup Function 
 */
void setup() {  
  MIDI.begin();


  // initialize the teensy optimized wire library
  Wire.begin(I2C_SLAVE, 0x11, I2C_PINS_18_19, enablePullups ? I2C_PULLUP_INT : I2C_PULLUP_EXT, I2C_RATE_400); // I2C_RATE_2400
  Wire.onReceive(receiveEvent);
  Serial.print('x');
}

/*
 * Main Arduino Appliation Loop
 */
void loop() {

}

/*
 * Wire Callback
 */
void receiveEvent(size_t len) {
#ifdef DEBUG
  // set the back LED active to indicate data transfer
  Serial.printf("Event received of size %d \n", len);
#endif

  // parse the response
  TxResponse response = TxHelper::Parse(len);

  // act on the command
  actOnCommand(response.Command, response.Output, response.Value);
}

/*
 * Act on the Commands Delivered to the Teletype MIDI Module
 */
void actOnCommand(byte cmd, byte out, int value){
  Serial.printf("Action: %d, Output: %d, Value: %d\n", cmd, out, value);  
  switch(cmd) {
    case II_MIDI_PRG:
      MIDI.sendProgramChange(highByte(value), out);
      break;
    
    case II_MIDI_CC:     
      MIDI.sendControlChange(highByte(value),lowByte(value), out);
      break;  

    case II_MIDI_ON:
      MIDI.sendNoteOn(highByte(value), lowByte(value), out);

      break;
      
    case II_MIDI_OFF:
          MIDI.sendNoteOff(highByte(value), lowByte(value), out);
      break;
    
    case II_BUS_SAVE:
      MIDI.sendControlChange(16,127,1);
      MIDI.sendProgramChange(out,1);
      break;
    case II_BUS_LOAD:
      MIDI.sendControlChange(16,64,1);
      MIDI.sendProgramChange(out,1);
      
      //
      break;
      
   
      break;


  }
}  
