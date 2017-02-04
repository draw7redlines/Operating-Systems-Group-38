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


    //testing to see if we populated it correctly
    //delete this later or this will never work!!!
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

}














