#include "../headers/connection.h"
#include "../headers/utility.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <thread>

#define BUFFER_SIZE 100000
#define BACKLOG 1313
#define PORT 1313

#define USE_CORS true


void Server::Listen(short (*HandlePost)(json requestJSON,std::string requestRoute),short (*HandleGet)(json requestJSON,std::string requestRoute)){

 // Create the listener
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);
    if(serverSocket <0){
        printf("[+] Server socket failed to initialize...\n");
        return;
    }


    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT); 

    if(bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr))!=0){
        printf("[+] Failed binding the server socket...\n");
        return;
    }

    if(listen(serverSocket,BACKLOG) != 0){
        printf("[+] Failed listening...\n");
        return;
    }

    printf("[-] HTTP++ online [%d]\n",PORT);

    while (true){
        sockaddr_in connectingAddress;
        socklen_t clientAddressLen = sizeof(connectingAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&connectingAddress, &clientAddressLen);
        if(clientSocket == -1){
            printf("[+] Accepting client failed...\n");
        }
        std::thread t(HandleRequest,clientSocket,HandlePost,HandleGet);
        t.detach();
    }
    
    return;
}

short Server::HandleRequest(int clientSocket,short (*HandlePost)(json requestJSON,std::string requestRoute),short (*HandleGet)(json requestJSON,std::string requestRoute)){
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
        HandlePost(requestJSON,requestRoute);
        break;
    case 1:
        HandleGet(requestJSON,requestRoute);
        break;
    
    default:
        break;
    }
    return 0;
}


short Response::RespondJSON(short type,json response){

    return 0;
}

