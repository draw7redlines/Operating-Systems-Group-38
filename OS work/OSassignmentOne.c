#include <stdio.h>
#include <stdlib.h>
#include <math.h>




typedef struct node{
char processNumber[20];
int arrivalTime;
int burstTime;
struct node *next;
int serialNum;
int finishTime;
int originalBurst;
}listNode;






void firstComeFirstServed(char **keyWord, FILE *outfile);
void headerMaker(char **wordList, FILE *outFile);
void roundRobin(char **keyWord, FILE *outFile);
void shortestJobFirst(char** keyWord, FILE *outFile);


void main()
{

    //gonna approach this by reading in with space delimeter and
    //cutting line at #
    char *keyword[999];
    char someString[999];
    char *token;
    char *helper;

    //let's get a basic file reader function first...
    FILE *finput = fopen("set1_process.in", "r");
    FILE *foutput = fopen("processes.out", "w");

    //file exception catch
    if(finput==NULL)
    {
        printf("Error: Could not find the source file \n");
        exit(0);
    }


    int flag =0;

    //while we still have strings
    while(fgets(someString, 999, finput)!= NULL)
    {
        //we only want what's before the #
        token = strtok(someString, "#");

        //if we have a valid word add it to our array
        if(token!= NULL)
        {

            //parse the spaces
            helper= strtok(token, " ");

            while(helper!= NULL)
            {

            //printf("flag value is %d \n", flag);
            keyword[flag] = malloc(50 *sizeof(char));
            strcpy(keyword[flag],helper);
            //printf("Got the current token as: %s \n", keyword[flag]);
            flag++;

            helper = strtok(NULL, " ");
            }
        }
    }

    headerMaker(keyword, foutput);
    fprintf(foutput, "\n");


    if(strcmp(keyword[5], "rr")==0)
    {
        roundRobin(keyword, foutput);
    }
    if(strcmp(keyword[5], "sjf")==0)
    {
        shortestJobFirst(keyword, foutput);
    }

    if(strcmp(keyword[5], "fcfs")==0)
    {
        firstComeFirstServed(keyword, foutput);
    }


    fclose(finput);
    fclose(foutput);

    return;
}



//making a heading function since process count, method, and possible quantum are pretty universal on the outputs
void headerMaker(char **wordList, FILE *outFile)
{

    int processHelper;
    int quantumHelper;


    processHelper = atoi(wordList[1]);


        fprintf(outFile,"%d processes \n", processHelper);
            if(strcmp(wordList[5], "rr")==0)
            {
                fprintf(outFile,"Using Round-Robin\n");

                quantumHelper = atoi(wordList[7]);
                fprintf(outFile,"Quantum %d \n", quantumHelper);

            }
            else if(strcmp(wordList[5], "fcfs")==0)
            {

                fprintf(outFile,"Using First Come First Served\n");
            }

            else if(strcmp(wordList[5], "sjf")==0)
            {
                fprintf(outFile,"Using Shortest Job First (Pre)\n");
            }




    return;

}





