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
  if (raw) {
    // here we have at least 2 characters
    // inBuffer[0] is the command char
    // inBuffer[1] is the count
    // Collect chars until we have the total
    if (index >= inBuffer[1] + 3) {
      // here we have the entire contents plus the command char and count
      // that means this character should be the end marker
      if (c == eop) {
        checkCommands();
        raw = false;
        receiving = false;
      }
    }
    // Otherwise just add it to the buffer and keep counting
    inBuffer[index++] = c;
  } else if (c == sop) {
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
      if (index == 2) {
        if (findCommand(inBuffer[0])->isRaw) {
          raw = true;
        }
      }
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
  if (findCommand(com->matchChar) == nullptr) {
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
  if (findCommand(c) == nullptr) {
    FunctionCommand* command = new FunctionCommand(c, f);
    addCommand(command);
  }
}

void StreamHandler::addTimedFunctionReporter(RepFuncPtr f, unsigned long i) {
  TimedFunctionReporter* reporter = new TimedFunctionReporter(f, i);
  addReporter(reporter);
}

void StreamHandler::addReturnCommand(char c, RetFuncPtr f) {
  if (findCommand(c) == nullptr) {
    ReturnCommand* command = new ReturnCommand(c, f);
    addCommand(command);
  }
}

StreamCommand* StreamHandler::findCommand(char c) {
  for (StreamCommand* ptr = firstCom; ptr != nullptr; ptr = ptr->next) {
    if (ptr->match(c)) {
      return ptr;
    }
  }
  return nullptr;
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