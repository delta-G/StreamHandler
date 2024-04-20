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

#include "StreamParser.h"

Parser defaultParser = Parser();
Formatter defaultFormatter = Formatter();
/*
*
*   Parser Commands
*
*/
template<>
int Parser::parse<int>(char* str) {
  return atoi(str);
}

template<>
float Parser::parse<float>(char* str) {
  return atof(str);
}
/*
*
*   Formatter Commands
*
*/
template<>
void __attribute__((weak)) Formatter::format<int>(int v, char* out) {
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, "%d", v);
}

template<>
void __attribute__((weak)) Formatter::format<float>(float v, char* out) {
  char buf[16];
  dtostrf(v, 2, 2, buf);
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, "%s", buf);
}
