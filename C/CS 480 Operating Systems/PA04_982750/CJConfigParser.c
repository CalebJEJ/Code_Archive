//Headers
#include "CJConfigParser.h"


/*
The function processConfig must:

Take a string representing a file name

Parse and save the data in this file

Output the SUCCESS code if successful
Output an appropraite error if any of the following:
    Cannot find file : 1
    File has corrupted data  : 2


FILE Format requirements:
    NO DUPLICATES
    NO MISSING DATA
    MUST CONFORM TO PROPER PREFIXS, START AND END LINE
    MUST BE IN THE FOLLOWING DATA TYPES: .cnf

*/
int processConfig(char* fileName, ConfigDataType **configData)
   {

      const int NUM_DATA_LINES = 9;
      int lineCounter = 0;

      // set read only constant to "r"
      const char READ_ONLY_FLAG[] = "r";

      //make a pointer for data
      ConfigDataType *tempData;
      
      //other variables
      FILE *fileAccessPtr;
      char dataBuffer [ MAX_STR_LEN ];
      int intData, dataLineCode;
      double doubleData;
      long longData;

      //init config data incase of return error
      *configData = NULL;

      //open file using fopen
      fileAccessPtr = fopen( fileName, READ_ONLY_FLAG);

      //verify file open
      if ( fileAccessPtr == NULL )
         {
           return CFG_FILE_ACCESS_ERR;
         }

      //check first line : START
      if( getLineTo( fileAccessPtr, MAX_STR_LEN, COLON,
                          dataBuffer, IGNORE_LEADING_WS ) != NO_ERR
          || compareString( dataBuffer, "Start Simulator Configuration File")
                                                          != STR_EQ)
         {
           //file is corrupt
             //close the file
           fclose ( fileAccessPtr );

     
            //return corrupt file data err code
           return CFG_CORRUPT_DESC_ERR;
         }

      //create a temp pointer for data config
      tempData = (ConfigDataType *) malloc( sizeof(ConfigDataType) );

      //begin loop to end og config data items
      while ( lineCounter < NUM_DATA_LINES )
      {
         //get the line reader and check for error
         if( getLineTo( fileAccessPtr, MAX_STR_LEN, COLON,
                          dataBuffer, IGNORE_LEADING_WS ) != NO_ERR)
         {
            //free the temp
            free(tempData);
            //close the file
            fclose( fileAccessPtr );

            //return error for file incomplete
            return INCOMPLETE_FILE_ERR;
         }

         //find the correct data line code
         dataLineCode = getDataLineCode( dataBuffer );

         // check to see if the data line code is valid
         // if so get the data value
         if( dataLineCode != CFG_CORRUPT_PROMPT_ERR )
         {
            switch (dataLineCode) 
            {
               //possiblity 1: version code is found => double
               case CFG_VERISON_CODE: 
                        fscanf( fileAccessPtr, "%lf", &doubleData);
                        break;

               //possiblity 2: metadata file => string
               case CFG_META_FILE_NAME_CODE: 
                        fscanf( fileAccessPtr, "%s", dataBuffer);
                        break;

               //possiblity 3: log file name => string
               case CFG_LOG_FILE_NAME_CODE: 
                        fscanf( fileAccessPtr, "%s", dataBuffer);
                        break;

               //possiblity 4: cpu schedule => string
               case CFG_CPU_SCHED_CODE: 
                        fscanf( fileAccessPtr, "%s", dataBuffer);
                        break;

               //possiblity 5: log to => string
               case CFG_LOG_TO_CODE: 
                        fscanf( fileAccessPtr, "%s", dataBuffer);
                        break;

               //possiblity 6: Memory available => long
               case CFG_MEM_AVAIL_CODE: 
                        fscanf( fileAccessPtr, "%li", &longData);
                        break;

               //possiblity 7: default, assume that its integer data
               default: 
                        fscanf( fileAccessPtr, "%d", &intData);
                        break;

            }//end of switch case statement

            if( valueInRange( dataLineCode,
                              intData,
                              doubleData,
                              longData,
                              dataBuffer ) == True )
            {
               switch (dataLineCode) 
               {
                  //possiblity 1: version code is found; assign then leave
                  case CFG_VERISON_CODE: 
                        tempData->version = doubleData;
                        break;

                  //possiblity 2: metadata file
                  case CFG_META_FILE_NAME_CODE: 
                        copyString(tempData->metaFilePath, dataBuffer);
                        break;

                  //possiblity 3: log file name
                  case CFG_LOG_FILE_NAME_CODE: 
                        copyString(tempData->logFilePath, dataBuffer);
                        break;

                  //possiblity 4: cpu schedule
                  case CFG_CPU_SCHED_CODE: 
                        tempData->CPUSchCode = getCpuSchedCode(dataBuffer);
                        break;

                  //possiblity 5: log to
                  case CFG_LOG_TO_CODE: 
                        tempData->logTo = getLogToCode(dataBuffer);
                        break;

                  //possiblity 6: quantum time
                  case CFG_Q_CYCLES_CODE: 
                        tempData->quantumTime = intData;
                        break;

                  //possiblity 7: memory available
                  case CFG_MEM_AVAIL_CODE: 
                        tempData->memoryAvailable = longData;
                        break;

                  //possiblity 8: IO cycles
                  case CFG_IO_CYCLES_CODE: 
                        tempData->IOCycTime = intData;
                        break;

                  //possiblity 9: processor cycles
                  case CFG_PROC_CYCLES_CODE: 
                        tempData->processCycTime = intData;
                        break;

                  

               }//end of switch case statement for data assignment

            }//end of if check for value in range
              else //assume data not in range
              {

                 //free the data
                 free(tempData);
                 //close the file
                 fclose(fileAccessPtr);

                 //return the "data out of range" code
                 return CFG_DATA_OUT_OF_RANGE_ERR;
              }


         }//end of if for checking if the data line exists
            else
            {
               //line does not exist or wasn't found
           
                 //free the data
                 free(tempData);
                 //close the file
                 fclose(fileAccessPtr);

                 //return the "corrupt promt" code
                 return CFG_CORRUPT_PROMPT_ERR;
            }

         //increment the line counter before the loop ends
         lineCounter++;

      }//end of master while loop

      //last check: test the final line for correctness : END
      if( getLineTo( fileAccessPtr, MAX_STR_LEN, PERIOD,
                          dataBuffer, IGNORE_LEADING_WS ) != NO_ERR
         || compareString( dataBuffer, "End Simulator Configuration File" )
                                                          != STR_EQ )
         {
           //file failed to have correct end line
           //free the data
           free(tempData);
             //close the file
           fclose ( fileAccessPtr );

            //return corrupt file data err code
           return CFG_CORRUPT_DESC_ERR;
         }

      //ensure that no data is missing/empty
          
          ConfigDataType *emptyData;
          emptyData = (ConfigDataType *) malloc( sizeof(ConfigDataType) );
      if( tempData->processCycTime == emptyData->processCycTime
          || tempData->IOCycTime == emptyData->IOCycTime
          || tempData->memoryAvailable == emptyData->memoryAvailable
          || tempData->quantumTime == emptyData->quantumTime
          || tempData->logTo == emptyData->logTo
          || tempData->CPUSchCode == emptyData->CPUSchCode
          || tempData->logFilePath == emptyData->logFilePath
          || tempData->metaFilePath == emptyData->metaFilePath
          || tempData->version == emptyData->version ) 
      {
         //for now I will treat missing things as corrupt
             //this is due to the fact that it can only happen
             //if a line of input in the file is duplicated
         return CFG_MISSING_DATA_ERR;
      }
      //done with the emptyData item so free it
      free(emptyData);


      //final data clean up:
        //assign temp data to config data pointer
        *configData = tempData;

        //close the file
        fclose( fileAccessPtr );

      //return the code for no errors
      return NO_ERR;
   }

