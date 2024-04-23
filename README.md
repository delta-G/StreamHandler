# StreamHandler

This code automates a bunch of parsing that normally happens around Stream objects.  
Streams are handled using the @Robin2 style code with end markers.  
By defulault, `'<'` and `'>'` are used for end markers.  

<br><br>

**`StreamHandler(Stream* input, Stream* output)`**
>The constructor takes two Stream objects.  The first is an input Stream and the second is the output Stream.  They can be the same.  If the output Stream is left out or is `NULL` then no output is sent. 

**`void run()`**
>Should be called from the main code as often as possible.

**`void setDefaultHandler(void(*f)(char*, char*));`**
>This sets a default handler that will be called if there is no match to the input command.  The function pointer `f` should point to a function that returns void and takes two char* arguments.  The first will be set to the input string.  The second will be set to point to the output buffer.  The function can write any sort of output to this buffer and it will be sent to the output Stream as soon as the function returns.  

**`void setGreedy(int)`**<br>
**`int getGreedy()`**
>These methods set and get the `greedy` setting.  This setting determines how many characters the StreamHandler is allowed to consume in one call to `run()`.  With the default setting of 0, the `run()` function will block and read until there are no more bytes available (`available() returns 0).  If greedy is set to a higher positive value then it will read a maximum of that many characters before returning.  

<br><br>
#Commands and Reporters

Two base class types are defined, `StreamCommand` and `StreamReporter` that handle common
 operations.

The `StreamCommand` objects each have a char that can be matched against the first character after the start marker in the incoming string.  If a match is found, then the entire input string is sent to a `handle` method.  Different derived classes handle the string in different ways from calling commands to updating variable values.  There are also types that allow for a return string to be sent back.

The `StreamReporter` objects handle unsolicited outgoing messages.  Examples of these are classes that call functions at regular intervals to generate output strings or classes that send updates whenever a variable changes.  

The `StreamHandler` class strips the start and end markers from the input and adds them to the output.  So there is no need to even think about them in the `StreamCommand` or `StreamReporter` classes.  This makes it easy to change markers later without having to find all the strings that include them.  

StreamHandler contains facotry methods to build the different commands and reporters.  

<br><br>
# Commands

These all require as a first argument a char.  This char must be unique.  If you try to add a command that already exists then the command will not be added.  This character is matched against the first character after the start marker in the incoming string to determine which command to run.  

**`addFunctionCommand(char c, void(*f)(char*))`** 
>This function creates the most basic type of function caller.  The function pointer `f` should point to a function that returns void and takes a char* as the argument.  When called, the entire input string will be passed to this function including the command character but not the end markers.  You are free to include anything in this message that you want. 

**`addReturnCommand(char c, void(*f)(char*, char*))`**
>This function is similar to `addFunctionCommand` except that the function pointer must point to a function that takes two char* arguments.  The first will point to the input string with end markers stripped.  The second will point to an output buffer where you can write a return string up to `STREAM_HANDLER_MAX_LENGTH` bytes long.  This string will be sent to the output Stream as soon as your function returns.  

**`addVariableUpdater(char c, T& v, bool e = true)`**
>This function takes a reference to any type of variable.  It will call a parser and update the given variable with whatever value follows the command character.  The optional third boolean argument determines whether the new parsed value will be echoed back to the output (default is true).  Currently parsers are implemented for float and int, but they are marked as weak and can be overridden if you wish to implement your own parsing routine.  

<br><br>
#Reporters

These functions add Reporter objects to the StreamHandler.  Reporter objects handle sending output to the output Stream without needing to be called by a command in the input.  

**`addTimedFunctionReporter(void(*f)(char*), unsigned long i)`**
>This function takes a pointer to a function that returns void and takes a char* argument.  It also takes an interval in milliseconds.  The function will be called every `i` milliseconds and the output buffer will be passed as the argument.  The function may write any desired output to that buffer and it will be sent to the output Stream as soon as the function returns.  

**`addTimedVariableReporter(char c, T& v, unsigned long i)`**
>This function takes an id char and a variable of any type and an interval.  Every `i` milliseconds a packet will be created containing the id char and the value of the variable.  The variable is stringified by an internal function.  Like the parser for the VariableUpdater, it can be overridden to display values however you want.  Currently float and int are implemented.  

**`addOnChangeVariableReporter(char c, T& v)`**
>This function creates a reporter similar to the `TimedVariableReporter` except that it will send the report any time the value of the variable changes.  In order to use this, the type must have a `!=` operator defined.  

<br><br>
#StreamParser

The `StreamParser` base class defines the base parser for the different types.  It is a template class and the fully specialized implementation must be provided for all types.  The library includes:<br>

|Included Types |
|---------------|
|`int`          |
|`unsigned int` |
|`long`         |
|`unsigned long`|
|`float`        |
|`double`       |

Other types can be added by simply implementing a `parse` method for them.  This can be done directly in the sketch, the code does not need to be added to the library.  The `parse` method must take a char* argument that will point to the string containing the value to be parsed.  It should return the type you are trying to add and you must place the same type in the braces in the function definition.<br>
<br>
An example parser to add support for `uint8_t` might look like this:
```
template<>
uint8_t Parser<uint8_t>::parse(char* str) {
  return atoi(str + 1);  // skip command character
}
```
Note the `template<>` before the definition and the `<type>` in the definition where type is the type you are wanting to add.  

<br>
<br>
#StreamFormatter

The `StreamFormatter` class does the opposite job of the `StreamParser` class.  This class takes a type and turns it back into a string for delivery to serial.  Similar to the parsers, the `StreamFormatter` class is also set up as a template so that additional types can easily be added.<br><br>
All of the types that are implemented in the library for the parser are also implemented for the formatter.  There is no need to do anything for those.  Adding additional types is very similar to adding a parser where you just write a function in your sketch.  You must implement a `format` method which takes a value of the type you are working with and a char pointer that points to the buffer where you must write the string representation. <br>
<br>
An example formatter to add support for `uint8_t` might look like this:

```
template<>
void Formatter<uint8_t>::format(uint8_t v, char* ret) {
  snprintf(ret, STREAM_HANDLER_BUFFER_SIZE, "%u", v);
}
```
Note the similar syntax to the parser.  This makes adding any type easy as long as you know how to convert it back and forth to a string.  

