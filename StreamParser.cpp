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
  return strtol(str, NULL, base);
}

template<>
unsigned int Parser<unsigned int>::parse(char* str) {
  return strtoul(str, NULL, base);
}

template<>
long Parser<long>::parse(char* str) {
  return strtol(str, NULL, base);
}

template<>
unsigned long Parser<unsigned long>::parse(char* str) {
  return strtoul(str, NULL, base);
}

template<>
float Parser<float>::parse(char* str) {
  return atof(str);
}

template<>
double Parser<double>::parse(char* str) {
  return atof(str);
}

/*
*
*   Formatter Commands
*
*/

const char* defaultIntFormat = "%d";
const char* defaultUnsignedIntFormat = "%u";
const char* defaultLongFormat = "%ld";
const char* defaultUnsignedLongFormat = "%lu";
const char* defaultFloatFormat = "%-1.2f";
const char* defaultDoubleFormat = "%-1.2f";

template<>
void __attribute__((weak)) Formatter<int>::format(int v, char* out) {
  if (formStr == nullptr) {
    formStr = defaultIntFormat;
  }
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, formStr, v);
}

template<>
void __attribute__((weak)) Formatter<unsigned int>::format(unsigned int v, char* out) {
  if (formStr == nullptr) {
    formStr = defaultUnsignedIntFormat;
  }
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, formStr, v);
}

template<>
void __attribute__((weak)) Formatter<long>::format(long v, char* out) {
  if (formStr == nullptr) {
    formStr = defaultLongFormat;
  }
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, formStr, v);
}

template<>
void __attribute__((weak)) Formatter<unsigned long>::format(unsigned long v, char* out) {
  if (formStr == nullptr) {
    formStr = defaultUnsignedLongFormat;
  }
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, formStr, v);
}


void __attribute__((weak)) Formatter<float>::format(float v, char* out) {
#if defined NO_FLOAT_F
  dtostrf(v, 2, decimals, out);
#else
  if (formStr == nullptr) {
    snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, "%-1.*f", decimals, v);
  }
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, formStr, v);
#endif
}


void __attribute__((weak)) Formatter<double>::format(double v, char* out) {
#if defined NO_FLOAT_F
  dtostrf(v, 2, decimals, out);
#else
  if (formStr == nullptr) {
    snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, "%-1.*f", decimals, v);
  }
  snprintf(out, STREAM_HANDLER_MAX_LENGTH - 2, formStr, v);
#endif
}
