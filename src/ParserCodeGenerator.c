/*
Anthony Dinh
Alvin Lam
COP 3402 : System Software
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{

	int i;
	int printPars = 0;

	 //Checks to see if the number of arguments is correct
    if (argc > 2){

        printf("Invalid number of arguments: %d", argc);

        exit(1);
    }

    //Checks to see what arguments are passed
    for (i = 1; i < argc; i++){

        if (strcmp(argv[i],"-a") == 0){

            printPars = 1;
            continue;

        }

        else {

            printf("Invalid argument: %s \nArgument number: %d\n", argv[i], i );
            exit(1);


        }

    }

	return 0;
}