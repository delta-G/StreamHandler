/*

StreamHandler  --  Some automation for Stream objects
     Copyright (C) 2024  David C.

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.

     */

#include "StreamHandler.h"

// parser and formatter for a type not defined in the library
template<>
uint8_t Parser<uint8_t>::parse(char* str) {
  return atoi(str + 1);  // skip command character
}
template<>
void Formatter<uint8_t>::format(uint8_t v, char* ret) {
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "%u", v);
}

// Define a custom formatter
class MyCustomFormatter : public Formatter<unsigned int> {
  virtual void format(unsigned int, char*);
};

void MyCustomFormatter::format(unsigned int v, char* out) {
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, "CUSTOM - %d!", v);
}


// define some variables to use in the demo
int i = 2;
long j = 3;
unsigned int k = 4;
float m = 2.2;
uint8_t n = 6;

int analog;

//define some functions to call
// a function that will respond back to the stream takes two char*
// one for the input string and one for the return string. 
void aFunction(char* str, char* ret) {
  Serial.print("\n ** A function - ");
  Serial.println(str);
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "matched 'A' printed i %d", i);
}
void bFunction(char* str, char* ret) {
  Serial.print("\n ** B function - ");
  Serial.println(str);
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "matched 'B' printed j %ld", j);
}

// Functions that don't respond just take the input string
void cFunction(char* str) {
  Serial.print("\n ** C function - ");
  Serial.println(str);
  Serial.print(" - prints k - ");
  Serial.println(k);
}

void dFunction(char* str) {
  Serial.print("\n ** D function - ");
  Serial.println(str);
  Serial.print(" - prints m - ");
  Serial.println(m, 6);
}

void eFunction(char* str) {
  Serial.print("\n ** E function - ");
  Serial.println(str);
  Serial.print(" - prints n - ");
  Serial.println(n);
}

// Timed responders take a char* to write their message to
void timeFunction(char* out) {
  snprintf(out, STREAM_HANDLER_MAX_LENGTH, "Time is %lu\n", millis());
}

// You can set a default command for when no other matches
void def(char* str, char* ret) {
  strncpy(ret, "Default Handler", STREAM_HANDLER_MAX_LENGTH);
  Serial.println(str);
}

// Commands and Reporters can be set to 
// take in and / or respond with raw bytes
void rawFunc(char* str, char* ret) {
  Serial.println("Raw Func Called!");
  Serial.println(str);
  ret[0] = '<';
  memcpy(ret + 1, str, str[1] + 2);
  ret[str[1] + 3] = '>';
}



// create a StreamHandler and connect to Serial
StreamHandler streamHandler(&Serial, &Serial);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n**** Starting StreamHandler.ino **** \n\n");
  // add commands
  streamHandler.addReturnCommand('A', aFunction);
  streamHandler.addReturnCommand('B', bFunction);

  streamHandler.addFunctionCommand('C', cFunction);
  streamHandler.addFunctionCommand('D', dFunction);
  streamHandler.addFunctionCommand('E', eFunction);

  // Variable Updaters parse the input for values and update the appropriate variables
  streamHandler.addVariableUpdater('I', i);
  streamHandler.addVariableUpdater('J', j);  
  // You can set parsing options or even build custom parsers
  streamHandler.addVariableUpdater('K', k)->setParser(new Parser<unsigned int>(HEX));  // This one takes input in hexadecimal. 

  streamHandler.addVariableUpdater('M', m);
  streamHandler.addVariableUpdater('N', n);

  // Send a variable value at regular intervals       // set up custom formatters to customize data
  streamHandler.addTimedVariableReporter('Z', k, 1000)->setFormatter(new MyCustomFormatter());

  // Call a function at regalar intervals and send a response
  streamHandler.addTimedFunctionReporter(timeFunction, 750);

  // Send a variable value whenever it changes. 
  streamHandler.addOnChangeVariableReporter('#', analog);     // this one spams a lot, comment it out to play with others. 

  // Set commands to take and give raw data 
  streamHandler.addReturnCommand('Q', rawFunc)->setRawIn()->setRawOut();

  // Set a default handler for unmatched commands. 
  streamHandler.setDefaultHandler(def);
}


void loop() {
  streamHandler.run();  // run the stream handler
  analog = analogRead(0) / 10;  // divide by 10 so it doesn't bounce so much
}


// test String   <A_Hello><I42><A><D><M3.141592><D><E><N127><E><C><KAE><C>
// raw test string <Q 12345678901234567890123456789012>>>>>
