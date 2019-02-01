#include <stdio.h>
#include <stdint.h>
#include "../headers/num_methods.h"
#include "../headers/html.h"
#include "../headers/struct_student.h"
#include "../headers/constants.h"
#include "../headers/display.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include <pthread.h>
#include <termio.h>
#include <inttypes.h>



#define closesocket(s) close(s)
SOCKET sock;
int loop_list = 0;
int exitProg = 0;
uint16_t port = 0;
volatile int waitingFlag = 1;
student waiting[WAIT_LEN];


/**
 * Thread: Infinite loop displaying the list of students waiting on the screen
 * @param void
 * @return void
 */
void *thread_list(void* arg){
    while(!exitProg) {
        if (loop_list) {
            clrscr();
            while(!waitingFlag){}
            waitingFlag = 0;
            for (int i = 0; i < WAIT_LEN; i++) {
                if (waiting[i].name[0] != '\0') {
                    printf("Student waiting: %s\n"
                           "Mission: %s\n"
                           "XP: %d\n\n", waiting[i].name, waiting[i].mission, waiting[i].xp);
                }
            }
            waitingFlag = 1;
            printf("Press [ENTER] to go back to the main menu\n");
        }
        sleep(1);
    }

    (void) arg;
    return NULL;
}

/**
 * Thread: Web server
 * @param void
 * @return void
 */
void *thread_socket(void* arg){

    //Socket creation
    if(sock == INVALID_SOCKET)
    {
        perror("socket()");
        exit(errno);
    }

    //Prevents adress alreaedy used
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    //Socket config
    {
        SOCKADDR_IN sin = {0};

        sin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */

        sin.sin_family = AF_INET;

        sin.sin_port = htons(port ? port : DEFAULT_PORT);

        if (bind(sock, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR) {
            perror("bind()");
            exit(errno);
        }
    }

    //Listening for max 20 connections
    if(listen(sock, 20) == SOCKET_ERROR)
    {
        perror("listen()");
        exit(errno);
    }
    SOCKADDR_IN csin = { 0 };
    SOCKET csock;

    unsigned int sinsize = sizeof csin;



    while(!exitProg) {
        csock = accept(sock, (SOCKADDR *) &csin, &sinsize);

        if (csock == INVALID_SOCKET) {
            perror("accept()");
            exit(errno);
        }

        char buffer[4096];
        int n = 0;

        if ((n = (int) recv(csock, buffer, sizeof buffer - 1, 0)) < 0) {
            perror("recv()");
            exit(errno);
        }

        buffer[n] = '\0';
        char buffer2[4096];
        strcpy(buffer2, buffer);
        char *ptr;
        const char *s = " ";
        /* get the first token */
        ptr = strtok(buffer, s);
        /* walk through other tokens */


        if(ptr==NULL) {
            close(csock);
            return NULL;
        }

        if (strcmp(ptr, "GET") == 0) {
            ptr = strtok(NULL, s);

            if (strcmp(ptr, "/") == 0)
                ptr = "/index.gweb";

            char page[128] = "/pages";

            strcat(page, ptr);
            handleGET(&csock, page);

        }
        else if (strcmp(ptr, "POST") == 0) {
            ptr = strtok(NULL, s);
            if(strcmp(ptr, "/valid.gweb")==0){
                char *ptr2;
                const char *s2 = "\r\n";
                ptr2 = strtok(buffer2, s2);
                int mustStop = 1;
                student std;
                while(ptr2 != NULL && mustStop){
                    if(!strstr(ptr2, ":")&&!strstr(ptr2, "/")){
                        mustStop = 0;
                    }

                    else
                        ptr2 = strtok(NULL, s2);
                }

                char name[31];
                char mission[5];
                char xp[10];

                const char *s3 = "&";


                ptr2 = strtok(ptr2, s3);
                int i=5;
                for(;i<strlen(ptr2);i++){
                    name[i-5] = ptr2[i];
                }
                name[i-5]='\0';
                ptr2 = strtok(NULL, s3);
                for(i=8; i<strlen(ptr2);i++){
                    mission[i-8] = ptr2[i];
                }
                mission[i-8]='\0';
                ptr2 = strtok(NULL, s3);
                for(i=3; i<strlen(ptr2);i++){
                    xp[i-3] = ptr2[i];
                }
                xp[i-3]='\0';




                strcpy(std.name, name);
                strcpy(std.mission, mission);
                std.xp = (int) strtoumax(xp, NULL, 10);
                addStudent(waiting, &waitingFlag, std);

            }

            handlePOST(&csock);
        }

        closesocket(csock);
    }

    (void) arg;
    return NULL;
}


#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc32-c"
#pragma ide diagnostic ignored "cert-msc30-c"
int main(int argc, char *argv[]){
    for(int i=1; i<argc; i++){
        if(!strcmp(argv[i], "-debug")) {

            srand((unsigned int) time(NULL));
            for(int i2=1; i2<=5; i2++){
                student s;
                int xp = rand()%150;
                strcpy(s.mission, itoa(xp, 10));
                strcpy(s.name, itoa(i2, 10));
                s.xp = xp;
                addStudent(waiting, &waitingFlag, s);
            }
        }
        else if(!strcmp(argv[i], "-port")){
            if(i<argc-1){
                port = (uint16_t) strtoumax(argv[i + 1], NULL, 10);
            }
        }
    }
    setbuf(stdout, NULL);
    pthread_t threadsocket;
    pthread_t threadlist;


    clrscr();
    printf("Hello, World!\n"
           "This program have been designed by Mr. LOTTIN Gwendal, IE4-I91\nStarting in 3 secs\n");
    sleep(3);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (pthread_create(&threadsocket, NULL, thread_socket, NULL)) {

        perror("pthread_create");

        return EXIT_FAILURE;

    }

    if (pthread_create(&threadlist, NULL, thread_list, NULL)) {

        perror("pthread_create");

        return EXIT_FAILURE;

    }
    while(!exitProg) {
        printMenu(&loop_list, &exitProg, port, waiting, &waitingFlag);
        if(loop_list) {
            mygetch();
            loop_list = 0;
        }
    }


    closesocket(sock);
    return 0;
}
#pragma clang diagnostic pop



