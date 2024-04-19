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