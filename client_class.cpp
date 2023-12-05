#include "common_header.h"
#include "client_header.h"


std::string client::convertToString(char* a, int size)
{
	    int i;
	    string s = "";
	    for (i = 0; i < size; i++) {
	        s = s + a[i];
	    }
	    return s;
}

vector<string> client::split (string s, string delimiter) {
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


void client::listen_loop(int sockfd) {

    message req;
    message res;
    
    while(1) {

        socklen_t len;
        int message_size;
        struct sockaddr_in cliaddr;

        len = sizeof(cliaddr);
        

	len = sizeof(cliaddr);  
	memset(&req, 0, sizeof(req));
	int n = recvfrom(sockfd, (message*) &req, sizeof(req), MSG_WAITALL, (struct sockaddr*) &cliaddr, &len);
	
	
	request r;
        r.type = 3;
	string que = convertToString(inet_ntoa(cliaddr.sin_addr), strlen(inet_ntoa(cliaddr.sin_addr)))+":"+to_string(ntohs(cliaddr.sin_port));
	strcpy(r.id, que.c_str());
	response ress = query_dns(r);
	
	
	
	switch (req.type) {
		case 1 : {
		
		       cout << "\nQuery message received from " <<ress.ip <<" as : "<<req.n.a<<" and "<<req.n.b<<".\n";
			memset(&res, 0, sizeof(res));
			res.type = 2;
			res.size=(sizeof(sum));
			res.s.result =  req.n.a + req.n.b;
			
			sendto(sockfd, &res, sizeof(res), MSG_CONFIRM, (const struct sockaddr*) &cliaddr, len);
			
			break;
		}
		case 2 : {
			cout << "\nResult message received from " <<ress.ip <<".\n";
			cout <<"Result is "<< req.s.result<<"\n";
			break;
		}
		default:{
			cout <<"nhi"<<"\n";
		}
	}
	//cout <<req.type <<" "<<req.size<<" "<<req.n.a<<"\n";
	
	res.type = 2;
	
        
    }

}

response client::query_dns(struct request m)
{
	int sockfd;
	struct sockaddr_in servaddr;
	char buffer[MAX_LOAD];


	//Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed.");
		exit(EXIT_FAILURE);
	}

	//Filling server information
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);// converts char* to unsigned int


	sendto(sockfd, &m, sizeof(m), MSG_CONFIRM, (const struct sockaddr*) &servaddr, sizeof(servaddr));

	response q;
	socklen_t len = sizeof(servaddr);  
	memset(&q, 0, sizeof(q));
	int n = recvfrom(sockfd, (response*) &q, sizeof(q), MSG_WAITALL, (struct sockaddr*) &servaddr, &len);

	close(sockfd);

	return q;
}


void client::post_loop(int sockfd) {

	
    message m;
    
    while (1) {
        string input;

        getline(cin, input);

        vector <string> v = split(input," ");
        
        
        request r;
        r.type = 2;
	strcpy(r.id, v[0].c_str());
	response q =  query_dns(r);

	if (q.status == 0) {
		cout << "Invalid user\n";
	} else {
		struct sockaddr_in	 servaddr;
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(q.port);
		servaddr.sin_addr.s_addr = inet_addr(q.ip);
		
		//take input
		
		 memset(&m, 0, sizeof(m));
		m.type =1;
		m.size = sizeof(struct num);
		m.n.a = stoi(v[1]);
		m.n.b = stoi(v[2]); 

		sendto(sockfd, &m, sizeof(m), MSG_CONFIRM, (const struct sockaddr*) &servaddr, sizeof(servaddr));
	}
        
 

	
        
    }

}


int client::register_user(struct request m)
{
		int sockfd;
		struct sockaddr_in servaddr;
		char buffer[MAX_LOAD];


		//Creating socket file descriptor
		if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("socket creation failed.");
			exit(EXIT_FAILURE);
		}

		//Filling server information
		memset(&servaddr, 0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(SERVER_PORT);
		servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);// converts char* to unsigned int


		sendto(sockfd, &m, sizeof(m), MSG_CONFIRM, (const struct sockaddr*) &servaddr, sizeof(servaddr));

		response q;
		socklen_t len = sizeof(servaddr);  
		memset(&q, 0, sizeof(q));
		int n = recvfrom(sockfd, (response*) &q, sizeof(q), MSG_WAITALL, (struct sockaddr*) &servaddr, &len);
		cout << q.ip <<"\n\n";

		close(sockfd);

		return q.port;
}



int client::start_server(int port) {

	int sockfd;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) ;

    return sockfd;

}
