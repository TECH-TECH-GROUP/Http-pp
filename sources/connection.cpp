#include "../headers/connection.h"
#include "../headers/utility.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <thread>

#define BUFFER_SIZE 100000

void Server::Listen(short (*HandlePost)(json requestJSON),short (*HandleGet)(json requestJSON)){

}

short Server::HandleRequest(int clientSocket,short (*HandlePost)(json requestJSON),short (*HandleGet)(json requestJSON)){
    char* buffer = new char[BUFFER_SIZE];
    std::string request = "";
    while(true){
        recv(clientSocket,buffer,BUFFER_SIZE,0);
        request+=buffer;

        if(request.find("\r\n\r\n")!=std::string::npos) break;

        // Possible issue here? Infinite loop without the \r\n\r\n?
    }

    std::fill(buffer, buffer + BUFFER_SIZE, 0);
    delete[] buffer;

    printf("%s\n",request.c_str());

    if(request.size() == 0){
        printf("[-] Client sent an empty request...\n");
        close(clientSocket);
        return -1;
    }
    // Get if request is post or get ( and add more later )

    std::string requestRoute = GetRequestRoute(&request);

    json requestJSON;
    try{
       requestJSON = json::parse(request); 
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
    

    
    switch (GetRequestType(&request))
    {
    case 0:
        HandlePost(requestJSON);
        break;
    case 1:
        HandleGet(requestJSON);
        break;
    
    default:
        break;
    }
    return 0;
}




