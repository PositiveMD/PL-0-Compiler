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

    if (argc )







    return 0;
}
