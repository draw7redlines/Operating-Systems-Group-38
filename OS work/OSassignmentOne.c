#include <stdio.h>
#include <stdlib.h>
#include <math.h>




typedef struct node{
char processNumber[20];
int arrivalTime;
int burstTime;
struct node *next;
}listNode;





void headerMaker(char **wordList);
//void roundRobin(char **keyWord);



void main()
{

    //gonna approach this by reading in with space delimeter and
    //cutting line at #
    char *keyword[999];
    char someString[999];
    char *token;
    char *helper;

    //let's get a basic file reader function first...
    FILE *finput = fopen("set2_process.in", "r");

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

            printf("flag value is %d \n", flag);
            keyword[flag] = malloc(50 *sizeof(char));
            strcpy(keyword[flag],helper);
            printf("Got the current token as: %s \n", keyword[flag]);
            flag++;

            helper = strtok(NULL, " ");
            }
        }
    }

    headerMaker(keyword);


    if(strcmp(keyword[5], "rr")==0)
    {
        roundRobin(keyword);
    }


    fclose(finput);

    return;
}



//making a heading function since process count, method, and possible quantum are pretty universal on the outputs
void headerMaker(char **wordList)
{

    int processHelper;
    int quantumHelper;


    processHelper = atoi(wordList[1]);


        printf("\n\n\n");
        printf("Below will print to file\n");
        printf("=======================================\n\n");
        printf("%d processes \n", processHelper);
            if(strcmp(wordList[5], "rr")==0)
            {
                printf("using Round-Robin\n");

                quantumHelper = atoi(wordList[7]);
                printf("Quantum %d \n", quantumHelper);

            }
            else if(strcmp(wordList[5], "fcfs")==0)
            {

                printf("using First Come First Serve\n");
            }

            else if(strcmp(wordList[5], "sjf")==0)
            {
                printf("using Shortest Job First\n");
            }




    return;

}





//let's get some red robin's, I mean round robin going
void roundRobin(char **keyWord)
{
    int processCount;
    int runTime;
    int quantum;


    //linked lists are so much fun to make in c, let's build one
    listNode *root;
    root = (struct node *) malloc(sizeof(struct node));
    root=NULL;

    //temp->next=NULL;

    processCount = atoi(keyWord[1]);
    runTime = atoi(keyWord[3]);
    quantum = atoi(keyWord[7]);

    //populate the list based on our amount of processes
    for(int i=0; i<processCount; i++)
    {
        listNode *temp;
        temp = (struct node *) malloc(sizeof(struct node));

        strcpy(temp->processNumber,keyWord[(i*7)+10]);
        temp->arrivalTime = atoi(keyWord[(i*7)+12]);
        temp->burstTime = atoi(keyWord[(i*7)+14]);
        temp->next= root;
        root=temp;
    }


    listNode *temp;
    temp = (struct node *) malloc(sizeof(struct node));



    printf("===========here's the contents of the linked list==========");
    temp = root;
    while(temp!=NULL)
    {
        printf("\n");
        printf("process name: %s \n", temp->processNumber);
        printf("arrival time: %d \n", temp->arrivalTime);
        printf("burst time: %d \n", temp->burstTime);
        temp=temp->next;
    }



    temp=root;
    sortListByArrivalTime(temp);
    root=temp;

    printf("===========here's the contents of the linked list after a sort==========");
    while(temp!=NULL)
    {
        printf("\n");
        printf("process name: %s \n", temp->processNumber);
        printf("arrival time: %d \n", temp->arrivalTime);
        printf("burst time: %d \n", temp->burstTime);
        temp=temp->next;
    }


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
    //int minProcNum = 0;

    while(time<=runTime && gameOver==0)
    {
        //termination case set
        gameOver=1;
        jumpProcFlag = 0;
        somethingFinished =0;
        quantumCatch =0;

        //process arrives, which oversteps standard quantum rules and
        //makes a report no matter what
        temp=root;
        for(int i=0; i<processCount; i++)
        {
            if(temp->arrivalTime==time)
            {
                printf("Time %d: %s arrived\n", time, temp->processNumber);
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
        for(int i=0; i<processCount; i++)
        {
            if(temp->burstTime==0)
            {
                printf("Time %d: %s finished\n", time, temp->processNumber);
                idleCheck--;
                maxProcNum--;
                temp->burstTime=-1;
                currentProc++;
                somethingFinished=1;
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
        for(int i=0; i<processCount; i++)
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
                printf("Time %d: IDLE\n", time);
            }

            //we've got something selected, and we report it's remaining burst
            else
            {
                printf("Time %d: %s selected (burst %d) \n", time, temp->processNumber, temp->burstTime);
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
        for(int i=0; i<processCount; i++)
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
    printf("Finished at time %d\n", time);


    //I'm gonna add turnaround and wait time here later, I'm tired right now...




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
                strcpy(helper->processNumber, root->processNumber);


                root->arrivalTime=root->next->arrivalTime;
                root->burstTime=root->next->burstTime;
                strcpy(root->processNumber,root->next->processNumber);

                root->next->arrivalTime=helper->arrivalTime;
                root->next->burstTime=helper->burstTime;
                strcpy(root->next->processNumber,helper->processNumber);

                switchFlag=1;
            }

            root=root->next;
        }


    }

}

















