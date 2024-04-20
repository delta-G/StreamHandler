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

class Parser {

public:
  template<class T>
  T parse(char*);
};

class Formatter {

public:
  template<class T>
  void format(T, char*);
};

extern Parser defaultParser;
extern Formatter defaultFormatter;

#endif
