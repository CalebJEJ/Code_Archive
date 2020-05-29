//Header
#include "MetaDataParser.h"


/*
Function: getOpCodes
Input: file name and an op code data head pointer
Output: The new op code data structure
Returns: An int value representing a error code
Notes: an op code data structure is a linked list data type
*/
int getOpCodes( char *fileName, OpCodeType **opCodeDataHead)
   {
       //initialize variables
          //read only flag constant for code readability
       const char READ_ONLY_FLAG[] = "r";

          //start and end counts to balance app operations
       int startCount = 0;
       int endCount = 0;

          //init the local head pointer
       OpCodeType *localHeadPtr = NULL;

          //init other varaibles
       int accessResult;
       char dataBuffer[ MAX_STR_LEN ];
       OpCodeType *newNodePtr;
       FILE *fileAccessPtr;

          //in case of a return error set the head to null
       *opCodeDataHead = NULL;

          //open the file for reading
       fileAccessPtr = fopen( fileName, READ_ONLY_FLAG );

          //check for file open failure
       if(fileAccessPtr == NULL)
       {
          return MD_FILE_ACCESS_ERR;
       }

      //check first line : START
      if( getLineTo( fileAccessPtr, MAX_STR_LEN, COLON,
                          dataBuffer, IGNORE_LEADING_WS ) != NO_ERR
          || compareString( dataBuffer, "Start Program Meta-Data Code")
                                                          != STR_EQ)
        {
           //first line check has failed
              //close the file
           fclose( fileAccessPtr );
 
             //return corrupt description error
           return MD_CORRUPT_DESC_ERR;
            
        }//end of if that checks starting line

      //allocate the memory for the temporary op code data structure
      newNodePtr = ( OpCodeType * ) malloc( sizeof( OpCodeType ) );

      //get the first op command
      accessResult = getOpCommand( fileAccessPtr, newNodePtr );


      //update start and end counters
      startCount = updateStartCount( startCount, newNodePtr->opName );
      endCount = updateEndCount( endCount, newNodePtr->opName );

      //verify the op command as a valid one, if not return err code
      if( accessResult != COMPLETE_OPCMD_FOUND_MSG )
      {
         //the op command is invalid
            //close the file
         fclose( fileAccessPtr );

         //clear data from the current structure
         *opCodeDataHead = clearMetaDataList( localHeadPtr );

         //free the temp data node
         free( newNodePtr );

         //return the access result
         return accessResult;

      }

      //first access is good, now loop through the remaining valid ops
      while( accessResult ==  COMPLETE_OPCMD_FOUND_MSG )
      {
         //add the new valid op command to the link list
         localHeadPtr = addNode( localHeadPtr, newNodePtr );

         //get the next op command
         accessResult = getOpCommand( fileAccessPtr, newNodePtr );

         //update start and end counters
         startCount = updateStartCount( startCount, newNodePtr->opName );
         endCount = updateEndCount( endCount, newNodePtr->opName );

      }//end of loop over valid op commands

      //verify the last op command as the ending op code
      if( accessResult == LAST_OPCMD_FOUND_MSG )
      {
         //verfiy that the start and end counters are equal
         if( startCount == endCount ) 
         {
            //add the last node in
            localHeadPtr = addNode( localHeadPtr, newNodePtr );

            //set the access result to be returned at the end of the function
            accessResult = NO_ERR;

            //check the last file line for valid end string
            if( getLineTo( fileAccessPtr, MAX_STR_LEN, PERIOD,
                          dataBuffer, IGNORE_LEADING_WS ) != NO_ERR
                || compareString( dataBuffer, "End Program Meta-Data Code")
                                                          != STR_EQ)
            {
               //end string is invalid
                 //set the access result to an err code
               accessResult = MD_CORRUPT_DESC_ERR;
            }

         }//end of if the validates sart and end equlity
           else   //assume counters are unbalenced
           {
              accessResult = UNBALANCED_START_END_ERR;
           }

      }//end of if that checks for last op code

      //check for any access errors
      if( accessResult != NO_ERR )
      {
         localHeadPtr = clearMetaDataList( localHeadPtr );
      }

      //close the file
      fclose( fileAccessPtr );

      //assing the temp local head pointer to the returned pointer
      *opCodeDataHead = localHeadPtr;

      //free up the temp node
      free( newNodePtr );

      //return the access result
      return accessResult;
   }

