#include "CJSimulator.h"

//TODO List
    //Add SJF-N capability

/* Function: runSimulation
   Input: Assumes it will recieve a valid configuration data and meta data
   Output: Stores updates into a structure
        Either prints these updats as their created and/or sotres them in a log at the end
   Notes:
*/
void runSimulation(ConfigDataType **configData, OpCodeType **metaData)
{
    //copy the data to ensure nothing gets written by accident
    ConfigDataType *configDataCopy = *configData;
    OpCodeType *metaDataCopy = *metaData;
    
    
    //create our temporary structure for holding updates
    char *tempLog = (char *)malloc(MAX_STR_LEN + 1);
    
    //create header
    char header[] = "Simluator Log File \n============================\n";
          
          
    //add header
    concatenateString( tempLog, header );
    
    //Create our timer
    char * CPUtime;
    CPUtime = (char *)malloc( MAX_STR_LEN + 1 );
    accessTimer(ZERO_TIMER, CPUtime);
    
    //Start the system and update
    updateTSim("OS: System Start", CPUtime, configDataCopy, &tempLog, False);
    updateTSim("OS: Create Process Control Blocks", CPUtime, configDataCopy, &tempLog, True);
    
    //create a linked list header to start the linked list
    struct ProcessHead *pcbList;
    pcbList = (struct ProcessHead *)malloc( sizeof(ProcessHead) );
    struct ProcessList **pcbListNextOpen = &pcbList->processStart;
    
    //create a counter to assign processes with an ID number
    int availableID = 0;
    
    //create a lock to ensure mutual exclusion for later thread use
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);
    
    //create process control blocks and add them to the linked list
    while(metaDataCopy->next != NULL)
    {
        if(metaDataCopy->opLtr == 'A' && compareString(metaDataCopy->opName , "start") == True)
        {
            //initialize a new PCB
            struct ProcessControlBlock *processBk;
            processBk = (struct ProcessControlBlock *) malloc( sizeof(struct ProcessControlBlock) );
            
            //add in the information for the PCB
            
            //give the PCB an id number
            processBk->id = availableID;
            //Set the availble ID value to the next value
            availableID++;
            
            //add config data
            processBk->pConfigData = configDataCopy;
            
            //add the metadata
            processBk->pMetaData = metaDataCopy;

            //Set current op to start of metadata in PCB
            processBk->currentOp = processBk->pMetaData;
            

            //Create and add in a interger list to represent memory for the PCB
            int * NewPCBMemory;
            NewPCBMemory = (int *)malloc(sizeof(int[configDataCopy->memoryAvailable]));
     
            
            //initialize all of the memory to 0 which means unallocated
            int i;
            for(i = 0; i < configDataCopy->memoryAvailable; i++)
            {
 
                NewPCBMemory[i] = 0;
            }
            
                //add the memory to the PCB
            processBk->PCBMemory = NewPCBMemory;

        
            //add in the lock for use in mutual exclusion
            processBk->lock = &lock;
        
            //set the state to NEW
            processBk->processState = "NEW";
            
            //add in tempLogs pointer
            processBk->tempLogPointer = &tempLog;
            
            //add in CPUTimes pointer
            processBk->CPUTimePointer = &CPUtime;
            
            //Calulate the total time of the PCB runtime
            processBk->totalTime = getTotalOpTime(processBk->pMetaData,
                                                    processBk->pConfigData);
            
            
            //create the next list node to be placed
            struct ProcessList *pcbListNext;
            pcbListNext = (struct ProcessList *)malloc( sizeof(ProcessList) );
            //add the pcb into the linked list node
            pcbListNext->processBlock = processBk;
            
            //set the next holder to NULL
            pcbListNext->next = NULL;

            *pcbListNextOpen = pcbListNext;
            
            //set the next open pointer to the next list memory address
            pcbListNextOpen = &pcbListNext->next;
            
        }
        
        metaDataCopy = metaDataCopy->next;
    }

    
    //update the system of the new process blocks
    updateTSim("OS: All Processes Initialized and set in NEW state", CPUtime, configDataCopy, &tempLog, True);
    
    //Send a copy of our list of PCBs to the ready queue (for memory managment reasons)
    struct ProcessHead *pcbListCopy;
    pcbListCopy = (struct ProcessHead *)malloc( sizeof(ProcessHead) );
    *pcbListCopy = *pcbList;
    
    sendToReady(pcbListCopy);
   
    //Check if the config is set to log the data
    if(configDataCopy->logTo == LOG_TO_BOTH_CODE
       || configDataCopy->logTo == LOG_TO_FILE_CODE)
       {
           writeToLog(configDataCopy, tempLog);
       }
       
    //free up our data
      //free our time string
    free(CPUtime);
    
    //loop through our linked list and free everything
    struct ProcessList *tempPcbListNode;
    
    while(pcbList->processStart != NULL)
    {
        //grab linked list node
        tempPcbListNode = pcbList->processStart;
        
        //set next node
        pcbList->processStart = pcbList->processStart->next;
        
        //free the memory in the grabbed pcb
        free(tempPcbListNode->processBlock->PCBMemory);
        tempPcbListNode->processBlock->PCBMemory = NULL;
        
        //free the held process block
        free(tempPcbListNode->processBlock);
        tempPcbListNode->processBlock = NULL;

        //free our linked list node
        free(tempPcbListNode);
        tempPcbListNode = NULL;
    }
    
    //free the head pointer
    free(pcbList);
    pcbList = NULL;
    
    //free our copy
    free(pcbListCopy);
    pcbListCopy = NULL;
    
    //Free our temporary log
    free(tempLog);
    tempLog = NULL;
    
}

