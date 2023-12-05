#include "common_header.h"
#include "dns_header.h"


map <string, pair<string, int>> db;


std::string dns::convertToString(char* a, int size)
{
	    int i;
	    string s = "";
	    for (i = 0; i < size; i++) {
	        s = s + a[i];
	    }
	    return s;
}


vector<string> dns::split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


int dns::start_server() {


    int sockfd;
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("socket creation failed.");
            exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));


    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;


    if (bind(sockfd, (const struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
            perror("bind failed.");
            exit(EXIT_FAILURE);
    }

    return sockfd;
}

void dns::process_requests(int sockfd) {

    struct sockaddr_in cliaddr;
    socklen_t len;
    int n;
    string reply;
    request m;
    response q;

    while(1) {

        len = sizeof(cliaddr);  
        memset(&m, 0, sizeof(m));
        n = recvfrom(sockfd, (request*) &m, sizeof(m), MSG_WAITALL, (struct sockaddr*) &cliaddr, &len);

                                
        if (n < 0) {
            perror("Error reading the socket.");
            exit(0);
        }

        switch(m.type) {
            case 1: {

                
                string id = convertToString(m.id, strlen(m.id));
                                        
                                    
                if (db.find(id) == db.end())  {

                    cout << "\nNew Registration for " + id + ".\n";  

                    db[id] = make_pair(inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);

                    reply = "New User " + id + " registered successfully!";

                    memset(&q, 0, sizeof(q));
                    q.status = 1;
                    strcpy(q.ip, reply.c_str());
                    q.port = cliaddr.sin_port;

                    sendto(sockfd, &q, sizeof(q), MSG_CONFIRM, (const struct sockaddr*) &cliaddr, sizeof(cliaddr));
                    
                                       
                } else {

                    cout << "\nUpdation for " + id + ".\n"; 

                    db[id] = make_pair(inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);

                    reply = "Details of User " + id + " Updated Successfully!";
                    
                    memset(&q, 0, sizeof(q));
                    q.status = 1;
                    strcpy(q.ip, reply.c_str());
                    q.port = cliaddr.sin_port;
                    
                    sendto(sockfd, &q, sizeof(q), MSG_CONFIRM, (const struct sockaddr*) &cliaddr, sizeof(cliaddr));

                }

                break;
            }
            case 2: {

                string id = convertToString(m.id, strlen(m.id));
                

                cout << "\nSearch query for " + id + ".\n"; 

                if (db.find(id) == db.end()) {
                        memset(&q, 0, sizeof(q));
                        reply = "Invalid query!!!";
                        q.status = 0;
                        strcpy(q.ip, reply.c_str());
                } else {
                        memset(&q, 0, sizeof(q));
                        q.status = 1;
                        q.port = db[id].second;
                        strcpy(q.ip, (db[id].first).c_str());  
                }

                sendto(sockfd, &q, sizeof(q), MSG_CONFIRM, (const struct sockaddr*) &cliaddr, sizeof(cliaddr));
                
                break;
            }
            case 3: {

                string id = convertToString(m.id, strlen(m.id));
            

                vector<string> splited = split(id, ":");
                string ip = splited[0];
                int port = stoi(splited[1]);

                cout << "\nReverse search query for " + id + ".\n";


                int flag = 0;
                for (auto x : db) {
                    if (x.second.first == ip && x.second.second == port) {
                        memset(&q, 0, sizeof(q));
                        q.status = 1;
                        strcpy(q.ip, (x.first).c_str());
                        flag = 1;
                    }
                } 

                if (flag == 0) {
                    memset(&q, 0, sizeof(q));
                    q.status = 1;
                    strcpy(q.ip, "Sender not registered!!!");
                }


                sendto(sockfd, &q, sizeof(q), MSG_CONFIRM, (const struct sockaddr*) &cliaddr, sizeof(cliaddr));
                
                break;
            }

            default: {

                    cout << "Galat option" <<endl;
                    continue;
            }

        }

    }
}