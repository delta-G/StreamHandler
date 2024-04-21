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

// template<>
// class Formatter<int> {

// public:
//   const char* formStr;
//   virtual void format(int, char*);
//   Formatter(const char* f)
//     : formStr(f) {}
//   Formatter()
//     : Formatter((const char*)"%d") {}
//   virtual ~Formatter() {}
// };

// template<>
// class Formatter<unsigned int> {

// public:
//   const char* formStr;
//   virtual void format(unsigned int, char*);
//   Formatter(const char* f)
//     : formStr(f) {}
//   Formatter()
//     : Formatter((const char*)"%u") {}
//   virtual ~Formatter() {}
// };

// template<>
// class Formatter<long> {

// public:
//   const char* formStr;
//   virtual void format(long, char*);
//   Formatter(const char* f)
//     : formStr(f) {}
//   Formatter()
//     : Formatter((const char*)"%ld") {}
//   virtual ~Formatter() {}
// };

// template<>
// class Formatter<unsigned long> {

// public:
//   const char* formStr;
//   virtual void format(unsigned long, char*);
//   Formatter(const char* f)
//     : formStr(f) {}
//   Formatter()
//     : Formatter((const char*)"%lu") {}
//   virtual ~Formatter() {}
// };

template<>
class Formatter<float> {
public:
  int decimals;
  virtual void format(float, char*);
  Formatter(int d)
    : decimals(d) {}
  Formatter()
    : Formatter(2) {}
  virtual ~Formatter() {}
};

template<>
class Formatter<double> {
public:
  int decimals;
  virtual void format(double, char*);
  Formatter(int d)
    : decimals(d) {}
  Formatter()
    : Formatter(2) {}
  virtual ~Formatter() {}
};

#endif
