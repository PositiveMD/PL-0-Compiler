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

    FILE *ifp, *ofp, *ofp2;

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

    //Checks to see if the file exists
    if (!(access( "lexemelist.txt", F_OK ) != -1)){
        printf("input.txt does not exist in the current directory\n");
        exit(1);
    }


    ifp = fopen("lexemelist.txt", "r");
    ofp = fopen("mcode.txt", "w");
    ofp2 = fopen("output.txt", "a");

    



	return 0;
}