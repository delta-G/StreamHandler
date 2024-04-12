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


// create a StreamHandler and connect to Serial
StreamHandler streamHandler(&Serial);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n**** Starting StreamHandler.ino **** \n\n");
  // add commands
  streamHandler.addCommand(new FunctionCommand('A', aFunction));
  streamHandler.addCommand(new FunctionCommand('B', bFunction));
  streamHandler.addCommand(new FunctionCommand('C', cFunction));
  streamHandler.addCommand(new FunctionCommand('D', dFunction));
  streamHandler.addCommand(new FunctionCommand('E', eFunction));

  streamHandler.addCommand(new VariableUpdater<int>('I', i));
  streamHandler.addCommand(new VariableUpdater<int>('J', j));
  streamHandler.addCommand(new VariableUpdater<int>('K', k));

  streamHandler.addCommand(new VariableUpdater<float>('M', m));
  streamHandler.addCommand(new VariableUpdater<uint8_t>('N', n));
}

void loop() {
  streamHandler.run();  // run the stream handler
}
