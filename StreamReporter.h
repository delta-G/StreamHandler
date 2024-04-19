#ifndef STREAMREPORTER_H
#define STREAMREPORTER_H

#include "Arduino.h"
#include "StreamHandlerDefines.h"

typedef void (*RepFuncPtr)(char*);

class StreamReporter {
  friend class StreamHandler;
private:
  StreamReporter* next;
  StreamReporter* prev;

protected:
  virtual void handle(char*) = 0;

  StreamReporter() {}

public:
  StreamReporter(const StreamReporter& other) = delete;
  StreamReporter& operator=(const StreamReporter& other) = delete;
  ~StreamReporter() {
    // Remove a link in the chain.
    if (prev) {
      prev->next = next;
    }
    if (next) {
      next->prev = prev;
    }
  }
};

class TimedFunctionReporter : public StreamReporter {

private:
  unsigned long previous = 0;
  unsigned long interval;
  RepFuncPtr func;

protected:
  virtual void handle(char* out) {
    unsigned long current = millis();
    if (current - previous >= interval) {
      previous = current;
      func(out);  // My new favorite line of code ever.
    }
  }

public:
  TimedFunctionReporter(RepFuncPtr f, unsigned long i)
    : StreamReporter(), interval(i), func(f) {}
};

template<class T>
class VariableReporter : public StreamReporter {
protected:
  char codeChar;
  T& var;
  void display(char*);

public:
  VariableReporter(char c, T& v)
    : codeChar(c), var(v) {}

  void send(char* out) {
    display(out);
  }
};

template<class T>
class TimedVariableReporter : public VariableReporter<T> {
private:
  unsigned long previous = 0;
  unsigned long interval;

protected:
  virtual void handle(char* out) {
    unsigned long current = millis();
    if (current - previous >= interval) {
      previous = current;
      VariableReporter<T>::display(out);
    }
  }

public:
  TimedVariableReporter(char c, T& v, unsigned long i)
    : VariableReporter<T>(c, v), interval(i) {}
};

template<class T>
class OnChangeVariableReporter : public VariableReporter<T> {
private:
  T last;

protected:
  virtual void handle(char* out) {
    if (VariableReporter<T>::var != last) {
      last = VariableReporter<T>::var;
      VariableReporter<T>::display(out);
    }
  }

public:
  OnChangeVariableReporter(char c, T& v)
    : VariableReporter<T>(c, v), last(v) {}
};
#endif  //STREAMREPORTER_H