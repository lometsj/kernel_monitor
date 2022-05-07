#include "sothoth.h"

void sothoth_init(const char * agentId){
    strcpy(AGENT_ID, agentId);
    struct sockaddr_un addr;

    // Create a new client socket with domain: AF_UNIX, type: SOCK_STREAM, protocol: 0
    SOCKET_FD = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("Client socket fd = %d\n", SOCKET_FD);

    // Make sure socket's file descriptor is legit.
    if (SOCKET_FD == -1) {
      printf("socket error");
      return;
    }

    //
    // Construct server address, and make the connection.
    //
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);

    // Connects the active socket referred to be sfd to the listening socket
    // whose address is specified by addr.
    if (connect(SOCKET_FD, (struct sockaddr *) &addr,
                sizeof(struct sockaddr_un)) == -1) {
      printf("connect error");
      return;
    }
}

void sothoth_emit(cJSON *data){
     cJSON * msg = cJSON_CreateArray();
     cJSON * agentIdJson = cJSON_CreateString(AGENT_ID);
     cJSON_AddItemToArray(msg, agentIdJson);
     cJSON_AddItemToArray(msg, data);
     char * pp = cJSON_PrintUnformatted(msg);
     int len = strlen(pp);
     write(SOCKET_FD, pp, len);
     write(SOCKET_FD, "\n", 1);
}


void sothoth_close(){

}
