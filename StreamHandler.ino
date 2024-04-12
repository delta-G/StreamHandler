#include "StreamHandler.h"

// parses an double from the string
template<>
uint8_t VariableUpdater<uint8_t>::parse(char* str) {
  return atoi(str + 1);  // skip command character
}

// define some variables
int i = 2;
int j = 3;
int k = 4;
float m = 2.2;
uint8_t n = 6;

//define some functions to print them
void aFunction(char* str) {
  Serial.print("\n ** A function - ");
  Serial.println(str);
  Serial.print(" - prints i - ");
  Serial.println(i);
}
void bFunction(char* str) {
  Serial.print("\n ** B function - ");
  Serial.println(str);
  Serial.print(" - prints j - ");
  Serial.println(j);
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

// test String   <A_Hello><I42><A><D><M3.141592><D><E><N127><E>
// create a StreamHandler and connect to Serial
StreamHandler streamHandler(&Serial);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n**** Starting StreamHandler.ino **** \n\n");
  // add commands
  streamHandler.addFunctionCommand('A', aFunction);
  streamHandler.addFunctionCommand('B', bFunction);
  streamHandler.addFunctionCommand('C', cFunction);
  streamHandler.addFunctionCommand('D', dFunction);
  streamHandler.addFunctionCommand('E', eFunction);

  streamHandler.addVariableUpdater('I', i);
  streamHandler.addVariableUpdater('J', j);
  streamHandler.addVariableUpdater('K', k);

  streamHandler.addVariableUpdater('M', m);
  streamHandler.addVariableUpdater('N', n);
}

void loop() {
  streamHandler.run();  // run the stream handler
}