//Updates the systems temporary log and if system is set to print will print
    //if the time given has not been initialized it is set to 0
void updateTSim(char *printStr, char *timeStr, ConfigDataType *configData, char **tempLog, Boolean updateT)
{

    int logCode = configData->logTo;
    
    //check to see if the timer needs to be started
    if( updateT == True )
    {
        //The timer is already started so update the time
        accessTimer(LAP_TIMER, timeStr);
    }

    
    char tempTime[getStringLength(timeStr)];
    copyString(tempTime, timeStr);
    
    char tempStr[getStringLength(tempTime)];
    copyString(tempStr, tempTime);
    
    //create the temporary string that will be added to the temp log
   // concatenateString(tempStr, tempTime);
    concatenateString(tempStr, ", ");
    concatenateString(tempStr, printStr);
    concatenateString(tempStr, "\n");
    
    //add string to the temp log
    concatenateString(*tempLog, tempStr);
    
    
    //check to see if the simulator is supposed to print
    if(logCode == LOG_TO_BOTH_CODE
       || logCode == LOG_TO_MONITOR_CODE)
    {
        //print out the temporary string
        printf(tempStr);
    }

}

//given configuration data and a string creates a log text file
void writeToLog(ConfigDataType *configData, char tempLog[])
{
    
    FILE *logFile;
    char *WRITE_ONLY = "w";
    char *logPath = configData->logFilePath;
    char codeStr[STD_STR_LEN];
           
    //open file
    logFile = fopen(logPath, WRITE_ONLY);
    
    //copy over some desired config data to our file
    
    fprintf(logFile,"===========================================\n");
    fprintf(logFile,"Config Data\n\n");

    fprintf(logFile, "File Name                       : %s\n", configData->metaFilePath);
    
    //Turn the cpu code into a human readble string
    configCodeToString(configData->CPUSchCode, codeStr);
    fprintf(logFile, "CPU Scheduling                  : %s\n",codeStr);
    
    fprintf(logFile, "Quantum Cycles                  : %d\n", configData->quantumTime);
    
    fprintf(logFile, "Memory Available [KB]           : %li\n", configData->memoryAvailable);
    
    fprintf(logFile, "Processor Cycle Rate [ms/cycle] : %d\n", configData->processCycTime);
    
    fprintf(logFile, "I/O Cycle Rate [ms/cycle]       : %d\n\n", configData->IOCycTime);
    
    fprintf(logFile,"===========================================\n");
           
    //copy the given simulator log data to file
    fprintf(logFile, tempLog);
           
    //close file
    fclose(logFile);
}

//Changes the state of all PCBs in a list to "READY"
void sendToReady( ProcessHead *linkedPCBList )
{
    //Loop through the given list and change all states to ready
    ProcessList *likedListIndex = linkedPCBList->processStart;
    
    while(likedListIndex != NULL)
    {
        likedListIndex->processBlock->processState = "READY";
        likedListIndex = likedListIndex->next;
    }
    
    //Grab the first pcb block from the list to use its data to:
    //    print and determine the CPU scheduling to run
    ProcessList *pcbList = linkedPCBList->processStart;
    ProcessControlBlock *pcbBlockStart = pcbList->processBlock;
    //Grab the CPU time string
    char *pcbCPUTimer = *pcbBlockStart->CPUTimePointer;
    
    updateTSim("OS: All Processes now set in READY state",
                pcbCPUTimer,
                pcbBlockStart->pConfigData,
                pcbBlockStart->tempLogPointer,
                True);
    
    //Defaults to handling FCFS-N
    switch (pcbBlockStart->pConfigData->CPUSchCode)
    {
        case CPU_CODE_FCFS_N:
        handleFCFSNready( linkedPCBList );
        break;
        
        //Case for SJF-N
            //Bubbles sorts the pcb list then sends the list to handleFCFSNready
            //This is because once sorted for shortest job first they aren't different
        case CPU_CODE_SJF_N:
        shortestFirstSort(linkedPCBList);
        handleFCFSNready( linkedPCBList );
        break;
        
        //Case foe the handling of any P based scheduling
        case CPU_CODE_FCFS_P:
        case CPU_CODE_RR_P:
        case CPU_CODE_SRTF_P:
        handlePQueue( linkedPCBList );
        break;
        
        default:
        handleFCFSNready( linkedPCBList );
        break;
    }

}

