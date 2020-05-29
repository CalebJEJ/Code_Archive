#include "CJString.h"

// Global Constants
const int MAX_STR_LEN = 20000;
const int STD_STR_LEN = 80;
const int STR_EQ = 0;
const char NULL_CHAR = '\0';
const char SPACE = ' ';
const char COLON = ':';
const char PERIOD = '.';
const char SEMICOLON = ';';
const char LEFT_PAREN = '(';
const char RIGHT_PAREN = ')';
const Boolean IGNORE_LEADING_WS = True;
const Boolean ACCEPT_LEADING_WS = True;

/*
Function name: getString Length
Algorithm: finds the lenght of the string and returns it as an int
Precondition: given a series of characters with a null character at the end
Postcondition: returns the number of chars from the beginning to the null char
		as an int value
Exceptions: none
Note: Imposes a max character limit for safty purposes
*/

int getStringLength(char *inStr )
   {
      //initialize
      int index = 0;

      //loop until the max lenght is reached or the null char is
      while( index < MAX_STR_LEN && *(inStr + index ) != NULL_CHAR ) 
         {
            //increment index
            index++;
         }


      //return index value
      return index;
   }

/* 
compares two strings and returns one of the following:
          the given strings are exactly equal: STR_EQ
          strOne < strTwo : some int < 0
          strOne > strTwo : some int > 0

          if the strings are equal up to 
          a point where one is longer : the difference in length

*/
int compareString (char *strOne, char *strTwo) 
   {
      //initialize a index and a difference to return
      int index = 0;
      int difference;

      //begin loop until the end of either string is reached
      while( index < MAX_STR_LEN
             && *(strOne + index) != NULL_CHAR
             && *(strTwo + index) != NULL_CHAR )
      {
         //get the difference between the aligned characters
         difference = *(strOne + index) - *(strTwo + index);

         //check if any diffrences in characters have been found
                //return the difference if so
         if( difference != 0 )
         {
            return difference;
         }
      //no differences found so increment and continue

      index++;

      }//end of loop until any string ends

      //the strings must be equal up until some length
          //thus return the difference
      return getStringLength(strOne) - getStringLength(strTwo);
   }

//inserts the data as a copy from the source to the destination
void copyString ( char *destination, char *source)
   {
      int index = 0;

      //begin loop until the null character is reached
      //loop is limited to the maximum string length
      while( index < MAX_STR_LEN && *(source + index) != NULL_CHAR )
      {
         // assign the source character to the destination
         *(destination + index) = *(source + index);

         //increment the index
         index++;

         //assign a null character to the next destination element
         *(destination + index) = NULL_CHAR;
         
      }//end of while loop until null char


      //no return
   }

//performs the same function as copyString,
//BUT edits the string to be lowercase
void setStrToLowerCase ( char *destStr, char *sourceStr)
   {
       //create a temp string for safety
       int strLen = getStringLength(sourceStr);
       char *tempStr = (char *) malloc(strLen + 1);

       //make an index for later
       int index = 0;

       //copy over the source string into the temp
       copyString( tempStr, sourceStr );

       //loop till the end of the temp string 
               //which contains a copy of the source
       while( index < MAX_STR_LEN
              && tempStr[index] != NULL_CHAR )
       {
          //set the destination char at the index to
          //its lowercase equivalent
          destStr[index] = setCharToLowerCase( tempStr[index] );

          //incremnt the index
          index++;

          //set the next destination character at index
          //to a null character
          destStr[index] = NULL_CHAR;

       }//end of loop through the temp string

       //clean up memeory by freeing our temp
       free(tempStr);
       
       //no return as this func modifies the destination string
   }


//takes in an upper case character and returns its lower case equivalent
//passes any lowercase values out untouched
char setCharToLowerCase ( char testChar )
   {
      //check for an uppercase letter
        //between A and Z inclusive
      if( testChar >= 'A'
          && testChar <= 'Z' )
      {
         //convert the char to lowercase
         testChar = (char) ( testChar - 'A' + 'a');
      }


      return testChar;
   }

int getLineTo( FILE *filePtr, int bufferSize, char stopChar, 
                            char *buffer, Boolean omitLeadingWhiteSpace )
   {
      //initailize vairiables

         //index
      int charIndex = 0;

         //status for return as NO_ERR code
      int statusReturn = NO_ERR;

         //buffer size availbe flag as True
      Boolean bufferSizeAvailable = True;

         //a variable to hold a character as an int
      int charAsInt;

      //get the first character in the file
      charAsInt = fgetc( filePtr );

      //loop to consume white space, IF flagedd
      while( omitLeadingWhiteSpace == True
             && charAsInt != (int) stopChar
             && charIndex < bufferSize
             && charAsInt <= (int) SPACE )
      {
         //get the character as an int
         charAsInt = fgetc( filePtr );

      }//end of white space eating loop

      //capture the string
      //loop until the stop chracter or the buffer size is reached
      while( charAsInt != (int) stopChar
             && bufferSizeAvailable == True ) 
      {
         //check for input failure
         if( isEndOfFile( filePtr ) == True )
         {
            //return the code for incomplete file
            return INCOMPLETE_FILE_ERR;
         }

         //check for non-whitespace character
         if( charAsInt >= (int) SPACE )
         {
            //assing it to the buffer
            buffer[ charIndex ] = (char) charAsInt;

            //move on the the next character (index)
            charIndex++;
         }

         // set next buffer element to the null character
         buffer[ charIndex ] = NULL_CHAR;

         //check for not at end of buffer size
         if( charIndex < bufferSize - 1 )
         {
            //get a new character
            charAsInt = fgetc( filePtr );
         }
           else //we are at the end of the buffer size
           {
              //set the buffer size to false to end loop
              bufferSizeAvailable = False;

              //set the return to the buffer overrun err code
              statusReturn = INPUT_BUFFER_OVERRUN_ERR;
           }

      }//end of buffer loop


      //give the status report
      return statusReturn;
   }

//returns True if the file pointer given is at the end of the file,
//returns False otherwise
//functionally replaces feof to improve readablitly and consistancy
Boolean isEndOfFile (FILE *filePtr)
   {
      //give True for any return thats not 0
      if( feof(filePtr) != 0)
      {
         return True;
      }

      return False;
   }
   
//concatenates the source to the destination string
void concatenateString(char *destination, char *source)
{
   int destIndex = getStringLength(destination);
   
   int sourceIndex = 0;
   
   while( sourceIndex < MAX_STR_LEN && source[ sourceIndex ] != NULL_CHAR )
   {
       destination[ destIndex ] = source[ sourceIndex ];
       
       sourceIndex++;
       destIndex++;
       
       destination[ destIndex ] = NULL_CHAR;
   }
}



