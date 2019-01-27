//
// Created by gwendal on 1/27/19.
//

#ifndef GWENMISSIONSUBMIT_DISPLAY_H
#define GWENMISSIONSUBMIT_DISPLAY_H

#include "struct_student.h"

void printMenu(int *loop_list, int *exitProg, int port, student *waiting, volatile int *waitingFlag);

void printMenuRemoveStudent(student *waiting, volatile int *waitingFlag);
void clrscr();

char * getLine(void);

int mygetch(void);
#endif //GWENMISSIONSUBMIT_DISPLAY_H