//Handles the "READY" state for FCFS-N specifically
void handleFCFSNready( ProcessHead *linkedPCBList )
{
    //Grab the first pcb block from the list
    ProcessList *pcbList = linkedPCBList->processStart;
    ProcessControlBlock *pcbBlockStart = pcbList->processBlock;


    //prep a string to hold the total time string message
    char printStr[MAX_STR_LEN];
    snprintf(printStr, MAX_STR_LEN, "OS: Process %d selected with %d ms time left", 
                                                    pcbBlockStart->id,
                                                    pcbBlockStart->totalTime);
    
    //give an update of the process selected's total time
    updateTSim(printStr,
                *linkedPCBList->processStart->processBlock->CPUTimePointer,
                linkedPCBList->processStart->processBlock->pConfigData,
                linkedPCBList->processStart->processBlock->tempLogPointer,
                True);
     
    //Create a new thread and send the process to our run function
    //Create and join the thread
    pthread_t process_block_thread;

    pthread_create(&process_block_thread,
                    NULL,
                    handleNPrun,
                    (void *) linkedPCBList->processStart->processBlock);
                    
    pthread_join(process_block_thread, NULL);
    
    //If the list is not at the end then recursivly call ready to start at the next PCB
    if(linkedPCBList->processStart->next)
    {

        //set the list to start at the next PCB
        linkedPCBList->processStart = linkedPCBList->processStart->next;

        //recursivly call ready to continue going
        handleFCFSNready( linkedPCBList );
    }
}

void *handleNPrun( void *pcb )
{
    ProcessControlBlock *processBk = (ProcessControlBlock *) pcb;
    //set our process to running
    processBk->processState = "RUNNING";
    
    
    //prep a string to hold the total time string message
    char printStr[MAX_STR_LEN];
    snprintf(printStr, MAX_STR_LEN, "OS: Process %d Initialized in %s state\n", 
                                                    processBk->id,
                                                    processBk->processState);
    
    //update the system that the process was set to RUNNING
    updateTSim(printStr,
                *processBk->CPUTimePointer,
                processBk->pConfigData,
                processBk->tempLogPointer,
                True);
    
    
    //Run the process using the FCFS-N method

    while(processBk->currentOp != NULL 
          && compareString(processBk->processState, "RUNNING") == True)
    {                  
        if(processBk->currentOp->opLtr == 'A' 
          && compareString(processBk->currentOp->opName , "end") == True)
          {
              break;
          }
        
        //If the process is supposed to be running then make and join a new process
        if(compareString(processBk->processState, "RUNNING") == True)
        {
            //Create and join a new thread using the pcb
            pthread_t process_thread;

            pthread_create(&process_thread,
                            NULL,
                            runProcess,
                            (void *) processBk);
                    
            pthread_join(process_thread, NULL);
        
            //go to next operation
            processBk->currentOp = processBk->currentOp->next;
        }
    }
    
    
    //No longer running so...
    
    //if it was a natural exit then set the pcb to exit
    if(processBk->currentOp != NULL)
    {
        processBk->processState = "EXIT";
    }
    
    //Create string for output
    char exitPrintStr[MAX_STR_LEN];
    snprintf(exitPrintStr, MAX_STR_LEN, "OS: Process %d ended and set in %s state\n", 
                                                    processBk->id,
                                                    processBk->processState);
    //update that the PCB stopped running
    updateTSim(exitPrintStr,
                *processBk->CPUTimePointer,
                processBk->pConfigData,
                processBk->tempLogPointer,
                True);
    
    //end the thread function with an exit
    pthread_exit(NULL);
}


//Takes in a pcb block and runs the opertaion for its cycles decrementing the value each cycle
    //Currently only does N based scheduling right now
