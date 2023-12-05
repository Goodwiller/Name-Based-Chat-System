#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

using namespace std;

#define SERVER_PORT 8020
#define SERVER_IP "172.16.15.6"
#define MAX_LOAD 1024

struct request {
    int type;
    char id [MAX_LOAD];
};

struct response {
    bool status;
    char ip [MAX_LOAD];
    int port;
};
