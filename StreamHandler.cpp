#include "StreamHandler.h"

Command* Command::first = nullptr;  // initialize list head

// static function to check all commands agains the given string
void Command::check(char* str) {
  for (Command* ptr = first; ptr != nullptr; ptr = ptr->next) {
    // Start markers should be stripped.
    if (ptr->match(str)) {
      ptr->handle(str);
      break;
    }
  }
}

// parses an integer from the string
template<>
int VariableCommand<int>::parse(char* str) {
  return atoi(str + 1);  // skip command character
}
// parses an double from the string
template<>
float VariableCommand<float>::parse(char* str) {
  return atof(str + 1);  // skip command character
}

/*
*
*    StreamHandler methods
*
*/

void StreamHandler::run() {

  //	if (receivingRaw) {
  //		handleRawData();
  //	} else
  if (in->available()) {
    do {
      char c = in->read();
      handleChar(c);
    } while (in->available() && greedy);
  }
}

void StreamHandler::handleChar(const char c) {

  if (c == sop) {
    receiving = true;
    index = 0;
    _SPbuffer[0] = 0;
  } else if (receiving) {
    if (c == eop) {
      receiving = false;
      Command::check(_SPbuffer);
    }
    _SPbuffer[index] = c;
    _SPbuffer[++index] = 0;

    if (index >= STREAM_HANDLER_BUFFER_SIZE - 1) {
      index--;
    }
  }
}

void StreamHandler::setGreedy(bool aBoo) {
  greedy = aBoo;
}

bool StreamHandler::getGreedy() {
  return greedy;
}