//let's get some red robin's, I mean round robin going
void roundRobin(char **keyWord, FILE *outFile)
{
    int processCount;
    int runTime;
    int quantum;
    int i;
    int serialTrack =0;


    //linked lists are so much fun to make in c, let's build one
    listNode *root;
    root = (struct node *) malloc(sizeof(struct node));
    root=NULL;


    processCount = atoi(keyWord[1]);
    runTime = atoi(keyWord[3]);
    quantum = atoi(keyWord[7]);

    //populate the list based on our amount of processes
    for(i=0; i<processCount; i++)
    {
        listNode *temp;
        temp = (struct node *) malloc(sizeof(struct node));

        strcpy(temp->processNumber,keyWord[(i*7)+10]);
        temp->arrivalTime = atoi(keyWord[(i*7)+12]);
        temp->burstTime = atoi(keyWord[(i*7)+14]);
        temp->serialNum= serialTrack;
        temp->originalBurst = atoi(keyWord[(i*7)+14]);
        temp->next= root;
        root=temp;
        serialTrack++;
    }


    listNode *temp;
    temp = (struct node *) malloc(sizeof(struct node));


    i=0;



    temp=root;
    sortListByArrivalTime(temp);
    root=temp;


    //start of the actual processing step
    listNode *systemsTracker;
    systemsTracker = (struct node *) malloc(sizeof(struct node));

    int time=0;
    int idleCheck = 0;
    int gameOver=0;
    int currentProc = 0;
    int maxProcNum = 0;
    int jumpProcFlag = 0;
    int somethingFinished = 0;
    int quantumCatch =0;

    while(time<=runTime && gameOver==0)
    {
        //termination case set
        gameOver = 1;
        jumpProcFlag = 0;
        somethingFinished =0;
        quantumCatch =0;

        //process arrives, which oversteps standard quantum rules and
        //makes a report no matter what
        temp=root;
        for(i=0; i<processCount; i++)
        {
            if(temp->arrivalTime==time)
            {
                fprintf(outFile,"Time %d: %s arrived\n", time, temp->processNumber);
                idleCheck++;
                maxProcNum++;

                //catches which are used later down the line to manage counter increment shenanigans
                // when a process arrives
                jumpProcFlag =1;
                quantumCatch =1;

                if((time)%quantum==0)
                {
                    jumpProcFlag=0;
                    currentProc=maxProcNum;
                }

            }
            temp=temp->next;
        }


        //process finishes, which oversteps quantum rules and makes
        //a report no matter what
        temp=root;
        for(i=0; i<processCount; i++)
        {
            if(temp->burstTime==0)
            {
                fprintf(outFile,"Time %d: %s finished\n", time, temp->processNumber);
                idleCheck--;
                maxProcNum--;
                temp->burstTime=-1;
                currentProc++;
                somethingFinished=1;
                //finishTimeKeeper[i] = time;
                temp->finishTime=time;
            }
            temp=temp->next;
        }


        if(currentProc>maxProcNum)
        {
            if(idleCheck==0)
                currentProc=0;
            else
                currentProc=1;
        }



        //pick our poison (process)
        //find currentProc instance of node without -1 burst. currentProc increments until it hits
        //max process which is highest node it should go up to. Then it resets to the
        //first instance of a node without a -1 burst, then increments up to the next one
        // without exceeding max process number
        temp=root;
        int instance = 1;
        for(i=0; i<processCount; i++)
        {
            if(instance<currentProc&&temp->burstTime!=-1)
            {
                temp=temp->next;
                instance++;
            }

            else if(instance<currentProc)
            {
                temp=temp->next;
            }
        }



        //systems report on current operations, this is for all the marbles
        if(time%quantum==0 || somethingFinished == 1)
        {
            //it is idling
            if(idleCheck==0)
            {
                fprintf(outFile,"Time %d: IDLE\n", time);
            }

            //we've got something selected, and we report it's remaining burst
            else
            {
                fprintf(outFile,"Time %d: %s selected (burst %d) \n", time, temp->processNumber, temp->burstTime);
            }


        }


        //we're not idling so reduce the burst time of the current process
        if(idleCheck>0)
        {
                temp->burstTime--;

        }



        //shift the process if we're on a quantum
        //this had to be modded with a +1 because I didn't plan ahead when I first wrote this
        //and because of how I increment my time counter, this has to be shifted by one.
        //hooray for cowboy coding this earlier
        if((time+1)%quantum==0)
        {
            if(quantumCatch!=1)
            {
                currentProc++;
            }
        }



        //check for the need to reset the process counter before the loop runs again
        if(currentProc>maxProcNum)
        {
            if(idleCheck==0)
                currentProc=0;
            else
                currentProc=1;
        }



        //we're done, let's get out of here ghost rider
        systemsTracker=root;
        for(i=0; i<processCount; i++)
        {
            if(systemsTracker->burstTime!=-1)
                gameOver=0;

            systemsTracker=systemsTracker->next;
        }

        //increment the time
        time++;



        //if something was added and it wasn't on a quantum iteration, then we need to increment to account for it
        if(jumpProcFlag==1)
        {
            currentProc++;
        }

    }

    //print out the runtime used
    fprintf(outFile,"Finished at time %d\n", time);
    fprintf(outFile, "\n");


    //I'm gonna add turnaround and wait time here later, I'm tired right now...
    temp=root;
    int wait;
    int turnaround;


    for(i=0; i<processCount; i++)
    {
        int j;
        for(j=0; j<processCount; j++)
        {
            if(temp->serialNum==i)
            {
            wait = (temp->finishTime - temp->arrivalTime - temp->originalBurst);
            turnaround = (temp->finishTime - temp->arrivalTime);
            fprintf(outFile,"%s wait %d turnaround %d \n", temp->processNumber, wait, turnaround);
            }

            temp=temp->next;
        }
        temp=root;
    }
}