/*
Function: getOpCommand
Input: file pointer and an op code data
Output: The new op code data into the op data that was input
Returns: An int value representing a error code
*/
int getOpCommand( FILE *filePtr, OpCodeType *inData)
   {
      //inititalize variables
         //create some constants for maximums
      const int MAX_OP_NAME_LENGTH = 10;
      const int MAX_OP_VALUE_LENGTH = 9;

      //create buffer
      int intBuffer = 0;

      //create some indices starting at zero
      int sourceIndex = 0;
      int destIndex = 0;

      //create a varialbe for holding the result error code
      //and create a string buffer
      int accessResult;
      char strBuffer[ STD_STR_LEN ];

      //get the entire op command as a string
      accessResult = getLineTo( filePtr,  STD_STR_LEN, SEMICOLON,
                                           strBuffer, IGNORE_LEADING_WS );

      //check if the access was successful
      if(accessResult == NO_ERR )
      {
         inData->opLtr = strBuffer[ sourceIndex ];
      }
        else //assume acces failed
        {
           //set the pointer in the data structure to null
           inData = NULL;
        
           return OPCMD_ACCESS_ERR;
        }

      //verfiy that the command letter is valid
      switch( inData->opLtr )
      {
         case 'S':
         case 'A':
         case 'P':
         case 'M':
         case 'I':
         case 'O':
            break;

         //if it defaults its not a valid letter
         default:
             //set the pointer in the data structure to null
             inData = NULL;

             //return op command error letter
             return CORRUPT_OPCMD_LETTER_ERR;
      }
 
      //move until you reach a left parenthesis is found
      while( sourceIndex < STD_STR_LEN && strBuffer[ sourceIndex ] != LEFT_PAREN )
      {
         sourceIndex++;
      }

      //skip over the parenthesis
      sourceIndex++;

      //read in the op command text up until the right parenthesis
      while( sourceIndex <  STD_STR_LEN
             && destIndex < MAX_OP_NAME_LENGTH
             && strBuffer[ sourceIndex ] != RIGHT_PAREN )
      {
         //read in one letter at a time
         inData->opName[ destIndex ] = strBuffer[ sourceIndex ];
         
         //increment the indicies
         destIndex++; sourceIndex++;

         //set the null to the current end of the string
         inData->opName[ destIndex ] = NULL_CHAR;

      }//end of loop that reads in the op name

      //verify the op string
      if( checkOpString( inData->opName ) == False )
      {
         //set the structure to null
         inData = NULL;

         //return the error code for incorrect op name/command
         return CORRUPT_OPCMD_NAME_ERR;
      }

      //reset the destination index
      destIndex = 0; 
      //skip over the right parenthesis
      sourceIndex++;

      //read in the op value
      while( sourceIndex <  STD_STR_LEN
             && destIndex < MAX_OP_VALUE_LENGTH
             && isDigit( strBuffer[ sourceIndex ] ) == True )
      {
         //multiply the int buffer by 10
         intBuffer *= 10;

         //add next interger value to convert from a character to an int
         intBuffer += (int) ( strBuffer[sourceIndex] - '0' );
         
         //increment the indicies
         destIndex++; sourceIndex++;

      }//end of loop that reads in the op value

      //check for loop overrun and if so give an error
      if( sourceIndex == STD_STR_LEN 
          || destIndex == MAX_OP_VALUE_LENGTH )
      {
         //set the structure to null
         inData = NULL;

         //return the error for corrupt op value
         return CORRUPT_OPCMD_VALUE_ERR;
      }

      //the value is valid so we will set it in the structure
      inData->opValue = intBuffer;

      //check to see if this command is the last op command
             //last command : "S(end)0"
      if( inData->opLtr == 'S'
          && compareString ( inData->opName, "end" ) == STR_EQ )
      {
         // return the message that this is the last command
         return LAST_OPCMD_FOUND_MSG;
      }

    // return the message that a complete op code was found
    return COMPLETE_OPCMD_FOUND_MSG;
   }

/*
Function: checkOpString
Input: a string to verify
Output: a Boolean to dictate pass (True) or fail (False)
Pass: the string is a valid op code representation
Fail: any not valid
Returns: Boolean
*/
Boolean checkOpString( char *testStr )
   {
     //check all possiblities
     if( compareString( testStr, "access" ) == STR_EQ 
         ||compareString( testStr, "allocate" ) == STR_EQ 
         ||compareString( testStr, "end" ) == STR_EQ 
         ||compareString( testStr, "hard drive" ) == STR_EQ 
         ||compareString( testStr, "keyboard" ) == STR_EQ 
         ||compareString( testStr, "printer" ) == STR_EQ 
         ||compareString( testStr, "monitor" ) == STR_EQ 
         ||compareString( testStr, "run" ) == STR_EQ 
         ||compareString( testStr, "start" ) == STR_EQ ) 
     {
        //valid string found => return True
        return True;
     }

     //failed to find valid string => return false
     return False;
   }

/*
Function: isDigit
Input: a char to verify
Output: a Boolean to dictate pass (True) or fail (False)
Pass: the char is a digit between 0 and 9 inclusive
Fail: any other char
Returns: Boolean
*/
Boolean isDigit( char testChar )
   {
     //check all possiblities
     if( testChar == '0'
         || testChar == '1' 
         || testChar == '2' 
         || testChar == '3'
         || testChar == '4'
         || testChar == '5'
         || testChar == '6'
         || testChar == '7'
         || testChar == '8' 
         || testChar == '9' ) 
     {
        //valid value found => return True
        return True;
     }

     //failed to find valid value => return false
     return False;
   }


