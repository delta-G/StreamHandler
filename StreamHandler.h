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

#ifndef STREAM_HANDLER_H
#define STREAM_HANDLER_H

#include "Arduino.h"
#include "StreamCommand.h"
#include "StreamReporter.h"
#include "StreamParser.h"
#include "StreamHandlerDefines.h"



/*
*
*    SreamHandler class
*    handles the actual parsing
*/

class StreamHandler {

private:

  char inBuffer[STREAM_HANDLER_BUFFER_SIZE];
  char outBuffer[STREAM_HANDLER_BUFFER_SIZE];
  int index;

  Stream* in;
  Print* out;
  char sop;
  char eop;

  boolean receiving = false;
  boolean raw = false;
  int greedy = 0;

  void handleChar(char c);

  void (*defaultHandler)(char*, char*) = nullptr;

  StreamCommand* firstCom;
  StreamReporter* firstRep;

  StreamCommand* findCommand(char);
  void checkCommands();
  void sendReports();
  void sendOutBuffer();

public:
  StreamHandler(Stream* aIn, Print* aOut, char aSop, char aEop)
    : index(0), in(aIn), out(aOut), sop(aSop), eop(aEop){};
  StreamHandler(Stream* aIn, char aSop, char aEop)
    : StreamHandler(aIn, nullptr, aSop, aEop){};
  StreamHandler(Stream* aIn, Print* aOut)
    : StreamHandler(aIn, aOut, DEFAULT_SOP, DEFAULT_EOP){};
  StreamHandler(Stream* aIn)
    : StreamHandler(aIn, nullptr){};

  StreamHandler(const StreamHandler& other) = delete;
  StreamHandler& operator=(const StreamHandler& other) = delete;
  ~StreamHandler() {
    while (firstCom != nullptr) {
      // The StreamCommand destructor will move firstCom to the next instance.
      delete firstCom;
    }
    while (firstRep != nullptr) {
      delete firstRep;
    }
  }

  void run();

  void setGreedy(int);
  int getGreedy();

  void setDefaultHandler(void (*)(char*, char*));

  StreamCommand* addCommand(StreamCommand*);
  StreamReporter* addReporter(StreamReporter*);
  FunctionCommand* addFunctionCommand(char, ComFuncPtr);
  TimedFunctionReporter* addTimedFunctionReporter(RepFuncPtr, unsigned long);
  ReturnCommand* addReturnCommand(char, RetFuncPtr);
  template<class T>
  VariableUpdater<T>* addVariableUpdater(char, T&, bool = true);
  template<class T>
  TimedVariableReporter<T>* addTimedVariableReporter(char, T&, unsigned long);
  template<class T>
  OnChangeVariableReporter<T>* addOnChangeVariableReporter(char, T&);
};

template<class T>
VariableUpdater<T>* StreamHandler::addVariableUpdater(char c, T& v, bool e) {
  if (findCommand(c) == nullptr) {
    VariableUpdater<T>* updater = new VariableUpdater<T>(c, v, e);
    addCommand(updater);
    return updater;
  }
  return nullptr;
}

template<class T>
TimedVariableReporter<T>* StreamHandler::addTimedVariableReporter(char c, T& v, unsigned long i) {
  TimedVariableReporter<T>* reporter = new TimedVariableReporter<T>(c, v, i);
  addReporter(reporter);
  return reporter;
}

template<class T>
OnChangeVariableReporter<T>* StreamHandler::addOnChangeVariableReporter(char c, T& v) {
  OnChangeVariableReporter<T>* reporter = new OnChangeVariableReporter<T>(c, v);
  addReporter(reporter);
  return reporter;
}


#endif  //STREAM_HANDLER_H