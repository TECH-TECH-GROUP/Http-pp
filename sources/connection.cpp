#include "../headers/connection.h"
#include "../headers/utility.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <thread>

#define BUFFER_SIZE 100000
#define BACKLOG 1313

#define HTTP_VERSION "1.1"
#define USE_CORS true


void Server::Listen(int port,short (*HandlePost)(int clientSocket, json requestJSON,std::string requestRoute),short (*HandleGet)(int clientSocket, std::string requestRoute),int backlog){

 // Create the listener
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);
    if(serverSocket <0){
        printf("[+] Server socket failed to initialize...\n");
        return;
    }


    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port); 

    if(bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr))!=0){
        printf("[+] Failed binding the server socket...\n");
        return;
    }

    if(listen(serverSocket,BACKLOG) != 0){
        printf("[+] Failed listening...\n");
        return;
    }

    printf("[-] HTTP++ online [%d]\n",port);

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

short Server::HandleRequest(int clientSocket,short (*HandlePost)(int clientSocket, json requestJSON,std::string requestRoute),short (*HandleGet)(int clientSocket, std::string requestRoute)){
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

    // Get request type
    short requestType = GetRequestType(&request);
    switch (requestType)
    {
    case 0:{
        json requestJSON;

        try{
            requestJSON = json::parse(request); 
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
        }

        HandlePost(clientSocket, requestJSON,requestRoute);
        break;
    }
        
    case 1:
        HandleGet(clientSocket, requestRoute);
        break;
    
    default:
        break;
    }
    return 0;
}


short Response::RespondJSON(int clientSocket,short type,json response){

    std::string responseType = "200 OK";
    switch (type)
    {
    case 0:
        break;
    case 1:
        responseType = "400 CLIENT ERROR";
        break;
    case 2:
        responseType = "500 SERVER ERROR";
        break;
    default:
        break;
    }

    std::string responseDump = "HTTP/"+std::string(HTTP_VERSION)+" "+responseType+(USE_CORS?"\r\nAccess-Control-Allow-Origin: *":"")+"\r\nContent-Length:"+std::to_string(response.dump().size())+"\r\nContent-Type: application/json\r\n\r\n"+response.dump()+"\r\n\r\n";

    if(send(clientSocket,responseDump.c_str(),responseDump.size(),0) == -1){
        printf("[-] Failure sending response\n");
        return -1;
    }

    return 0;
}