void *runProcess(void *pcbBlock)
{
    
    //devoid our data so that it can be used
    struct ProcessControlBlock *pcbOperation = (struct ProcessControlBlock *) pcbBlock;
    
    //Grab the follwing from the given pcb structure to run the current process
    //int processID, int timeRun, OpCodeType *opCode, ConfigDataType *configData, char **tempLog, char *CPUtime
    int processID = pcbOperation->id;

    int timeRun = pcbOperation->currentOp->opValue;
    
    OpCodeType *opCode = pcbOperation->currentOp;
    
    ConfigDataType *configData = pcbOperation->pConfigData;
    
    char **tempLog = pcbOperation->tempLogPointer;
    
    char *CPUtime = *pcbOperation->CPUTimePointer;
    
    
    
    //Begin critical section with a lock
    pthread_mutex_lock(pcbOperation->lock);
    /////////////////////////////////////////////////////////////
    
    //if given a non running function -> exit
    if(timeRun == 0)
    {
        //Early exit -> unlock the lock
        pthread_mutex_unlock(pcbOperation->lock);
        //EXIT
        pthread_exit(NULL);
    }
    
    
    
    //prep a string to state the ID, process, and information of running
    char printStrStart[MAX_STR_LEN];
    char printStrEnd[MAX_STR_LEN];
    
    
    //Identify the type of operation so that it can be handled accordingly
    char *operationType;
    
    char operationChar = opCode->opLtr;

    int allocate = 0;
    int offset = 0;
    
    //Parse Memory Values
    //get last 4 digits
    allocate = timeRun % 10000;
            
    //get first 5 or so digits
    offset = timeRun;
            
    while(offset >= 10000)
    {
        offset = offset/10000;
    }
    
    //Set the operation type and print out string based on type
    switch (operationChar)
    {
        case 'P':
        operationType = "operation";
        
        //Form print string from data
        snprintf(printStrStart, MAX_STR_LEN, "Process: %d, %s %s start\n", processID, opCode->opName, operationType);
        break;
        
        case 'I':
        operationType = "input";
        
        //Form print string from data
        snprintf(printStrStart, MAX_STR_LEN, "Process: %d, %s %s start\n", processID, opCode->opName, operationType);
        break;
        
        case 'O':
        operationType = "output";
        
        //Form print string from data
        snprintf(printStrStart, MAX_STR_LEN, "Process: %d, %s %s start\n", processID, opCode->opName, operationType);
        break;
        
        
        case 'M':
        if( compareString(opCode->opName, "allocate") == True )
        {
            
            operationType = "memory allocate";
            
            //Form print string from data
            snprintf(printStrStart, MAX_STR_LEN, "Process: %d, %s %s %d/%d start\n", processID,
                                                                            opCode->opName,
                                                                            operationType,
                                                                            offset,
                                                                            allocate);
        }
        else
        {
            operationType = "memory access";
            
            //Form print string from data
            snprintf(printStrStart, MAX_STR_LEN, "Process: %d, %s %s %d/%d start\n", processID,
                                                                            opCode->opName,
                                                                            operationType,
                                                                            offset,
                                                                            allocate);
        }
        break;

    }
    
    
    //start process
    updateTSim(printStrStart, CPUtime, configData, tempLog, True);
    
   
    if( compareString(operationType, "memory allocate") == True )
    {
        //Try to perform memory operation
        if(allocateMemory(pcbOperation, allocate, offset) == False)
        {
            //update end of process
            snprintf(printStrEnd, MAX_STR_LEN, "Process: %d, %s %s SEGMENTATION FAULT\n",
                                                   processID,
                                                   opCode->opName,
                                                   operationType);
                                                   
            updateTSim(printStrEnd, CPUtime, configData, tempLog, True);
            
            //Set PCB to exit
            pcbOperation->processState = "EXIT";
    
            //Early Exit -> End critical and unlock
            pthread_mutex_unlock(pcbOperation->lock);
    
            //end the thread
            pthread_exit(NULL); //Exit and segfault complete
        }
        
    }
    else
    {
        if( compareString(operationType, "memory access") == True )
        {
            //Try to perform memory operation
            if(accessMemory(pcbOperation, allocate, offset) == False)
            {
                //update end of process
                snprintf(printStrEnd, MAX_STR_LEN, "Process: %d, %s %s SEGMENTATION FAULT\n",
                                                   processID,
                                                   opCode->opName,
                                                   operationType);
                                                   
                updateTSim(printStrEnd, CPUtime, configData, tempLog, True);
    
                //Set PCB to exit
                pcbOperation->processState = "EXIT";
    
                //Early Exit -> End critical and unlock
                pthread_mutex_unlock(pcbOperation->lock);
    
    
                //end the thread
                pthread_exit(NULL); //Exit and segfault complete
            }

        }
        //Not a memory operation so we just spin through the cycles
        else
        {
            
            if(( configData->CPUSchCode == CPU_CODE_FCFS_P
                      || configData->CPUSchCode == CPU_CODE_RR_P
                      || configData->CPUSchCode == CPU_CODE_SRTF_P )
                      && !( pcbOperation->currentOp->opLtr == 'I' 
                           || pcbOperation->currentOp->opLtr == 'O' )
                )
            {                
                int cyclesRun = 0;
                while(cyclesRun <= configData->quantumTime)
                {
                    //For safty we break out early if the opvalue is at 0
                    if(pcbOperation->currentOp->opValue == 0)
                    {
                        break;
                    }
                    
                    //Get the time to run one cycle of the operation
                    int timeToRunEntireCycle = getOpTime(pcbOperation->currentOp, pcbOperation->pConfigData);
                     
                    int timeToRunSingleCycle = timeToRunEntireCycle/pcbOperation->currentOp->opValue;
                
                    //Update the operations value
                    pcbOperation->currentOp->opValue = pcbOperation->currentOp->opValue - 1;
                    timeRun = pcbOperation->currentOp->opValue;
                
                    //Run a single cycle of the operation
                    runTimer(timeToRunSingleCycle);
                    
                    if( compareString(((struct ProcessControlBlock *) pcbBlock)->processState,
                                        "RUNNING")
                                        == False)
                    {
                        //Report that there was an Interrupt
                        char *interruptUpdateStr = "";
                        snprintf(interruptUpdateStr, MAX_STR_LEN, "OS: Process %d was Interrupted", pcbOperation->id);
                        updateTSim(interruptUpdateStr, CPUtime, configData, tempLog, True);
                        break;
                    }
                    
                    //Update the number of cycles run so far
                    cyclesRun++;
                }
            }
            else
            {
                //Special handling for I/O operations
                if( ( configData->CPUSchCode == CPU_CODE_FCFS_P
                      || configData->CPUSchCode == CPU_CODE_RR_P
                      || configData->CPUSchCode == CPU_CODE_SRTF_P )
                      && ( pcbOperation->currentOp->opLtr == 'I' 
                           || pcbOperation->currentOp->opLtr == 'O' ) )
                {
                    //I/O opertaions must get sent to BLOCKED
                    char processUpdateStr[MAX_STR_LEN + 1];
                    snprintf(processUpdateStr, MAX_STR_LEN, "OS: Process %d set to BLOCKED", pcbOperation->id);
                    
                    updateTSim(processUpdateStr, CPUtime, configData, tempLog, True);
                    
                    pcbOperation->processState = "BLOCKED";
                }
                
                
                while(timeRun > 0)
                {
                    //Get the time to run one cycle of the operation
                    int timeToRunEntireCycle = getOpTime(pcbOperation->currentOp, pcbOperation->pConfigData);
                
                    int timeToRunSingleCycle = timeToRunEntireCycle/pcbOperation->currentOp->opValue;
                
                    //Update the operations value
                    pcbOperation->currentOp->opValue = pcbOperation->currentOp->opValue - 1;
                    timeRun = pcbOperation->currentOp->opValue;
                
                    //Run a single cycle of the operation
                    runTimer(timeToRunSingleCycle);
                }
            }
        }
    }
    
    
    //update end of process
    //If the process ended due to quantum time out report that instead
    if(pcbOperation->currentOp->opValue > 0)
    {
       //Process ran out of quantum time
       pcbOperation->processState = "INTERRUPTED";
       snprintf(printStrEnd, MAX_STR_LEN, "Process: %d, %s %s quantum timeout\n",
            processID, opCode->opName, operationType);
    }
    else
    {
        //Process is complete
        snprintf(printStrEnd, MAX_STR_LEN, "Process: %d, %s %s end\n", processID, opCode->opName, operationType);
    }

    //Update the total time of the PCB runtime
    pcbOperation->totalTime = getTotalOpTime(pcbOperation->pMetaData,
                                          pcbOperation->pConfigData);

    updateTSim(printStrEnd, CPUtime, configData, tempLog, True);
    
    //If the process is completly done then tell it to EXIT
    if(pcbOperation->totalTime == 0)
        {
            pcbOperation->processState = "EXIT";
        }
    
    ////////////////////////////////////////////////////////////////////////
    //End of critical section -> unlock the lock
    pthread_mutex_unlock(pcbOperation->lock);
    
    
    //end the thread
    pthread_exit(NULL);
}

