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

/*
*
*   Parser Commands
*
*/
template<>
int Parser<int>::parse(char* str) {
  return atoi(str);
}

template<>
float Parser<float>::parse(char* str) {
  return atof(str);
}
/*
*
*   Formatter Commands
*
*/
template<>
void __attribute__((weak)) Formatter<int>::format(int v, char* out) {
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, "%d", v);
}

template<>
void __attribute__((weak)) Formatter<float>::format(float v, char* out) {
  char buf[16];
  dtostrf(v, 2, 2, buf);
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, "%s", buf);
}
