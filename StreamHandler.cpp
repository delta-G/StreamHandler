#include "StreamHandler.h"

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
    int counter = greedy;
    do {
      char c = in->read();
      handleChar(c);
    } while (in->available() && counter--);
  }
  sendReports();
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
    } else {
      inBuffer[index] = c;
      inBuffer[++index] = 0;
    }

    if (index >= STREAM_HANDLER_BUFFER_SIZE - 1) {
      index--;
    }
  }
}

void StreamHandler::setGreedy(int g) {
  greedy = g;
}

int StreamHandler::getGreedy() {
  return greedy;
}

void StreamHandler::setDefaultHandler(void (*h)(char*, char*)) {
  defaultHandler = h;
}

void StreamHandler::addCommand(StreamCommand* com) {
  if (!commandExists(com->matchChar)) {
    com->next = firstCom;
    firstCom->prev = com;
    firstCom = com;
  }
}

void StreamHandler::addReporter(StreamReporter* rep) {
  rep->next = firstRep;
  firstRep->prev = rep;
  firstRep = rep;
}

void StreamHandler::addFunctionCommand(char c, ComFuncPtr f) {
  if (!commandExists(c)) {
    FunctionCommand* command = new FunctionCommand(c, f);
    addCommand(command);
  }
}

void StreamHandler::addTimedFunctionReporter(RepFuncPtr f, unsigned long i) {
  TimedFunctionReporter* reporter = new TimedFunctionReporter(f, i);
  addReporter(reporter);
}

void StreamHandler::addReturnCommand(char c, RetFuncPtr f) {
  if (!commandExists(c)) {
    ReturnCommand* command = new ReturnCommand(c, f);
    addCommand(command);
  }
}

boolean StreamHandler::commandExists(char c) {
  for (StreamCommand* ptr = firstCom; ptr != nullptr; ptr = ptr->next) {
    if (ptr->match(c)) {
      return true;
    }
  }
  return false;
}

void StreamHandler::checkCommands() {
  bool found = false;
  for (StreamCommand* ptr = firstCom; ptr != nullptr; ptr = ptr->next) {
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

void StreamHandler::sendReports() {
  for (StreamReporter* ptr = firstRep; ptr != nullptr; ptr = ptr->next) {
    ptr->handle(outBuffer);
    sendOutBuffer();
  }
}

void StreamHandler::sendOutBuffer() {
  if (out && strlen(outBuffer) > 0) {
    out->write(outBuffer, strlen(outBuffer));
  }
  outBuffer[0] = 0;
}