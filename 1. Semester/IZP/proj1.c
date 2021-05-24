/*
* IZP PROJEKT 1
* AUTHOR: Vojtech Fiala
* LOGIN: xfiala61
*/
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 102 //maximum length of the stdin to be used in other functions
#define ASCII_0 48 // ascii value of number 0, to be used in a check if the argument is made only of numbers
#define ASCII_9 57

//function to print all contacts
void print_all() {
    char line1[MAX_LENGTH];
    char line2[MAX_LENGTH];
    // load the whole stdin, split it into two rows (one for name, the other for number)
    while (fgets(line1, MAX_LENGTH, stdin) && fgets(line2, MAX_LENGTH, stdin)) {
            line1[strlen(line1) - 1] = '\0'; //remove newline character from the 'name' row
            printf("%s, %s", line1, line2);
            }
}

//function to find matches in names/numbers
int check(char search[MAX_LENGTH], char argument[MAX_LENGTH]) {

    char help[MAX_LENGTH];
    strcpy(help, search);
        for (int i = 0; i < (int) strlen(search); i++) { // Check each character of the name/number
            // If a character matches its corresponding number, rewrite it to that number
            switch(search[i]) {
                case '+':
                    help[i] = '0'; // if the character is '+', replace it with '0' and so on
                    break;
                case 'a':
                case 'A':
                case 'b':
                case 'B':
                case 'c':
                case 'C':
                    help[i] = '2';
                    break;
                case 'd':
                case 'D':
                case 'e':
                case 'E':
                case 'F':
                case 'f':
                    help[i] = '3';
                    break;
                case 'g':
                case 'G':
                case 'h':
                case 'H':
                case 'i':
                case 'I':
                    help[i] = '4';
                    break;
                case 'j':
                case 'J':
                case 'k':
                case 'K':
                case 'l':
                case 'L':
                    help[i] = '5';
                    break;
                case 'm':
                case 'M':
                case 'n':
                case 'N':
                case 'o':
                case 'O':
                    help[i] = '6';
                    break;
                case 'p':
                case 'P':
                case 'q':
                case 'Q':
                case 'r':
                case 'R':
                case 's':
                case 'S':
                    help[i] = '7';
                    break;
                case 't':
                case 'T':
                case 'u':
                case 'U':
                case 'v':
                case 'V':
                    help[i] = '8';
                    break;
                case 'w':
                case 'W':
                case 'x':
                case 'X':
                case 'y':
                case 'Y':
                case 'z':
                case 'Z':
                    help[i] = '9';
                    break;
            }
        }
        // check if the converted name/number matches the argument
        if (strstr(help, argument)) { 
            return 1;
        }
    return 0;
}

int main(int argc, char *argv[]) {
    int count = 0; 
    char line1[MAX_LENGTH];
    char line2[MAX_LENGTH];
    char argument[MAX_LENGTH]; 
    // Check how many arguments has the user entered
    if (argc > 2) {
        fprintf(stderr, "I accept only 1 parameter\n");
        return 1;
    }
    else if (argc < 2) {
        print_all();
        return 0;
    }
    // If the user entered only 1 argument (plus the file argument), search the file for matches
    else {
        //check if the argument consists only of numbers 0-9
        for (int o = 0; o < (int) strlen(argv[1]); o++) {
            if (argv[1][o] < ASCII_0 || argv[1][o] > ASCII_9) { //Check with the ascii value of numbers 0-9
                fprintf(stderr, "I only accept numbers\n");
                return 1;
            }
        }
        strcpy(argument, argv[1]);
        while (fgets(line1, MAX_LENGTH, stdin) && fgets(line2, MAX_LENGTH, stdin)) {
            line1[strlen(line1) - 1] = '\0'; // Remove newline character from the 'name' row
            //check if it found a match in either a name or a number line
            if (check(line2, argument) || check(line1, argument) == 1)  { 
                count++;
                printf("%s, %s", line1, line2);
            }
        }
        if (count < 1) { // If there is no match, it will say so
            printf("Not found\n");
            return 0;
        }
        return 0;
    }
}