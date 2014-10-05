/*
* Alvin Lam
* Anthony Dinh
* COP 3402 : System Software
* October 3rd 2014
* Assignment 3 : Tiny PL/0 Parser and Code Generator
*/

#include <CompilerDriver.h>

int main(int argc, char *argv[]){

    FILE *ifp, *ofp1, *ofp2, *ofp3, *ofp4;

    int i;

    //These variables determine whether to print the Lexemes, Assembly code, or the Stack Trace to the screen
    int printLex = 0;
    int printPars = 0;
    int printStack = 0; 


    //Checks to see if the file exists
    if (!(access( "input.txt", F_OK ) != -1)){
        printf("input.txt does not exist in the current directory\n");
        exit(0);
    }

    ifp = fopen("input.txt", "r");
    ofp1 = fopen("output.txt", "w");
    ofp2 = fopen("cleaninput.txt", "w");
    ofp3 = fopen("lexemetable.txt", "w");
    ofp4 = fopen("lexemelist.txt", "w");

    //Checks to see if the number of arguments is correct
    if (argc > 4){

        printf("Invalid number of arguments: %d", argc);
        fprintf(ofp1, "Invalid number of arguments: %d", argc);

        fclose(ifp);
        fclose(ofp1);
        fclose(ofp2);
        fclose(ofp3);
        fclose(ofp4);

        exit(0);
    }

    //Checks to see what arguments are passed
    for (i = 0; i < argc; i++){

        if (argv[i] == 'l'){

            printLex = 1;
            return;
        }

        if (argv[i] == 'a'){

            printPars = 1;
            return;
        }

        if (argv[i] == 'v'){

            printStack = 1;
            return;
        }

        else {

            printf("Invalid argument: %c", argv[i]);
            fprintf(ofp1, "Invalid argument: %c", argv[i]);

            fclose(ifp);
            fclose(ofp1);
            fclose(ofp2);
            fclose(ofp3);
            fclose(ofp4);

            exit(0);
        }

    }

    










    return 0;
}
