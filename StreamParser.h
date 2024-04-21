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

#ifndef STREAM_PARSER_H
#define STREAM_PARSER_H

#if defined ARDUINO_ARCH_AVR
#define NO_FLOAT_F
#endif

#include "Arduino.h"
#include "StreamHandlerDefines.h"
/*
*
*   PARSER
*
*/

template<class T>
class Parser {

public:
  int base;
  virtual T parse(char*);
  Parser(int b)
    : base(b) {}
  Parser()
    : Parser(10) {}
  virtual ~Parser() {}
};

/*
*
*   FORMATTER
*
*/

template<class T>
class Formatter {

public:
  const char* formStr;
  virtual void format(T, char*);
  Formatter(const char* f)
    : formStr(f) {}
  Formatter()
    : Formatter(nullptr) {}
  virtual ~Formatter() {}
};



template<>
class Formatter<float> {
public:
  const char* formStr;
  int decimals;
  virtual void format(float, char*);
  Formatter(char* f)
    :  formStr(f), decimals(0) {}
  Formatter(int d)
    : formStr(nullptr), decimals(d) {}
  Formatter()
    : Formatter(2) {}
  virtual ~Formatter() {}
};

template<>
class Formatter<double> {
public:
  const char* formStr;
  int decimals;
  virtual void format(double, char*);
  Formatter(char* f)
    :  formStr(f), decimals(0) {}
  Formatter(int d)
    : formStr(nullptr), decimals(d) {}
  Formatter()
    : Formatter(2) {}
  virtual ~Formatter() {}
};

#endif
