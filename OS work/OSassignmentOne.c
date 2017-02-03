#include <stdio.h>
#include <stdlib.h>
#include <math.h>




char *usefulWords[999];





void headerMaker(char **wordList);



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










