//
// Created by gwendal on 1/26/19.
//

#include <memory.h>
#include <stdio.h>
#include <unistd.h>
#include "../headers/struct_student.h"
#include "../headers/constants.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfor-loop-analysis"
void removeStudent(student *waiting, int num, volatile int *waitingFlag) {
    while(!*waitingFlag) {}
    *waitingFlag = 0;
    for(;num<WAIT_LEN;num++){
        strcpy(waiting[num-1].name, waiting[num].name);
        strcpy(waiting[num-1].mission, waiting[num].mission);
        waiting[num-1].xp = waiting[num].xp;
    }
    strcpy(waiting[num].name, "");
    *waitingFlag = 1;
}

void addStudent(student *waiting, volatile int *waitingFlag, student student) {
    while(!*waitingFlag) {}
    *waitingFlag = 0;
    {
        int xp = student.xp;
        int index = 0;
        int foundPlace = 0;

        do{
            if(waiting[index].name[0]== '\0')
                foundPlace = 1;
            else if(xp<=waiting[index].xp)
                foundPlace = 2;
            else
                index++;
        }while(!foundPlace);

        //We have to move everything from index to the right (including index), and add student to waiting[index]
        for(int i = WAIT_LEN-1; i>index; i--){
            strcpy(waiting[i].name, waiting[i-1].name);
            waiting[i].xp = waiting[i-1].xp;
            strcpy(waiting[i].mission, waiting[i-1].mission);
        }

        waiting[index] = student;


    }
    *waitingFlag = 1;
}