void shortestJobFirst(char** keyWord, FILE *outFile)
{
    int processCount;
    int runTime;
    int quantum;
    int finishedProcs = 0;
    char buffer[1024];



    //linked lists are so much fun to make in c, let's build one
    listNode *root;
    root = (struct node *) malloc(sizeof(struct node));
    root=NULL;

    listNode *rockRoot;
    rockRoot = (struct node *) malloc(sizeof(struct node));
    rockRoot=NULL;

    //temp->next=NULL;

    processCount = atoi(keyWord[1]);
    runTime = atoi(keyWord[3]);
    quantum = atoi(keyWord[7]);

    int i;

    //populate the list based on our amount of processes
    for(i=0; i<processCount; i++)
    {
        listNode *temp;
        temp = (struct node *) malloc(sizeof(struct node));

        strcpy(temp->processNumber,keyWord[(i*7)+10]);
        temp->arrivalTime = atoi(keyWord[(i*7)+12]);
        temp->burstTime = atoi(keyWord[(i*7)+14]);
        temp->originalBurst = temp->burstTime;
        temp->next= root;
        root=temp;
        rockRoot = temp;
    }


    listNode *temp;
    temp = (struct node *) malloc(sizeof(struct node));


    temp=root;
    sortListByArrivalTime(temp);
    root=temp;
    //rockRoot = temp;

    int time = 0;
    int idleCheck = 0;
    int processSelected = 0;
    int lowBurst = -1;


    while(time <= runTime)
    {
        //Arrival condition
        temp = root;

        for(i = 0; i < processCount; i++)
        {
            if(temp->arrivalTime == time)
            {
                fprintf(outFile,"Time %d: %s arrived\n",time ,temp->processNumber);


                if(processSelected == 0)
                {
                    fprintf(outFile,"Time %d: %s selected (burst %d)\n",time ,temp->processNumber,temp->burstTime);

                    processSelected = 1;
                    lowBurst = temp->burstTime;
                }
                else if((processSelected == 1) && (temp->burstTime < lowBurst))
                {
                    fprintf(outFile,"Time %d: %s selected (burst %d)\n",time ,temp->processNumber,temp->burstTime);


                    listNode *swapNode;
                    swapNode = (struct node *) malloc(sizeof(struct node));

                    swapNode = root;
                    sortListByLowBurstTime(swapNode);
                    root = swapNode;


                    processSelected = 1;
                    lowBurst = root->burstTime;
                }


            }

            temp = temp->next;
        }


        temp = root;

       if((idleCheck != 1) && (temp->burstTime == 0))
       {
            fprintf(outFile,"Time %d: %s finished\n",time ,temp->processNumber);
            temp->finishTime = time;

            processCount--;

            struct node *newRoot;

            if(root->next != NULL)
            {
                newRoot = root;
                root = root->next;

                lowBurst = root->burstTime;

                if(root->burstTime <= lowBurst)
                    fprintf(outFile,"Time %d: %s selected (burst %d)\n",time ,root->processNumber,root->burstTime);
            }


       }

        temp = root;

        if((processCount == 0) && (time != runTime))
        {
            fprintf(outFile,"Time %d: IDLE\n",time);
            idleCheck = 1;
        }



        if(time == runTime)
        {
            fprintf(outFile,"Finished at time %d\n\n",time);

            sortListByProcNumber(rockRoot);
            while(rockRoot)
            {

                int wait = (rockRoot->finishTime - rockRoot->arrivalTime - rockRoot->originalBurst);
                int turnaround = (rockRoot->finishTime - rockRoot->arrivalTime);

                fprintf(outFile,"%s wait %d turnaround %d\n",rockRoot->processNumber,wait,turnaround);

                rockRoot = rockRoot->next;
            }

        }



        if(temp->burstTime > 0)
            temp->burstTime--;



        time++;

    }

}


