#include "common_header.h"
#include "client_header.h"



int main(int argc, char* argv[])
{
    request m;
	memset(&m, 0, sizeof(m));
    m.type = 1;
    strcpy(m.id, argv[1]);
    int port = client::register_user(m);


	int sock_fd = client::start_server(port);

    

    cout << "Server started at port "<< port <<"...\n";
    cout << "Send messages by typing receiver's IP adress and message sepearted by \" \" : "<<"\n";



    std::thread thread_obj1(client::post_loop, sock_fd);
    std::thread thread_obj(client::listen_loop, sock_fd);



    thread_obj.join();
    thread_obj1.join();

    return 0;
}