//Part 4: P based process scheduling
/* Function: handlePQueue
   Output: N/A
   
   Details: This function handles the P based scheduling for PCBs which
            utilizes interrupts, the blocked state, and the waiting state
            The function handles these various states as well as
            maintains that proper queue order of the PCBs based on
            the scheduling.
*/
void handlePQueue( ProcessHead *linkedPCBList )
{
    
    //Do nothing if the list is empty
        //This also ends the recursion
    if( linkedPCBList == NULL 
        || linkedPCBList->processStart == NULL)
    {
        return;
    }
    
    /*To handle the queue a switch case is used
      to handle the queue based on the list which
      will be sorted to match the necessary scheduling
      so that the PCB that should be handled next
      will be in the front, skipping over any blocked PCBs
    */
    
    //Create a pointer to keep teack of the PCB the function is looking at
    struct ProcessList * nextPCB = linkedPCBList->processStart;
    
    
        //Case: Blocked
         /*PCBs that are blocked should be
           skipped over as they can run in the background
         */
    while( compareString(nextPCB->processBlock->processState, "BLOCKED") == True )
    {
        nextPCB = nextPCB->next;
    }

        //Case: Exit
         /*If a PCB is found in the exit state it
           should be removed form the list then that new list
           should get passsed back into the queue
         */
    if( compareString(nextPCB->processBlock->processState, "EXIT") == True )
    {
        linkedPCBList->processStart = linkedPCBList->processStart->next;
        
        //Update the system that a pcb finished
        //Create string for output
        char exitPrintStr[MAX_STR_LEN];
        snprintf(exitPrintStr, MAX_STR_LEN, "OS: Process %d ended and set in %s state\n", 
                                                    nextPCB->processBlock->id,
                                                    nextPCB->processBlock->processState);
        //update that the PCB stopped running
        updateTSim(exitPrintStr,
                *nextPCB->processBlock->CPUTimePointer,
                nextPCB->processBlock->pConfigData,
                nextPCB->processBlock->tempLogPointer,
                True);
        
        //Check to see if the list is empty and if so exit
        if(linkedPCBList != NULL
           && linkedPCBList->processStart != NULL)
           {
               handlePQueue( linkedPCBList );
           }
           else
           {
               return;
           }
        
    }
        
        //Case: Waiting/Interrupted
         /*If a PCB is found to be waiting or 
           interrupted then
           the system needs to set it to ready
           then send it to the end of the queue
           and then sort the queue if its SJF or FCFS
         */
    if( compareString(nextPCB->processBlock->processState, "WAITING" )   == True
        || compareString(nextPCB->processBlock->processState, "INTERRUPTED" )   == True )
    {
        
        nextPCB->processBlock->processState = "READY";
        //TODO make helper function for sending a PCB to the back of the list for RR
        switch(nextPCB->processBlock->pConfigData->CPUSchCode)
        {
            case CPU_CODE_FCFS_P:
            IDSort(linkedPCBList);
            break;
            
            case CPU_CODE_RR_P:
            sendPCBNodeToBack(linkedPCBList, nextPCB);
            break;
            
            case CPU_CODE_SRTF_P:
            shortestFirstSort(linkedPCBList);
            break;
            
            default:
            IDSort(linkedPCBList);
            break;
        }
        
        handlePQueue( linkedPCBList );
    }
        
        //Case: Running
         /*If there is a PCB that is running then
           the queue should just go back to checking for Interrupts
         */
    if( compareString(nextPCB->processBlock->processState, "RUNNING") == True )
    {
        //Hold up the queue until PCB is finshed or interrupted
        while(compareString(nextPCB->processBlock->processState, "RUNNING") == True)
        {
            ;//Do Nothing Until Not In Run State
        }
        
        handlePQueue( linkedPCBList );
    }
         
        //Case: Ready
         /*If the next PCB that needs to be handled
           is in the ready state then it should be set
           to Running and be run by the system
         */
    if( compareString(nextPCB->processBlock->processState, "READY") == True )
    {
        
        //Set the PCB to RUNNING
        nextPCB->processBlock->processState = "RUNNING";
        //Run the PCB
        
        //Create and join a new thread using the pcb
        pthread_t process_thread;

        pthread_create(&process_thread,
                            NULL,
                            handleP_PCB,
                            (void *) nextPCB->processBlock);
                    
        pthread_join(process_thread, NULL);
        
        handlePQueue( linkedPCBList );
    }
         
        //Case: Default
         /*At this point the system does not find
           a PCB to run or that is already running
           thus it needs to report that it idles
         */
    
    //Update system that idling is occurring
    updateTSim("OS: System/CPU Idle",
                *linkedPCBList->processStart->processBlock->CPUTimePointer,
                linkedPCBList->processStart->processBlock->pConfigData,
                linkedPCBList->processStart->processBlock->tempLogPointer,
                True);
    
    
    //Run the function again as it will stop when the list is empty
    handlePQueue( linkedPCBList );
}

