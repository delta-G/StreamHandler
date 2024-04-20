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

#include "StreamCommand.h"

// // parses an integer from the string
// template<>
// int __attribute__((weak)) VariableUpdater<int>::parse(char* str) {
//   return atoi(str + 1);  // skip command character
// }
// // parses an double from the string
// template<>
// float __attribute__((weak)) VariableUpdater<float>::parse(char* str) {
//   return atof(str + 1);  // skip command character
// }

// template<>
// void __attribute__((weak)) VariableUpdater<int>::display(char* ret) {
//   snprintf(ret, STREAM_HANDLER_MAX_LENGTH, "<%c%d>", matchChar, var);
// }

// template<>
// void __attribute__((weak)) VariableUpdater<float>::display(char* ret) {
//   char buf[16];
//   dtostrf(var, 2, 2, buf);
//   snprintf(ret, STREAM_HANDLER_MAX_LENGTH, "<%c%s>", matchChar, buf);
// }