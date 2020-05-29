//This is the header code for the CJString support code
//Pre-compiler directive
#ifndef STRING_UTIL_H
#define STRING_UTIL_H

//Headers
#include <stdio.h> //file operands
#include <stdlib.h> // memerory operands such as "free"


//Global Constants
//Boolean
typedef enum {True, False} Boolean;

//Error code data structure
typedef enum {NO_ERR,
              INCOMPLETE_FILE_ERR,
              INPUT_BUFFER_OVERRUN_ERR} StringManipCode;

extern const char NULL_CHAR;
extern const char SPACE;
extern const int MAX_STR_LEN;
extern const int STD_STR_LEN;
extern const char COLON;
extern const char PERIOD;
extern const char SEMICOLON;
extern const char LEFT_PAREN;
extern const char RIGHT_PAREN;
extern const Boolean IGNORE_LEADING_WS;
extern const Boolean ACCEPT_LEADING_WS;
extern const int STR_EQ;

//Prototyped Functions
int getStringLength(char *inStr );
int compareString (char *strOne, char *strTwo);

void copyString ( char *destination, char *source);

void setStrToLowerCase ( char *destStr, char *sourceStr);
char setCharToLowerCase ( char testChar );

int getLineTo( FILE *filePtr, int bufferSize, char stopChar, 
                   char *buffer, Boolean omitLeadingWhiteSpace );

Boolean isEndOfFile (FILE *filePtr);
void concatenateString(char *destination, char *source);

//TODO functions not yet implemented
/*
   getSubString
   findSubString
*/

#endif //of STRING_UTIL_H

