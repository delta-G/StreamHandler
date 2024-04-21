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

// parses an uint8_t from the string
template<>
uint8_t Parser<uint8_t>::parse(char* str) {
  return atoi(str + 1);  // skip command character
}
template<>
void Formatter<uint8_t>::format(uint8_t v, char* ret) {
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "%u", v);
}

class MyCustomFormatter : public Formatter<int> {
  virtual void format(int, char*);
} ;

void MyCustomFormatter::format(int v, char* out) {
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, "CUSTOM - %d!", v);
}


// Parser<int> hexParser = Parser<int>(16);


// define some variables
int i = 2;
int j = 3;
int k = 4;
float m = 2.2;
uint8_t n = 6;

int analog;

//define some functions to print them
void aFunction(char* str, char* ret) {
  Serial.print("\n ** A function - ");
  Serial.println(str);
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "matched 'A' printed i %d", i);
}
void bFunction(char* str, char* ret) {
  Serial.print("\n ** B function - ");
  Serial.println(str);
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "matched 'B' printed j %d", j);
}
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

void timeFunction(char* out) {
  snprintf(out, STREAM_HANDLER_MAX_LENGTH, "Time is %lu\n", millis());
}

void def(char* str, char* ret) {
  strncpy(ret, "Default Handler", STREAM_HANDLER_MAX_LENGTH);
  Serial.println(str);
}

void rawFunc(char* str, char* ret) {
  Serial.println("Raw Func Called!");
  Serial.println(str);
  ret[0] = '<';
  memcpy(ret + 1, str, str[1] + 2);
  ret[str[1] + 3] = '>';
}
// test String   <A_Hello><I42><A><D><M3.141592><D><E><N127><E><C><KAE><C>
//  <Q 12345678901234567890123456789012>>>>>
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

  streamHandler.addVariableUpdater('I', i);
  streamHandler.addVariableUpdater('J', j, false);  // doesn't send return back
  streamHandler.addVariableUpdater('K', k)->setParser(new Parser<int>(HEX));

  streamHandler.addVariableUpdater('M', m);
  streamHandler.addVariableUpdater('N', n);

  streamHandler.addTimedVariableReporter('Z', k, 1000)->setFormatter(new MyCustomFormatter());
  streamHandler.addTimedFunctionReporter(timeFunction, 750);
  // streamHandler.addOnChangeVariableReporter('#', analog);

  streamHandler.addReturnCommand('Q', rawFunc)->setRawIn()->setRawOut();

  streamHandler.setDefaultHandler(def);
}

void loop() {
  streamHandler.run();  // run the stream handler
  analog = analogRead(0);
}