//Clears out the data structures in their entirety
void deleteConfig(ConfigDataType **configDataPtr)
   {
     //check that the data is not null
     if( *configDataPtr != NULL )
     {
       //free the data structure
       free( *configDataPtr );
     }


      *configDataPtr = NULL;
   }

//takes in an error code and returns a string explaining the error
char* getErrorCodeMessage(int error)
   {
       //check the error code and give its corresponding message
       switch (error)
          {
             case NO_ERR: 
                  return "NO ERRORS : SUCCESS";
             case INCOMPLETE_FILE_ERR: 
                  return "INCOMPLETE FILE ERROR";
             case INPUT_BUFFER_OVERRUN_ERR: 
                  return "INPUT BUFFER OVERRUN ERROR";
             case CFG_FILE_ACCESS_ERR: 
                  return "CANNOT ACCESS FILE ERROR";
             case CFG_CORRUPT_DESC_ERR: 
                  return "CORRUPT FILE CONFIGURATION ERROR";
             case CFG_DATA_OUT_OF_RANGE_ERR: 
                  return "DATA OUT OF RANGE ERROR";
             case CFG_CORRUPT_PROMPT_ERR: 
                  return "CORRUPT CONFIG PROMPT ERROR";
             case CFG_MISSING_DATA_ERR: 
                  return "FILE IS MISSING KEY DATA";
          }
    return "UNKNOWN ERROR";

   }

