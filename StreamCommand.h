#ifndef STREAMCOMMAND_H
#define STREAMCOMMAND_H

#include "Arduino.h"
#include "StreamHandlerDefines.h"

#ifndef DEFAULT_VU_ECHO
#define DEFAULT_VU_ECHO true
#endif


typedef void (*ComFuncPtr)(char*);
typedef void (*RetFuncPtr)(char*, char*);

/*
*
*    Base class for commands
*
*/
class StreamCommand {
  friend class StreamHandler;
private:
  StreamCommand* next = nullptr;
  StreamCommand* prev = nullptr;

  boolean match(char com) {
    return ((com == matchChar));
  }
  StreamCommand();  // disallow default constructor
protected:
  const char matchChar;
  virtual void handle(char* str, char* ret) = 0;
  StreamCommand(char c)
    : matchChar(c) {}

public:

  StreamCommand(const StreamCommand& other) = delete;
  StreamCommand& operator=(const StreamCommand& other) = delete;
  ~StreamCommand() {
    // Remove a link in the chain. 
    if(prev){
      prev->next = next;
    }
    if(next){
      next->prev = prev;
    }
  }
};

/*
*
*    Commands that call a function
*    function must take a char* and return void
*/
class FunctionCommand : public StreamCommand {
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
    : StreamCommand(c), func(f){};
};

/*
*
*    Commands that call a function and send back a return
*    function must take a char* for the command and a char* to put the return in
*    and return void
*/
class ReturnCommand : public StreamCommand {
private:

  RetFuncPtr func;
  ReturnCommand();  // disallow default constructor

protected:

  virtual void handle(char* str, char* ret) {
    if (func) func(str, ret);
  };

public:

  ReturnCommand(char c, RetFuncPtr f)
    : StreamCommand(c), func(f){};
};

/*
*
*    Class for updating variables
*
*/

template<class T>
class VariableUpdater : public StreamCommand {
private:

  T& var;
  VariableUpdater();  // disallow default constructor
  T parse(char*);
  void display(char*);
  bool echo;

protected:

  virtual void handle(char* str, char* ret) {
    var = parse(str);
    if (echo) {
      display(ret);
    }
  }

public:

  VariableUpdater(char c, T& v, bool e)
    : StreamCommand(c), var(v), echo(e){};
  VariableUpdater(char c, T& v)
    : VariableUpdater(c, v, DEFAULT_VU_ECHO){};
};

#endif  //STREAMCOMMAND_H