/* Function: sendPCBNodeToBack
   Output: N/A
   
   Details: This function takes in a list and a node.
            If found in the list the node is removed
            then the node is added to the end of the list
*/
void sendPCBNodeToBack(ProcessHead *linkedPCBList, ProcessList *pcbNode)
{
    //Find the PCB node at the end of the list
    ProcessList *lastPCB = linkedPCBList->processStart;
    while(lastPCB->next != NULL)
    {
        lastPCB = lastPCB->next;
    }
    
    //Then attempt to find the pcbNode given in the list and remove it
    ProcessList *index = linkedPCBList->processStart;
    while(index->next != NULL)
    {
        if(index->next == pcbNode)
        {
            index->next = pcbNode->next;
        }
    }
    
    //Then add the pcbNode to the end of the list
    lastPCB->next = pcbNode;
}

/* Function: handleP_PCB
   Output: N/A
   
   Details: This function handles the operations in a
            selected PCB to get them to run and accounts
            for the differences in the types of operations
*/
void *handleP_PCB( void * pcb )
{
    /* Theses are the main cases to handle
        Finished Process: If an operation has 0 cycles
                        then ignore it and move on
    
        I/O: PCB should get set to Blocked and then told to 
             go execute just the I/O. Once it finishes should
             set itself to Waiting
             
        Non-I/O: The PCB should be already set to running and
                 only when the PCB is set to something other
                 than running do we stop
    */
    
    ProcessControlBlock *processBk = (ProcessControlBlock *) pcb;
    
    //For safety if the processBk is null we exit this function
    if(processBk == NULL || processBk->currentOp == NULL )
    {
        processBk->processState = "EXIT";
        //end the thread of the PCB executing
        pthread_exit(NULL);
    }
    
    //If the current op code doesnt have any time then skip it
    if(processBk->currentOp->opValue == 0)
    {
        //set our process to ready
        processBk->processState = "READY";
        processBk->currentOp = processBk->currentOp->next;
        
        //end the thread of the PCB executing
        pthread_exit(NULL);
    }
    
    //set our process to running
    processBk->processState = "RUNNING";
    
    
    //prep a string to hold the total time string message
    char printStr[MAX_STR_LEN];
    snprintf(printStr, MAX_STR_LEN, "OS: Process %d Initialized in %s state\n", 
                                                    processBk->id,
                                                    processBk->processState);
    
    //update the system that the process was set to RUNNING
    updateTSim(printStr,
                *processBk->CPUTimePointer,
                processBk->pConfigData,
                processBk->tempLogPointer,
                True);
    
    
    //If the process is supposed to be running then make and join a new process
    if(compareString(processBk->processState, "RUNNING") == True)
    {
        //Create and join a new thread using the pcb
        pthread_t process_thread;

        pthread_create(&process_thread,
                            NULL,
                            runProcess,
                            (void *) processBk);
                    
        pthread_join(process_thread, NULL);
    }

    
    
    //No longer running so...
    //Update the totoal time
    processBk->totalTime = getTotalOpTime(processBk->pMetaData, processBk->pConfigData);
    
    //if the op finishes change the current op
    if(processBk->currentOp->opValue == 0)
    {
        processBk->currentOp = processBk->currentOp->next;
    }
    
    //If there is no more total time then exit
    if(processBk->totalTime == 0)
    {
        processBk->processState = "EXIT";
    }
    else
    {
        //otherwise set our process to ready
        processBk->processState = "READY";
    }
    
    //end the thread of the PCB executing
    pthread_exit(NULL);
}

