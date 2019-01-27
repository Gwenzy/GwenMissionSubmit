//
// Created by gwendal on 1/27/19.
//

#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include "../headers/display.h"
#include "../headers/constants.h"
#include "../headers/struct_student.h"

void clrscr() {
    printf("\e[1;1H\e[2J");
}

void printMenu(int *loop_list, int *exitProg, int port, student *waiting, volatile int *waitingFlag) {
    clrscr();
    printf("------Welcome to Gwen's Mission Validation Monitor. Listening on port %d------\n\n\n\n", (port?port:DEFAULT_PORT));
    printf("1. Show waiting students\n");
    printf("2. Delete a waiting student\n");
    printf("3. Exit program\n");
    char choice;
    do {
        printf("Please make a selection (Press [ENTER] if needed): ");
        choice = (char) mygetch();
        printf("%c", choice);
        switch (choice) {
            case '1':
                *loop_list = 1;
                break;
            case '2':
                printMenuRemoveStudent(waiting, waitingFlag);
                break;
            case '3':
                *exitProg = 1;
                printf("\nThank you for using this program !\n");
                break;
            default:
                printf("\nUnknown choice\n");
                break;
        }
    }while(choice<'1'||choice>'3');





}

void printMenuRemoveStudent(student *waiting, volatile int *waitingFlag) {
    clrscr();
    printf("------Waiting student remove------\n\n\n\n");
    printf("Here is the list of waiting students:\n\n");
    while(!*waitingFlag){}
    *waitingFlag = 0;
    unsigned int numberOfStudents = 0;
    for (int i = 0; i < WAIT_LEN; i++) {
        if (waiting[i].name[0] != '\0') {
            printf("---STUDENT REQUEST ID %d---\n", i+1);
            printf("Student waiting: %s\n"
                   "Mission: %s\n"
                   "XP: %d\n\n", waiting[i].name, waiting[i].mission, waiting[i].xp);
            numberOfStudents++;
        }
    }
    *waitingFlag = 1;
    uintmax_t num;

    do {
        printf("Select the request ID to remove and press [ENTER]: ");
        char *line = getLine();

        num = strtoumax(line, NULL, 10);

    } while((num == UINTMAX_MAX && errno == ERANGE) || num<0 || num>numberOfStudents);
    if(!num)
        return;
    removeStudent(waiting, (int)num, waitingFlag);
}

int mygetch ( void )
{
    int ch;
    struct termios oldt, newt;

    tcgetattr ( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );

    return ch;
}
char * getLine(void) {
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}