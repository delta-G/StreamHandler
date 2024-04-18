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

template<>
void VariableUpdater<int>::display(char* ret) {
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "<%c%d>", matchChar, var);
}

template<>
void VariableUpdater<float>::display(char* ret) {
  char buf[16];
  dtostrf(var, 2, 2, buf);
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "<%c%s>", matchChar, buf);
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
    inBuffer[0] = 0;
  } else if (receiving) {
    if (c == eop) {
      receiving = false;
      checkCommands();
    }
    inBuffer[index] = c;
    inBuffer[++index] = 0;

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

void StreamHandler::setDefaultHandler(void (*h)(char*, char*)) {
  defaultHandler = h;
}

void StreamHandler::addCommand(Command* com) {
  if (!commandExists(com->matchChar)) {
    com->next = first;
    first = com;
  }
}

void StreamHandler::addFunctionCommand(char c, ComFuncPtr f) {
  if (!commandExists(c)) {
    FunctionCommand* command = new FunctionCommand(c, f);
    addCommand(command);
  }
}

void StreamHandler::addReturnCommand(char c, RetFuncPtr f) {
  if (!commandExists(c)) {
    ReturnCommand* command = new ReturnCommand(c, f);
    addCommand(command);
  }
}

boolean StreamHandler::commandExists(char c) {
  for (Command* ptr = first; ptr != nullptr; ptr = ptr->next) {
    if (ptr->match(c)) {
      return true;
    }
  }
  return false;
}

void StreamHandler::checkCommands() {
  bool found = false;
  for (Command* ptr = first; ptr != nullptr; ptr = ptr->next) {
    // Start markers should be stripped.  Command char should be first element
    if (ptr->match(*inBuffer)) {
      found = true;
      ptr->handle(inBuffer, outBuffer);
      sendOutBuffer();
      break;
    }
  }
  if (defaultHandler && !found) {
    defaultHandler(inBuffer, outBuffer);
    sendOutBuffer();
  }
}

void StreamHandler::sendOutBuffer() {
  if (out && strlen(outBuffer) > 0) {
    out->write(outBuffer, strlen(outBuffer));
  }
  outBuffer[0] = 0;
}