//Displays the data in a given config data structure using printf
void printConfigData(ConfigDataType *configData)
   {
      //initializes a string for display of decoded data
      char displayString[ STD_STR_LEN ];

      //print out the data

      printf("Version/Phase            : %3.2f\n", configData->version );
      printf("Program File Name        : %s\n", configData->metaFilePath );
      //update display string with the string version of CPU code
      configCodeToString( configData->CPUSchCode, displayString );
      printf("CPU Schedule Selection   : %s\n", displayString );
      printf("Quantum Time             : %d\n", configData->quantumTime );
      printf("Memory Available         : %li\n", configData->memoryAvailable );
      printf("Process Cycle Rate       : %d\n", configData->processCycTime );
      printf("I/O Cycle Rate           : %d\n", configData->IOCycTime );
      //update display string with the string version of Log To code
      configCodeToString( configData->logTo, displayString );
      printf("Log To Selection         : %s\n", displayString );
      printf("Log File Name            : %s\n", configData->logFilePath );

      //no return or data, only prints
   }

//returns the matching code to the type of data the given string matches
int getDataLineCode(char *dataBuffer)
   { 
      //check if the data string is version
      if( compareString( dataBuffer, "Version/Phase" ) == STR_EQ ) 
      {
         return CFG_VERISON_CODE;
      }

      //check if the data string is the metadata file path
      if( compareString( dataBuffer, "File Path" ) == STR_EQ ) 
      {
         return CFG_META_FILE_NAME_CODE;
      }

      //check if the data string is CPU Scheduling Code
      if( compareString( dataBuffer, "CPU Scheduling Code" ) == STR_EQ ) 
      {
         return CFG_CPU_SCHED_CODE;
      }

      //check if the data string is quantum cycles
      if( compareString( dataBuffer, "Quantum Time (cycles)" ) == STR_EQ ) 
      {
         return CFG_Q_CYCLES_CODE;
      }

      //check if the data string is memory available, in KB
      if( compareString( dataBuffer, "Memory Available (KB)" ) == STR_EQ ) 
      {
         return CFG_MEM_AVAIL_CODE;
      }

      //check if the data string is proccessor cycles
      if( compareString( dataBuffer, "Processor Cycle Time (msec)" ) 
                                                             == STR_EQ ) 
      {
         return CFG_PROC_CYCLES_CODE;
      }

      //check if the data string is IO cycles
      if( compareString( dataBuffer, "I/O Cycle Time (msec)" ) == STR_EQ ) 
      {
         return CFG_IO_CYCLES_CODE;
      }

      //check if the data string is Log to
      if( compareString( dataBuffer, "Log To" ) == STR_EQ ) 
      {
         return CFG_LOG_TO_CODE;
      }

      //check if the data string is Log File Path
      if( compareString( dataBuffer, "Log File Path" ) == STR_EQ ) 
      {
         return CFG_LOG_FILE_NAME_CODE;
      }

      // at this point if no "if" was triggered then this is a bad line
      return CFG_CORRUPT_PROMPT_ERR;
   }

