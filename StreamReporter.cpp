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

#include "StreamReporter.h"

template<>
void __attribute__((weak)) VariableReporter<int>::display(char* ret) {
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "<%c%d>", codeChar, var);
}

template<>
void __attribute__((weak)) VariableReporter<float>::display(char* ret) {
  char buf[16];
  dtostrf(var, 2, 2, buf);
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "<%c%s>", codeChar, buf);
}