class dns
{
        public:
                static vector<string> split (string s, string delimiter);
                static std::string convertToString(char* a, int size);
                static int start_server();
                static void process_requests(int sockfd);

};