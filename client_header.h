struct num {
	int a;
	int b;
};

struct sum {
	int result;
};

struct message {
	int type;
	int size;
	num n;
	sum s;
};

class client
{
        public:
                static string convertToString(char* a, int size);
                static vector<string> split (string s, string delimiter);
                static void listen_loop(int sockfd);
                static response query_dns(struct request m);
                static void post_loop(int sockfd);
                static int register_user(struct request m);
                static int start_server(int port);

};
