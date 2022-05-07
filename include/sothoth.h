#ifndef SOTHOTH_H
#define SOTHOTH_H
#include "cJSON.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define SOCK_PATH  "/sothoth/nodeagent.sock"
#define BUF_SIZE 4096

char AGENT_ID[33];
int SOCKET_FD;

void sothoth_init(const char * pluginId);
void sothoth_emit(cJSON *data);
void sothoth_close();

#endif