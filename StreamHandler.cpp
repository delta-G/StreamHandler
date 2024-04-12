#include "StreamHandler.h"


// parses an integer from the string
template<>
int VariableUpdater<int>::parse(char* str) {
  return atoi(str + 1);  // skip command character
}
// parses an double from the string
template<>
float VariableUpdater<float>::parse(char* str) {
  return atof(str + 1);  // skip command character
}

/*
*
*    StreamHandler methods
*
*/

void StreamHandler::run() {

  //	if (receivingRaw) {
  //		handleRawData();
  //	} else
  if (in->available()) {
    do {
      char c = in->read();
      handleChar(c);
    } while (in->available() && greedy);
  }
}

void StreamHandler::handleChar(const char c) {

  if (c == sop) {
    receiving = true;
    index = 0;
    _SHbuffer[0] = 0;
  } else if (receiving) {
    if (c == eop) {
      receiving = false;
      checkCommands();
    }
    _SHbuffer[index] = c;
    _SHbuffer[++index] = 0;

    if (index >= STREAM_HANDLER_BUFFER_SIZE - 1) {
      index--;
    }
  }
}

void StreamHandler::setGreedy(bool aBoo) {
  greedy = aBoo;
}

bool StreamHandler::getGreedy() {
  return greedy;
}

void StreamHandler::addCommand(Command* com){
  com->next = first;
  first = com;
}

void StreamHandler::checkCommands(){
  for (Command* ptr = first; ptr != nullptr; ptr = ptr->next) {
    // Start markers should be stripped.
    if (ptr->match(_SHbuffer)) {
      ptr->handle(_SHbuffer);
      break;
    }
  }
  
}