//Returns true if the value does not exceed its min or max values of the given data type
Boolean valueInRange(int lineCode,
                     int intVal,
                     double doubleVal,
                     long longVal,
                     char *stringVal)
   {


      //Data declarations for required mins and maxs
        //improves readablity and gets rid of "magic numbers"
      float MIN_VERSION = 0.0;
      float MAX_VERSION = 10.0;

      int MIN_Q_TIME = 0;
      int MAX_Q_TIME = 100;

      long MIN_MEM_AVB = 0;
      long MAX_MEM_AVB = 102400;

      int MIN_PROC_CYC_TIME = 0;
      int MAX_PROC_CYC_TIME = 1000;

      int MIN_IO_CYC_TIME = 0;
      int MAX_IO_CYC_TIME = 10000;

      //initailize variables
      Boolean result = True;

      //variables for a temp string
      char *tempStr;
      int strLen;

      //using line code idenity the prompt
      switch( lineCode )
      {
         //for version code
         case CFG_VERISON_CODE:
             if( doubleVal < MIN_VERSION
                 || doubleVal > MAX_VERSION)
             {
                //failed to have correct version
                result = False;
             }
             break;

         //for CPU schedule code
         case CFG_CPU_SCHED_CODE:

             //create temp lowercase string
             strLen = getStringLength( stringVal );
             tempStr = (char *) malloc( strLen+ 1);
             setStrToLowerCase( tempStr, stringVal );

             //failure to match any will result in a successful if
                   //which means it not a correct code
             if( compareString(tempStr, "none" ) != STR_EQ
                 && compareString(tempStr, "fcfs-n" ) != STR_EQ
                 && compareString(tempStr, "sjf-n" ) != STR_EQ
                 && compareString(tempStr, "srtf-p" ) != STR_EQ
                 && compareString(tempStr, "fcfs-p" ) != STR_EQ
                 && compareString(tempStr, "rr-p" ) != STR_EQ )
             {
                //failed to have correct CPU code
                result = False;
             }

             //free up our temp string
             free(tempStr);

             break;

         //for quantum cycles
         case CFG_Q_CYCLES_CODE:
             if( intVal < MIN_Q_TIME
                 || intVal > MAX_Q_TIME)
             {
                //failed to have correct quantum cycles value
                result = False;
             }
             break;

         //for memory availbe
         case CFG_MEM_AVAIL_CODE:
             if( longVal < MIN_MEM_AVB
                 || longVal > MAX_MEM_AVB)
             {
                //failed to have correct memory value
                result = False;
             }
             break;

         //for process cycles
         case CFG_PROC_CYCLES_CODE:
             if( intVal < MIN_PROC_CYC_TIME
                 || intVal > MAX_PROC_CYC_TIME)
             {
                //failed to have correct process cycles value
                result = False;
             }
             break;

         //for Log To code
         case CFG_LOG_TO_CODE:

             //create temp lowercase string
             strLen = getStringLength( stringVal );
             tempStr = (char *) malloc( strLen+ 1);
             setStrToLowerCase( tempStr, stringVal );

             //failure to match any will result in a successful if
                   //which means it not a correct code
             if( compareString(tempStr, "both" ) != STR_EQ
                 && compareString(tempStr, "monitor" ) != STR_EQ
                 && compareString(tempStr, "file" ) != STR_EQ )
             {
                //failed to have correct log to code
                result = False;
             }

             //free up our temp string
             free(tempStr);
             break;

         //for IO cycles
         case CFG_IO_CYCLES_CODE:
             if( intVal < MIN_IO_CYC_TIME
                 || intVal > MAX_IO_CYC_TIME)
             {
                //failed to have correct version
                result = False;
             }
             break;


      }//end of switch



      //return the result at the end
      return result;
   }

