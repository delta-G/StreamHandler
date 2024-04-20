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
    inBuffer[index++] = c;
    if (index >= inBuffer[1] + 3) {
      raw = false;
    }
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
        if (findCommand(inBuffer[0])->isRawIn) {
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

StreamCommand* StreamHandler::addCommand(StreamCommand* com) {
  if (com) {
    if (findCommand(com->matchChar) == nullptr) {
      com->next = firstCom;
      firstCom->prev = com;
      firstCom = com;
    }
  }
  return com;
}

StreamReporter* StreamHandler::addReporter(StreamReporter* rep) {
  if (rep) {
    rep->next = firstRep;
    firstRep->prev = rep;
    firstRep = rep;
  }
  return rep;
}

FunctionCommand* StreamHandler::addFunctionCommand(char c, ComFuncPtr f) {
  if (findCommand(c) == nullptr) {
    FunctionCommand* command = new FunctionCommand(c, f);
    addCommand(command);
    return command;
  }
  return nullptr;
}

TimedFunctionReporter* StreamHandler::addTimedFunctionReporter(RepFuncPtr f, unsigned long i) {
  TimedFunctionReporter* reporter = new TimedFunctionReporter(f, i);
  return addReporter(reporter);
}

ReturnCommand* StreamHandler::addReturnCommand(char c, RetFuncPtr f) {
  if (findCommand(c) == nullptr) {
    ReturnCommand* command = new ReturnCommand(c, f);
    addCommand(command);
    return command;
  }
  return nullptr;
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
      raw = ptr->isRawOut;
      sendOutBuffer();
      raw = false;
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
    raw = ptr->isRawOut;
    sendOutBuffer();
    raw = false;
  }
}

void StreamHandler::sendOutBuffer() {
  if (out) {
    if (raw) {
      out->write(outBuffer, outBuffer[2]);
      outBuffer[0] = 0;
    }
    if (strlen(outBuffer) > 0) {
      out->write(outBuffer, strlen(outBuffer));
    }
    outBuffer[0] = 0;
  }
}