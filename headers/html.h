//
// Created by gwendal on 1/25/19.
//

#ifndef GWENMISSIONSUBMIT_HTML_H
#define GWENMISSIONSUBMIT_HTML_H

#include "num_methods.h"

void handlePOST(SOCKET* connection);
void handleGET(SOCKET* connection, char* page);
char* readFile(char fileName[]);

#endif //GWENMISSIONSUBMIT_HTML_H