void sortListByArrivalTime(struct node *temp)
{
    int switchFlag, i;
    struct node *helper= NULL;
    struct node *helperTwo = NULL;
    struct node *root = temp;

    helper = (struct node *) malloc(sizeof(struct node));

    if(root==NULL)
    {
        return;
    }

    switchFlag=1;

    while(switchFlag!=0)
    {
        switchFlag=0;
        root=temp;
        while(root->next!=NULL)
        {

            if(root->arrivalTime>root->next->arrivalTime)
            {

                //let's just switch the data instead of the actual
                //nodes and save a big headache here
                helper->arrivalTime=root->arrivalTime;
                helper->burstTime= root->burstTime;
                helper->finishTime = root->finishTime;
                helper->originalBurst = root->originalBurst;
                strcpy(helper->processNumber, root->processNumber);
                helper->serialNum= root->serialNum;


                root->arrivalTime=root->next->arrivalTime;
                root->finishTime = root->next->finishTime;
                root->originalBurst = root->next->originalBurst;
                root->burstTime=root->next->burstTime;
                strcpy(root->processNumber,root->next->processNumber);
                root->serialNum = root->next->serialNum;

                root->next->arrivalTime=helper->arrivalTime;
                root->next->finishTime=helper->finishTime;
                root->next->originalBurst=helper->originalBurst;
                root->next->burstTime=helper->burstTime;
                strcpy(root->next->processNumber,helper->processNumber);
                root->next->serialNum = helper->serialNum;

                switchFlag=1;
            }

            root=root->next;
        }


    }

}

// I guess you could say it came first
void firstComeFirstServed(char **keyWord, FILE *outfile)
{
    int processCount;
    int runTime;
    int i, j;
    int time=0;
    int idleCheck = 0;
    int gameOver=0;
    int currentProc = 0;
    int maxProcNum = 0;
    int jumpProcFlag = 0;
    int finProcs = 0;

    //linked lists are so much fun to make in c, let's build one
    listNode *root;
    root = (struct node *) malloc(sizeof(struct node));
    root=NULL;

    listNode *rockRoot;
    rockRoot = (struct node *) malloc(sizeof(struct node));
    rockRoot=NULL;

    //temp->next=NULL;

    processCount = atoi(keyWord[1]);
    runTime = atoi(keyWord[3]);

    //populate the list based on our amount of processes
    for(i=0; i<processCount; i++)
    {
        listNode *temp;
        temp = (struct node *) malloc(sizeof(struct node));

        strcpy(temp->processNumber,keyWord[(i*7)+10]);
        temp->arrivalTime = atoi(keyWord[(i*7)+12]);
        temp->burstTime = atoi(keyWord[(i*7)+14]);
        temp->originalBurst = temp->burstTime;
        temp->next= root;
        root=temp;
        rockRoot = temp;
    }


    listNode *temp;
    temp = (struct node *) malloc(sizeof(struct node));

    temp=root;
    sortListByArrivalTime(temp);
    root=temp;

    int selectP = 0;
    temp = root;
    // Currently works, but it only sticks to P3 for some reason
    while(time <= runTime)
    {
        for(i = 0; i < processCount; i++)
        {

            // Shows which process
            if(temp->arrivalTime==time)
            {
                fprintf(outfile, "Time %d: %s arrived\n", time, temp->processNumber);
            }

            if(selectP == 0)
            {
                fprintf(outfile, "Time %d: %s selected (burst %d) \n", time, temp->processNumber, temp->burstTime);
                selectP++;
            }

            if(temp->burstTime == 0)
            {
                fprintf(outfile, "Time %d: %s finished\n", time, temp->processNumber);
                temp->finishTime = time;
                finProcs++;
                selectP = 0;



                listNode *newRoot;
                newRoot = (struct node *) malloc(sizeof(struct node));

                newRoot = rockRoot;

                if(root->next != NULL)
                {
                    newRoot = root;
                    root = root->next;
                }
            }

            if(temp->next != NULL)
                temp=temp->next;
            else
                break;
        }


        temp = root;

        if(finProcs == processCount && time < runTime)
        {
            fprintf(outfile, "Time %d: IDLE\n",time);
        }



        if(temp->burstTime > 0)
            temp->burstTime--;


        time++;
    }

    time--;
    if(time <= runTime)
    {
        fprintf(outfile, "Finished at time %d\n\n", time);

        sortListByProcNumber(rockRoot);

        while(rockRoot)
        {
            int wait = (rockRoot->finishTime - rockRoot->arrivalTime - rockRoot->originalBurst);
            int turnaround = (rockRoot->finishTime - rockRoot->arrivalTime);

            fprintf(outfile, "%s wait %d turnaround %d\n",rockRoot->processNumber,wait,turnaround);

            rockRoot = rockRoot->next;
        }

    }


}