//funtion uses printf to display the meta data op code list
void displayMetaData( OpCodeType *localPtr )
   {
      //begin the display of data
      printf("\n");
      //loop through the linked list and print the contents
      while( localPtr != NULL )
      {
         //start with a new line each time
         printf("\n");

         //print op code letter
         printf( "Op Code Letter  : %c\n", localPtr->opLtr );

         //print op code letter
         printf( "Op Code Name    : %s\n", localPtr->opName );

         //print op code letter
         printf( "Op Code Value   : %d\n", localPtr->opValue );

         //move to the next node
         localPtr = localPtr->next;
      }

      //no return or setting of data, simply displays
   }

//funtion uses printf to display the meta data error code
      //DISPLAYS THE ERROR EXACTLY WITHOUT NEW LINES OR EXTRA FLUFF
void displayMetaDataError( int code )
   {
       //check the error code and give its corresponding message
       switch (code)
          {
             case NO_ERR: 
                  printf( "NO ERRORS : SUCCESS" );
                  break;
             case INCOMPLETE_FILE_ERR: 
                  printf( "INCOMPLETE FILE ERROR" );
                  break;
             case INPUT_BUFFER_OVERRUN_ERR: 
                  printf( "INPUT BUFFER OVERRUN ERROR" );
                  break;

             case MD_FILE_ACCESS_ERR: 
                  printf( "FAILED TO ACCESS FILE ERROR" );
                  break;
             case MD_CORRUPT_DESC_ERR: 
                  printf( "META DATA DESCRIPTOR CORRUPT ERROR" );
                  break;
             case CORRUPT_OPCMD_LETTER_ERR: 
                  printf( "CORRUPT OP COMMAND LETTER ERROR" );
                  break;
             case CORRUPT_OPCMD_NAME_ERR: 
                  printf( "CORRUPT OP COMMAND NAME ERROR" );
                  break;
             case CORRUPT_OPCMD_VALUE_ERR: 
                  printf( "CORRUPT OP COMMAND VALUE ERROR" );
                  break;
             case UNBALANCED_START_END_ERR: 
                  printf( "UNBALANCED START/END CODE ERROR" );
                  break;
             case OPCMD_ACCESS_ERR: 
                  printf( "OP COMMAND ACCESS ERROR" );
                  break;

             default:
                  printf( "UNKNOWN ERROR" );
                  break;
          }

      //no return or setting of data, simply displays error messages
   }

//function for clearing out all meta data at a pointer
      //gives back the pointer head as a return
OpCodeType *clearMetaDataList( OpCodeType *localPtr )
   {
      //check the local pointer for null => list not yet empty
      if( localPtr != NULL )
      {
        //check the pointer for next for null value
        if( localPtr->next != NULL )
        {
           //there is more in the list so clear that out first
               //accomplished recursivly
           clearMetaDataList( localPtr->next );
        }
         //assume we are at the end of the current list
            //free the end thing in the list
         free( localPtr );
      }


      //returns null to the calling func
      return NULL;
   }

/*
Function : updateStartCount
Each increments the count value given to it by 1 and returns it
      IF: the function finds the "start" command in the op string 
*/

int updateStartCount( int count, char *opString )
   {
      //check for "start" and if so increment count
      if( compareString( opString, "start" ) == STR_EQ ) 
      {
         //found start so return an incremented count
         return count + 1;
      }

      //not found dont increment
      return count;
   }

/*
Function : updateEndCount
Each increments the count value given to it by 1 and returns it
      IF: the function finds the "end" command in the op string 
*/
int updateEndCount( int count, char *opString )
   {
      //check for "end" and if so increment count
      if( compareString( opString, "end" ) == STR_EQ ) 
      {
         //found start so return an incremented count
         return count + 1;
      }

      //not found dont increment
      return count;
   }

/*
Function: addNode

Purpose: adds a new op command structure to the link of an
         existing op command structure

Assumes: Memory access/availablility
*/
OpCodeType *addNode( OpCodeType *localPtr, OpCodeType *newNode )
   {
      //check to see if local pointer is null
      if( localPtr == NULL )
      {
         //access the memory for new node
         localPtr = ( OpCodeType * ) malloc( sizeof( OpCodeType ) );

         //assign all of the values to the new node
         localPtr->opLtr = newNode->opLtr;
         copyString( localPtr->opName, newNode->opName );
         localPtr->opValue = newNode->opValue;
         localPtr->next = NULL;


         //return the local pointer
         return localPtr;
      }
      //assume the end of the list is not found yet
      //recurvily call this function on the local next link
      localPtr->next = addNode( localPtr->next, newNode );

      //return the current local pointer
      return localPtr;
   }






