/*
* Alvin Lam
* Anthony Dinh
* COP 3402 : System Software
* October 3rd 2014
* Assignment 3 : Tiny PL/0 Parser and Code Generator
*/

#include <CompilerDriver.h>

int main(int argc, char *argv[]){


    int temp, temp2;
    

    //Checks to see if the file exists
    if (!(access( "input.txt", F_OK ) != -1)){
        printf("input.txt does not exist in the current directory\n");
        exit(0);
    }


/*
    char cmd[];
    scanf("%s %s %s", &cmd[1], &cmd[2], &cmd[3]);

    int i = 0;
    for (i <= 2)
    {

    }

    else
    {
        printf("ERROR: Invalid Command\n");
    }*/

    return 0;
}
