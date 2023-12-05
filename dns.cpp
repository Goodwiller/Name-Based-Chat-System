#include "common_header.h"
#include "dns_header.h"


int main () {
    int sockfd = dns::start_server();

    dns::process_requests(sockfd);

}