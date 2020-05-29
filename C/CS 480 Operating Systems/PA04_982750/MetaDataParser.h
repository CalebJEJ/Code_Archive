//Pre-compiler directive
#ifndef META_PARSER_H
#define META_PARSER_H

//headers
#include <stdio.h>
#include <stdlib.h>
#include "CJString.h"

//Global Constants

   //Enumeration of error codes for metadata parsing
   //starts a 3 to avoid string util error code conflict
typedef enum 
    {
      MD_FILE_ACCESS_ERR = 3, 
      MD_CORRUPT_DESC_ERR,
      CORRUPT_OPCMD_LETTER_ERR,
      OPCMD_ACCESS_ERR,
      UNBALANCED_START_END_ERR,
      CORRUPT_OPCMD_NAME_ERR,
      CORRUPT_OPCMD_VALUE_ERR,

      COMPLETE_OPCMD_FOUND_MSG,
      LAST_OPCMD_FOUND_MSG
    } MetaDataMsgCodes;

   //Data structure for an Op Code representation
          //It forms a single link, linked list
typedef struct OpCodeType 
    {
       char opLtr;
       char opName[100]; // length of op name = 99 characters
       int opValue;

       struct OpCodeType *next;
    } OpCodeType;

//Prototypes
int getOpCodes( char *fileName, OpCodeType **opCodeDataHead);
int getOpCommand( FILE *filePtr, OpCodeType *inData);

void displayMetaData( OpCodeType *localPtr );
void displayMetaDataError( int code );

OpCodeType *clearMetaDataList( OpCodeType *localPtr );

Boolean checkOpString( char *testStr );
Boolean isDigit( char testChar );

int updateEndCount( int count, char *opString );
int updateStartCount( int count, char *opString );

OpCodeType *addNode( OpCodeType *localPtr, OpCodeType *newNode );

#endif //of META_PARSER_H



