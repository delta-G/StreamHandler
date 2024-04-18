#ifndef STREAM_HANDLER_H
#define STREAM_HANDLER_H

#include "Arduino.h"

#ifndef STREAM_HANDLER_BUFFER_SIZE
#define STREAM_HANDLER_BUFFER_SIZE 64
#endif

#ifndef DEFAULT_SOP
#define DEFAULT_SOP '<'
#endif

#ifndef DEFAULT_EOP
#define DEFAULT_EOP '>'
#endif

typedef void (*ComFuncPtr)(char*);
typedef void (*RetFuncPtr)(char*, char*);

/*
*
*    Base class for commands
*
*/
class Command {
  friend class StreamHandler;
private:
  Command* next = nullptr;

  boolean match(char com) {
    return ((com == matchChar));
  }
  Command();  // disallow default constructor
protected:
  const char matchChar;
  virtual void handle(char* str, char* ret) = 0;
  Command(char c)
    : matchChar(c) {}

public:

  Command(const Command& other) = delete;
  Command& operator=(const Command& other) = delete;
  ~Command() {}
};

/*
*
*    Commands that call a function
*    function must take a char* and return void
*/
class FunctionCommand : public Command {
private:

  ComFuncPtr func;
  FunctionCommand();  // disallow default constructor

protected:

  virtual void handle(char* str, char* ret) {
    (void)ret;
    if (func) func(str);
  };

public:

  FunctionCommand(char c, ComFuncPtr f)
    : Command(c), func(f){};
};

/*
*
*    Commands that call a function and send back a return
*    function must take a char* for the command and a char* to put the return in
*    and return void
*/
class ReturnCommand : public Command {
private:

  RetFuncPtr func;
  ReturnCommand();  // disallow default constructor

protected:

  virtual void handle(char* str, char* ret) {
    if (func) func(str, ret);
  };

public:

  ReturnCommand(char c, RetFuncPtr f)
    : Command(c), func(f){};
};

/*
*
*    Class for updating variables
*
*/

template<class T>
class VariableUpdater : public Command {
private:

  T& var;
  VariableUpdater();  // disallow default constructor
  T parse(char*);
  void display(char*);

protected:

  virtual void handle(char* str, char* ret) {
    var = parse(str);
    display(ret);
  }

public:

  VariableUpdater(char c, T& v)
    : Command(c), var(v){};
};


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
  boolean greedy = false;

  void handleChar(char c);

  Command* first;

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

  void setGreedy(bool);
  bool getGreedy();

  void addCommand(Command*);
  void addFunctionCommand(char, ComFuncPtr);
  void addReturnCommand(char, RetFuncPtr);
  template<class T>
  void addVariableUpdater(char, T&);
  boolean commandExists(char);
  void checkCommands();
};

template<class T>
void StreamHandler::addVariableUpdater(char c, T& v) {
  if (!commandExists(c)) {
    VariableUpdater<T>* updater = new VariableUpdater<T>(c, v);
    addCommand(updater);
  }
}



#endif  //STREAM_HANDLER_H