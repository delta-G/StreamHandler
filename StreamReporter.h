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

#ifndef STREAMREPORTER_H
#define STREAMREPORTER_H

#include "Arduino.h"
#include "StreamHandlerDefines.h"
#include "StreamParser.h"

typedef void (*RepFuncPtr)(char*);

class StreamReporter {
  friend class StreamHandler;
private:
  StreamReporter* next;
  StreamReporter* prev;

protected:
  bool isRawOut = false;
  virtual void handle(char*) = 0;

  StreamReporter() {}

public:

  void setRawOut(bool r = true) {
    isRawOut = r;
  }

  StreamReporter(const StreamReporter& other) = delete;
  StreamReporter& operator=(const StreamReporter& other) = delete;
  virtual ~StreamReporter() {
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
  Formatter<T>* formatter;
  void format(char* out) {
    out[0] = codeChar;
    formatter->format(var, out + 1);
  }

public:
  VariableReporter* setFormatter(Formatter<T>* f) {
    delete formatter;
    formatter = f;
    return this;
  }
  VariableReporter(char c, T& v)
    : codeChar(c), var(v), formatter(new Formatter<T>()) {}
  ~VariableReporter() {
    delete formatter;
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
      VariableReporter<T>::format(out);
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
      VariableReporter<T>::format(out);
    }
  }

public:
  OnChangeVariableReporter(char c, T& v)
    : VariableReporter<T>(c, v), last(v) {}
};
#endif  //STREAMREPORTER_H