//Memory Allocation and Access functions //////////////////////////////////
/* Function: accessMemory
   Output: Boolean, True if sucessful
   
   Details: The memory is a array of integers that are either 0 or 1
			0 means that that memory bit is NOT allocated
			1 means that the memory is allocated and thus can be accessed
	Always returns false if the memory to access from the offset is greater than
	the maximum memory size
*/
Boolean accessMemory(ProcessControlBlock *pcbBlk, int allocateMem, int offsetMem)
{
    //Start by checking offset + allocation to see if it suceeds the max memory
    if(offsetMem + allocateMem > pcbBlk->pConfigData->memoryAvailable)
    {
        //If the memory requested is out of bounds return failure/false
        return False;
    }
	
    //See if the memory can be accessed and if not fail -> return false
    int memIndex;
    for(memIndex = offsetMem; memIndex <= allocateMem + offsetMem; memIndex++)
    {
        if(pcbBlk->PCBMemory[memIndex] == 0)
        {
            return False;
        }
    }
	
	
    return True; //No errors or bad memory return suceeds/true
}

/* Function: allocateMemory
   Output: Boolean, True if sucessful
   Assigns: The function alters the memory in the PCB to be set to 1
			aka assigned if it is sucessful ad can allocate the memory
		-cannot allocate if any part of the memory requested is already allocated
		
   Details: The memory is a array of integers that are either 0 or 1
			0 means that that memory bit is NOT allocated
			1 means that the memory is allocated and thus can be accessed
	Always returns false if the memory to allocate from the offset is greater than
	the maximum memory size
*/
Boolean allocateMemory(ProcessControlBlock *pcbBlk, int allocateMem, int offsetMem)
{
    //Start by checking offset + allocation to see if it suceeds the max memory
    if(offsetMem + allocateMem > pcbBlk->pConfigData->memoryAvailable)
    {
        //If the memory requested is out of bounds return failure/false
        return False;
    }
	
    //Check to see if any data in the area is already allocated if so 
    //-> return false
    //counter memIndex for loops through memory
    int memIndex;
    
    for(memIndex = offsetMem; memIndex <= allocateMem + offsetMem; memIndex++)
    {
        //If the memory is already allocated return false
        if(pcbBlk->PCBMemory[memIndex] == 1)
        {
            return False;
        }
    }
	
    //If theres not any memory already allocated then we can allocate then
    //return true to signal that allocate suceeds
    for(memIndex = offsetMem; memIndex <= allocateMem + offsetMem; memIndex++)
    {
        pcbBlk->PCBMemory[memIndex] = 1;
    }
    
	
    return True; //No errors or bad memory return suceeds/true
}



