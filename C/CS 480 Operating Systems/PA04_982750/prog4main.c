//headers
#include <stdio.h>
#include "CJConfigParser.h"
#include "MetaDataParser.h"
#include "CJSimulator.h"

/*
The folling main function must:

Take in a string from the cmd line which
represents the name of a config file to be processed

Output a display of both the saved config file contents
and the metadata op codes, all properly parsed and saved.

*/
////EXTRA CREDIT////
//Memory Management: Implemented
//Threads: Implemented
int main (int argc, char **argv)
   {
      int errorCode;
      ConfigDataType *configData;
      OpCodeType *metaData;
      char* processingResult;
      char* metaDataFileName;

      //verify correct cmd line arguments

      if(argc < 2)
      {
         printf("\nNo cmd line arguments given for file name\n");
         return 1;
      }


      printf("\n Reading File \'%s\' Now...\n", argv[1]);
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

      
      errorCode = processConfig(argv[1], &configData);

      processingResult = getErrorCodeMessage(errorCode);
      
      printf("\n Result: \'%s\' !\n", processingResult);
      printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");

      //Check the result state and exit main if not successful
      if( errorCode != NO_ERR )
         {
           //config file read failed give info then quit
           printf("Failure in Config Process: Closing Program\n\n");
           deleteConfig(&configData);
           return 2;
         }
		 
//////////////////////END OF CONFIG UPLOAD///////////////////////////

      //file reading was successful carry on to metadata

      metaDataFileName = configData->metaFilePath;
      errorCode = getOpCodes(metaDataFileName, &metaData);

      printf("\n Reading Metadata File \'%s\' Now...\n"
                                  , metaDataFileName);
      printf(">>----->>>>>>>----->>>>>>>---->>>>>>\n");
      //Display the results
      printf("\n Result of Meta Data Parsing: ");
      displayMetaDataError(errorCode);

      printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");


      //Check the result state and exit main if not successful
      if( errorCode != NO_ERR )
         {
           //config file read failed give info then quit
           printf("Failure in Metadata Process: Closing Program\n\n");

           //clear meta data
           metaData = clearMetaDataList(metaData);
           //clear config data
           deleteConfig(&configData);
           return 3;
         }


      printf("\n Now Running Simulation \n");
      printf("============================\n");
      
      runSimulation(&configData, &metaData);


      printf("\n End of Simulation \n");
      printf("============================\n");


      //END OF PROGRAM: SHUT DOWN
          //Shut Down Config
      deleteConfig(&configData);
          //Shut Down Meta Data
      metaData = clearMetaDataList(metaData);

      printf("\n");

      return 0;
   }



