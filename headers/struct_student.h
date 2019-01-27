//
// Created by gwendal on 1/26/19.
//

#ifndef GWENMISSIONSUBMIT_STRUCTS_H
#define GWENMISSIONSUBMIT_STRUCTS_H

typedef struct student student;

struct student{
    char name[32];
    char mission[5];
    int xp;
};




void addStudent(student *waiting, volatile int *waitingFlag, student student);

void removeStudent(student *waiting, int num, volatile int *waitingFlag);


#endif //GWENMISSIONSUBMIT_STRUCTS_H