//Gives an integer that is the sum of the operation times of a given metadata list
//  This is done by recursivly calling down the metadata list structure
int getTotalOpTime(OpCodeType *metaData, ConfigDataType *configData)
{
    //Make sure metadata is valid
    if(metaData != NULL)
    {
        //if we are at the process end stop
        if(metaData->opLtr == 'A' && compareString(metaData->opName, "end") == True)
        {
            return 0;
        }
        //otherwise get the current value and recusivly continue to next meta data
        int value;
        value = getOpTime(metaData, configData);
        
        return (value + getTotalOpTime(metaData->next, configData));
    }
    
    //we are at the end of the metadata
    return 0;
}

//Gives an integer that is the millisecond time that the first metadata op code must run for
int getOpTime(OpCodeType *metaData, ConfigDataType *configData)
{
    int value = 0;

    switch (metaData->opLtr)
    {
        case 'I': case 'O':
        value = metaData->opValue * configData->IOCycTime;
        break;
        
        case 'P':
        value = metaData->opValue * configData->processCycTime;
        break;
    }

    return value;
}

//PCB Sorting function for sorting into shortest job first 
void shortestFirstSort(ProcessHead *linkedPCBList)
{
    ProcessList *indexNodeLeft;
    ProcessList *indexNodeRight = NULL;
    
    
    //Begin Bubble Sort
    Boolean swapped = False;
    do
    {
        swapped = False;
        indexNodeLeft  = linkedPCBList->processStart;
        
        //Sort via swapping the pcb data
        while(indexNodeLeft->next != indexNodeRight)
        {
            int jobValueNext = indexNodeLeft->next->processBlock->totalTime;
            
            int jobValueIndex = indexNodeLeft->processBlock->totalTime;
            
            if( jobValueIndex > jobValueNext)
            {
                swapPCB(indexNodeLeft, indexNodeLeft->next);
                swapped = True;
            }

            indexNodeLeft = indexNodeLeft->next;
        }
        
        indexNodeRight = indexNodeLeft;
        
    }while(swapped == True);

}


//PCB Sorting function for sorting into shortest job first 
void IDSort(ProcessHead *linkedPCBList)
{
    ProcessList *indexNodeLeft;
    ProcessList *indexNodeRight = NULL;
    
    
    //Begin Bubble Sort
    Boolean swapped = False;
    do
    {
        swapped = False;
        indexNodeLeft  = linkedPCBList->processStart;
        
        //Sort via swapping the pcb data
        while(indexNodeLeft->next != indexNodeRight)
        {
            int jobValueNext = indexNodeLeft->next->processBlock->id;
            
            int jobValueIndex = indexNodeLeft->processBlock->id;
            
            if( jobValueIndex > jobValueNext)
            {
                swapPCB(indexNodeLeft, indexNodeLeft->next);
                swapped = True;
            }

            indexNodeLeft = indexNodeLeft->next;
        }
        
        indexNodeRight = indexNodeLeft;
        
    }while(swapped == True);

}


//Function for swapping 2 pcb list nodes pcb data
void swapPCB (ProcessList *linkedPCBListNode1, ProcessList *linkedPCBListNode2)
{
    ProcessControlBlock *tempPCB = linkedPCBListNode1->processBlock;
    linkedPCBListNode1->processBlock = linkedPCBListNode2->processBlock;
    linkedPCBListNode2->processBlock = tempPCB;
}