//failure defaults to FCFS-N
ConfigDataCodes getCpuSchedCode( char *codeStr)
   {

      //set up a temp string for safety and testing
      int strLen = getStringLength(codeStr);
      char *tempStr = (char *)malloc( strLen+1 );

      //set up default return
      int returnVal = CPU_CODE_FCFS_N;

      //set string to lower case for this test
      setStrToLowerCase( tempStr, codeStr );

      //check for SJF-N
      if( compareString( tempStr, "sjf-n" ) == STR_EQ )
      {
         returnVal = CPU_CODE_SJF_N;
      }

      //check for FCFS-P
      else if( compareString( tempStr, "fcfs-p" ) == STR_EQ )
      {
         returnVal = CPU_CODE_FCFS_P;
      }

      //check for SRTF-P
      else if( compareString( tempStr, "srtf-p" ) == STR_EQ )
      {
         returnVal = CPU_CODE_SRTF_P;
      }

      //check for RR-P
      else if( compareString( tempStr, "rr-p" ) == STR_EQ )
      {
         returnVal = CPU_CODE_RR_P;
      }

      //memory clean up
      free( tempStr );

      return returnVal;
   }

//given a code string of a log to option the function returns the appropraite assigned interger code
//failure defaults to MONITOR code
ConfigDataCodes getLogToCode( char *codeStr)
   {
     //initailize a temp string
     int strLen = getStringLength( codeStr );
     char *tempStr = (char *) malloc(strLen + 1);

     //set the default log to code : monitor
     int returnVal = LOG_TO_MONITOR_CODE;

     //create a lowercase copy of the string into temp
     setStrToLowerCase( tempStr, codeStr );

     //check for the code BOTH
     if( compareString( tempStr, "both" ) == STR_EQ )
     {
        returnVal = LOG_TO_BOTH_CODE;
     }

     //check for the code FILE
     if( compareString( tempStr, "file" ) == STR_EQ )
     {
        returnVal = LOG_TO_FILE_CODE;
     }

     //free our temp string
     free(tempStr);

     return returnVal;
   }

//Given a code and a string to be modified, the string will modified to contain the assigned matching string
void configCodeToString(int code, char *outStr)
   {
      //define a varialbe to be set to the string interpretation
      char *displayStrings;

       //check the code and set the display string
       switch (code)
          {
             case CPU_CODE_FCFS_N: 
                  displayStrings = "FCFS-N";
                  break;

             case CPU_CODE_SJF_N: 
                  displayStrings = "SJF-N";
                  break;

             case CPU_CODE_FCFS_P: 
                  displayStrings = "FCFS-P";
                  break;

             case CPU_CODE_RR_P: 
                  displayStrings = "RR-P";
                  break;

             case CPU_CODE_NONE: 
                  displayStrings = "FCFS-N"; //defaults to FCFS-N
                  break;

             case CPU_CODE_SRTF_P: 
                  displayStrings = "SRTF-P";
                  break;

             //start of Log To cases
             case LOG_TO_BOTH_CODE: 
                  displayStrings = "Both";
                  break;

             case LOG_TO_FILE_CODE: 
                  displayStrings = "File";
                  break;

             case LOG_TO_MONITOR_CODE: 
                  displayStrings = "Monitor";
                  break;
          }

      copyString( outStr, displayStrings );
   }




