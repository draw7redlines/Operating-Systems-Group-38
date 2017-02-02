#include <stdio.h>
#include <stdlib.h>
#include <math.h>




char *usefulWords[999];



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


    fclose(finput);

    return;
}
