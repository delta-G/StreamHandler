#include "StreamCommand.h"

// parses an integer from the string
template<>
int __attribute__((weak)) VariableUpdater<int>::parse(char* str) {
  return atoi(str + 1);  // skip command character
}
// parses an double from the string
template<>
float __attribute__((weak)) VariableUpdater<float>::parse(char* str) {
  return atof(str + 1);  // skip command character
}

template<>
void __attribute__((weak)) VariableUpdater<int>::display(char* ret) {
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "<%c%d>", matchChar, var);
}

template<>
void __attribute__((weak)) VariableUpdater<float>::display(char* ret) {
  char buf[16];
  dtostrf(var, 2, 2, buf);
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "<%c%s>", matchChar, buf);
}