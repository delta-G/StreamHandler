#include "StreamHandler.h"

// define some variables
int i = 2;
int j = 3;
int k = 4;

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

// define an array of function commands
// doesn't have to be an array but then they'd all need names
// you could also use new and create them on the heap.
FunctionCommand fcoms[] = {
  { 'A', aFunction },
  { 'B', bFunction },
  { 'C', cFunction }
};

// an array of variable commands
// same as above, doesn't have to be an array but this is easier.
VariableCommand vcoms[] = {
  { 'I', i },
  { 'J', j },
  { 'K', k }
};

// create a StreamHandler and connect to Serial
StreamHandler streamHandler(&Serial);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n**** Starting StreamHandler.ino **** \n\n");
}

void loop() {
  streamHandler.run();  // run the stream handler
}