void sortListByProcNumber(struct node *temp)
{
    int switchFlag, i;
    struct node *helper= NULL;
    struct node *helperTwo = NULL;
    struct node *root = temp;

    helper = (struct node *) malloc(sizeof(struct node));

    if(root==NULL)
    {
        return;
    }

    switchFlag=1;

    while(switchFlag!=0)
    {
        switchFlag=0;
        root=temp;
        while(root->next!=NULL)
        {

            if(strcmp(root->processNumber,root->next->processNumber) > 0)
            {

                //let's just switch the data instead of the actual
                //nodes and save a big headache here
                helper->arrivalTime=root->arrivalTime;
                helper->burstTime= root->burstTime;
                helper->finishTime = root->finishTime;
                helper->originalBurst = root->originalBurst;
                strcpy(helper->processNumber, root->processNumber);
                helper->serialNum= root->serialNum;


                root->arrivalTime=root->next->arrivalTime;
                root->finishTime = root->next->finishTime;
                root->originalBurst = root->next->originalBurst;
                root->burstTime=root->next->burstTime;
                strcpy(root->processNumber,root->next->processNumber);
                root->serialNum = root->next->serialNum;

                root->next->arrivalTime=helper->arrivalTime;
                root->next->finishTime=helper->finishTime;
                root->next->originalBurst=helper->originalBurst;
                root->next->burstTime=helper->burstTime;
                strcpy(root->next->processNumber,helper->processNumber);
                root->next->serialNum = helper->serialNum;

                switchFlag=1;
            }

            root=root->next;
        }


    }

}

void sortListByLowBurstTime(struct node *temp)
{
    int switchFlag, i;
    struct node *helper= NULL;
    struct node *helperTwo = NULL;
    struct node *root = temp;

    helper = (struct node *) malloc(sizeof(struct node));

    if(root==NULL)
    {
        return;
    }

    switchFlag=1;

    while(switchFlag!=0)
    {
        switchFlag=0;
        root=temp;
        while(root->next!=NULL)
        {

            if(root->burstTime > root->next->burstTime)
            {

                //let's just switch the data instead of the actual
                //nodes and save a big headache here
                helper->arrivalTime=root->arrivalTime;
                helper->burstTime= root->burstTime;
                helper->originalBurst = root->originalBurst;
                strcpy(helper->processNumber, root->processNumber);
                helper->serialNum= root->serialNum;


                root->arrivalTime=root->next->arrivalTime;
                root->originalBurst = root->next->originalBurst;
                root->burstTime=root->next->burstTime;
                strcpy(root->processNumber,root->next->processNumber);
                root->serialNum = root->next->serialNum;

                root->next->arrivalTime=helper->arrivalTime;
                root->next->originalBurst=helper->originalBurst;
                root->next->burstTime=helper->burstTime;
                strcpy(root->next->processNumber,helper->processNumber);
                root->next->serialNum = helper->serialNum;

                switchFlag=1;
            }

            root=root->next;
        }


    }

}
