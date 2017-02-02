#include <stdio.h>
#include <stdlib.h>
#include <math.h>




char *keyword[99999];
char *usefulWords[99999];



void main()
{

    //gonna approach this by reading in with space delimeter and
    //cutting line at #
    char someString[99999];
    char delimiter = " ";
    char stopFlag = "#";
    char *token;

    //let's get a basic file reader function first...
    FILE *finput = fopen("set1_process.in", "r");

    //file exception catch
    if(finput==NULL)
    {
        printf("Error: Could not find the source file \n");
        exit(0);
    }


    int flag =0;

    //while we still have strings
    while(fgets(someString, 99999, finput)!= NULL)
    {
        //we only want what's before the #
        token = strtok(someString, "#");

        //if we have a valid word add it to our array
        if(token!= NULL)
        {
            keyword[flag] = token;
            printf("Got the current token as: %s \n", keyword[flag]);
        }

        flag++;
    }


    system("pause");

    //seperate our good words on the spaces
    breakupDeliminatedStrings();

    printf("printing out fully parsed strings... \n");
    int i=0;
    while(usefulWords[i]!=NULL)
    {
        printf(usefulWords[i]);
    }

    fclose(finput);

    return;
}


void breakupDeliminatedStrings()
{
    //ugly counters
    int stupidFlag = 0;
    int stupidFlagTwo = 0;
    int counter = 0;
    int counterTwo = 0;


    //these checkpoints were me debugging, don't worry about them
    printf("checkpoint 1\n");

    //while we have a valid word on our array keep going
    while(keyword[stupidFlag]!=NULL)
    {


        printf("checkpoint 2\n");

        //while we see characters that are not spaces, start copying into a new string on our empty array
        while(keyword[stupidFlag][stupidFlagTwo]!= " ")
        {
                    printf("checkpoint 3\n");



                    //the code is breaking here, because it thinks we're referencing the wrong word, and then hangs on the copy statement
                    //I may have done this incorrectly, if you see a fix I couldn't get, then feel free to drop it in
                    printf("trace on breakpoint: \n");
                    printf("stupid flag is: %d \n", stupidFlag);
                    printf("counter is: %d \n", counter);
                    printf("stupidflagtwo is: %d \n", stupidFlagTwo);
                    printf("source word is: %s \n", keyword[0]);
                    printf("char at destination is: %c\n", keyword[stupidFlag][stupidFlagTwo]);



            strcpy(usefulWords[stupidFlag][counter], keyword[stupidFlag][stupidFlagTwo]);
            stupidFlagTwo++;
            counter++;
        }


        //we hit a space, so reset the counter on our target array to zero to copy a new word into the next index
        //and increment the counter on our source array to skip the space it found
        counter=0;
        stupidFlagTwo++;

                printf("checkpoint 4\n");


                //if we hit the end on our source array then increment its second counter to go to the next index, and copy
                //the next string
        if(keyword[stupidFlag][stupidFlagTwo]==NULL){

                        printf("checkpoint 5\n");


            stupidFlagTwo=0;
            stupidFlag++;
        }
    }

                printf("checkpoint 6\n");



    return;
}
