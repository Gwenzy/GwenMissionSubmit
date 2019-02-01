//
// Created by gwendal on 1/25/19.
//

#include <unistd.h>
#include <errno.h>
#include "../headers/html.h"
#include "stdio.h"
#include "stdlib.h"
#include "../headers/num_methods.h"
#include "memory.h"
#include "sys/socket.h"

#define HEADER "HTTP/1.1 200 OK\r\nContent-Type: text/html;charset=utf8\r\nContent-Length: "
#define HEADER_404 "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: "

void handlePOST(SOCKET *connection){
    char response[1024] = HEADER;
    char body[512] = "<html><head></head><body><p>La demande a été effectuée et va être ajoutée à la liste. <a href=\"/\">Cliquez ici</a> pour retourner à l'accueil</p></body></html>";
    strcat(response, itoa((int) strlen(body), 10));
    strcat(response, "\r\n\r\n");
    strcat(response, body);

    if (send(*connection, response, strlen(response), 0) < 0) {
        printf("An error has occured while sending page %s", "PAGE");
        exit(-1);
    }

}


void handleGET(SOCKET *connection, char *page) {


    char prefix[128] = "..";
    char *ptr;
    const char *s = "?";
    ptr = strtok(page, s);
    strcat(prefix, ptr);
    if( access( prefix, F_OK ) != -1 ) {
        char* fileContents = readFile(page);
        char response[4096] = HEADER;
        strcat(response, itoa((int) strlen(fileContents), 10));
        strcat(response, "\r\n\r\n");
        strcat(response, fileContents);


        if (send(*connection, response, strlen(response), 0) < 0) {
            printf("An error has occured while sending page %s", page);
            exit(-1);
        }
    } else {

        char response[512] = HEADER_404;
        char* body = "<html><head><title>404 Not Found</title></head><body><p>404</p></body></html>";

        strcat(response, itoa((int) strlen(body), 10));
        strcat(response, "\r\n\r\n");
        strcat(response, body);

        send(*connection, response, strlen(response), 0);
    }
}



char *readFile(char *fileName) {
    char * buffer = 0;
    long length;
    char prefix[128] = "..";
    strcat(prefix, fileName);
    FILE * f = fopen (prefix, "rb");
    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc((size_t) length);
        if (buffer)
        {
            fread (buffer, 1, (size_t) length, f);
        }
        fclose (f);
    }


    return buffer;
}


