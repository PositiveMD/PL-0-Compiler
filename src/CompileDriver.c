/*
* Alvin Lam
* Anthony Dinh
* COP 3402 : System Software
* October 3rd 2014
* Assignment 3 : Tiny PL/0 Parser and Code Generator
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//Checks to see if the environment is windows or not
#ifdef _WIN32

    #define IS_WINDOWS 1    

#else

    #define IS_WINDOWS 0

#endif

int main(int argc, char *argv[]){

    int i;

    //These variables determine whether to print the Lexemes, Assembly code, or the Stack Trace to the screen
    int printLex = 0;
    int printPars = 0;
    int printStack = 0;

    int continueExecution = 1;

    //Checks to see if the number of arguments is correct
    if (argc > 4){

        printf("Invalid number of arguments: %d", argc);

        exit(0);
    }

    //Checks to see what arguments are passed
    for (i = 1; i < argc; i++){

        if (strcmp(argv[i],"-l") == 0){

            printLex = 1;
            continue;
        }

        if (strcmp(argv[i], "-a") == 0){

            printPars = 1;
            continue;
        }

        if (strcmp(argv[i], "-v") == 0){

            printStack = 1;
            continue;
        }

        else {

            printf("Invalid argument: %s \nArgument number: %d\n", argv[i], i );


        }

    }

    if (IS_WINDOWS){

        continueExecution = (printLex == 1) ? system("scanner.exe -l") : system("scanner.exe");

        //Start Parser Code

        if (!continueExecution)
            continueExecution = (printStack == 1) ? system("P-Machine.exe -v") : system("P-Machine.exe");
    }

    else {


        continueExecution = (printLex == 1) ? system("./scanner -l") : system("./scanner");

        //Start Parser Code

        if (!continueExecution)
            continueExecution = (printStack == 1) ? system("./P-Machine -v") : system("P-Machine");
    }
	   

    
      
        














    return 0;
}
