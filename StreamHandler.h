#ifndef STREAM_HANDLER_H
#define STREAM_HANDLER_H

#include "Arduino.h"
#include "StreamCommand.h"
#include "StreamReporter.h"
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
  Stream* out;
  char sop;
  char eop;

  boolean receiving = false;
  int greedy = 0;

  void handleChar(char c);

  void (*defaultHandler)(char*, char*) = nullptr;

  StreamCommand* firstCom;
  StreamReporter* firstRep;

  boolean commandExists(char);
  void checkCommands();
  void sendReports();
  void sendOutBuffer();

public:
  StreamHandler(Stream* aIn, Stream* aOut, char aSop, char aEop)
    : index(0), in(aIn), out(aOut), sop(aSop), eop(aEop){};
  StreamHandler(Stream* aIn, char aSop, char aEop)
    : StreamHandler(aIn, nullptr, aSop, aEop){};
  StreamHandler(Stream* aIn, Stream* aOut)
    : StreamHandler(aIn, aOut, DEFAULT_SOP, DEFAULT_EOP){};
  StreamHandler(Stream* aIn)
    : StreamHandler(aIn, nullptr){};

  void run();

  void setGreedy(int);
  int getGreedy();

  void setDefaultHandler(void (*)(char*, char*));

  void addCommand(StreamCommand*);
  void addReporter(StreamReporter*);
  void addFunctionCommand(char, ComFuncPtr);
  void addTimedFunctionReporter(RepFuncPtr, unsigned long);
  void addReturnCommand(char, RetFuncPtr);
  template<class T>
  void addVariableUpdater(char, T&, bool = true);
  template<class T>
  void addTimedVariableReporter(char, T&, unsigned long);
  template<class T>
  void addOnChangeVariableReporter(char, T&);
};

template<class T>
void StreamHandler::addVariableUpdater(char c, T& v, bool e) {
  if (!commandExists(c)) {
    VariableUpdater<T>* updater = new VariableUpdater<T>(c, v, e);
    addCommand(updater);
  }
}

template<class T>
void StreamHandler::addTimedVariableReporter(char c, T& v, unsigned long i) {
  TimedVariableReporter<T>* reporter = new TimedVariableReporter<T>(c, v, i);
  addReporter(reporter);
}

template<class T>
void StreamHandler::addOnChangeVariableReporter(char c, T& v) {
  OnChangeVariableReporter<T>* reporter = new OnChangeVariableReporter<T>(c, v);
  addReporter(reporter);
}


#endif  //STREAM_HANDLER_H