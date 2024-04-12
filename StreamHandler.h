#ifndef STREAM_HANDLER_H
#define STREAM_HANDLER_H

#include "Arduino.h"

#ifndef STREAM_HANDLER_BUFFER_SIZE
#define STREAM_HANDLER_BUFFER_SIZE 64
#endif

/*
*
*    Base class for commands
*
*/
class Command {
  friend class StreamHandler;
private:
  const char matchChar;
  Command* next = nullptr;

  boolean match(const char* com) {
    return ((*com == matchChar));
  }
  Command();  // disallow default constructor
protected:
  virtual void handle(char* str) {
    (void)str;
  };
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

  void (*func)(char*);
  FunctionCommand();  // disallow default constructor

protected:

  virtual void handle(char* str) {
    if (func) func(str);
  };

public:

  FunctionCommand(char c, void (*f)(char*))
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

protected:

  virtual void handle(char* str) {
    var = parse(str);
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

  char _SHbuffer[STREAM_HANDLER_BUFFER_SIZE];
  int index;

  Stream* in;
  char sop;
  char eop;

  boolean receiving = false;
  boolean greedy = false;

  void handleChar(char c);

  Command* first;

public:

  StreamHandler(Stream* aIn, char aSop, char aEop)
    : index(0), in(aIn), sop(aSop), eop(aEop){};
  StreamHandler(Stream* aIn)
    : index(0), in(aIn), sop('<'), eop('>'){};
  void run();

  void setGreedy(bool);
  bool getGreedy();

  void addCommand(Command* com);
  void checkCommands();
};



#endif  //STREAM_HANDLER_H