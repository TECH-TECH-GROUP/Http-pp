#include <sys/socket.h>
#include <connection.h>
#include <arpa/inet.h>
#include <validator.h>
#include <utility.h>
#include <unistd.h>
#include <fstream>
#include <thread>

#define BUFFER_SIZE 100000
#define HTTP_VERSION "1.1"
#define USE_VERSIONING true
#define USE_CORS true

/*Creates a TCP listener.
port-> which port to listen on? 
HandlePost-> handle post function pointer.
HandleGet-> handle get function pointer.
backlog-> Amount of queued connections.*/
void Server::Listen(int port,short (*HandlePost)(int clientSocket, json request,std::string requestRoute,json headers,std::string apiVersion,pqxx::connection* dbConn),short (*HandleGet)(int clientSocket, std::string requestRoute,json query,json headers,std::string apiVersion,pqxx::connection* dbConn),int backlog){

 // Create the listener
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);
    if(serverSocket <0){
        printf("[!] Server socket failed to initialize...\n");
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port); 

    if(bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr))!=0){
        printf("\033[31m[!] Failed binding the server socket...\033[0m\n");
        return;
    }

    if(listen(serverSocket,backlog) != 0){
        printf("[!] Failed listening...\n");
        return;
    }

    /*
        Create a connection to the database and prepare statements
    */
   
    // Database connection
    pqxx::connection* dbConn = Database::ConnectToDatabase();
    if(dbConn == nullptr){
        return;
    }
    if(Database::PrepareStatements(dbConn)!=0){
        printf("[E] Error preparing statements, exiting...\n");
        return;
    }

    printf("\033[32m[OK] HTTP++ online [%d]\033[0m\n",port);

    while (true){
        sockaddr_in connectingAddress;
        socklen_t clientAddressLen = sizeof(connectingAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&connectingAddress, &clientAddressLen);
        if(clientSocket == -1){
            printf("[!] Accepting client failed...\n");
        }
        std::thread t(HandleRequest,clientSocket,HandlePost,HandleGet,dbConn);
        t.detach();
    }
    
    return;
}

short Server::HandleRequest(int clientSocket,short (*HandlePost)(int clientSocket, json request,std::string requestRoute,json headers,std::string apiVersion,pqxx::connection* dbConn),short (*HandleGet)(int clientSocket, std::string requestRoute,json query,json headers,std::string apiVersion,pqxx::connection* dbConn),pqxx::connection* dbConn){
    char* buffer = new char[BUFFER_SIZE];
    std::string request = "";
    while(true){
        recv(clientSocket,buffer,BUFFER_SIZE,0);
        request+=buffer;

        if(request.find("\r\n\r\n")!=std::string::npos) break;
    }

    std::fill(buffer, buffer + BUFFER_SIZE, 0);
    delete[] buffer;

    if(request.size() == 0){
        printf("[-] Client sent an empty request...\n");
        close(clientSocket);
        return -1;
    }

    json headers = GetHeaders(&request);

    // Before doing anything else, check if the API key is valid...
    // if(!headers.contains("Auth")){
    //     json res;
    //     res["reason"] = "No API key header supplied. (`Auth` header is required)";
    //     return Response::RespondJSON(clientSocket,1,res);
    // }

    // if(Validator::ValidateAPIKey(headers["Auth"])!=0){
    //     json res;
    //     res["reason"] = "Validation failed. (Your API key is not valid)";
    //     return Response::RespondJSON(clientSocket,3,res,"401 INVALID API KEY");
    // }

    std::string requestRoute = GetRequestRoute(&request);
    std::string apiVersion = "";
    if(USE_VERSIONING)
        apiVersion = GetApiVersionRequest(&requestRoute);

    // Get request type
    short requestType = GetRequestType(&request);
    switch (requestType){
    case 0:{
        json requestJSON;
        std::string toParse = request.substr(FindSubstringLocation(&request,"\r\n\r\n"));
        
        try{
            requestJSON = json::parse(toParse); 
        }
        catch(const std::exception& e){
            std::cerr<< "Issue with parsing JSON. Issue:\n" << e.what() << '\n';
            Response::RespondJSON(clientSocket,1);
        }
        try{
            HandlePost(clientSocket, requestJSON, requestRoute, headers, apiVersion,dbConn);
        }
        catch(const std::exception& e){
            std::cerr<< "Issue with handling POST request. Issue:\n" << e.what() << '\n';
            return Response::RespondJSON(clientSocket,2);
        }
        break;
    }
        
    case 1:{
        try{
            HandleGet(clientSocket, requestRoute, GetQuery(&requestRoute), headers, apiVersion,dbConn);
        }
        catch(const std::exception& e){
            std::cerr<< "Issue with handling GET request. Issue:\n" << e.what() << '\n';
            return Response::RespondJSON(clientSocket,2);
        }
        break;
    }
    default:
        break;
    }
    close(clientSocket);
    return 0;
}

/* 
### Sends a response to the connecting client. 

`clientSocket` Which socket to respond to?
`type` Type of reponse. [0=200;1=400;2=500;3=custom]
`response` JSON response.
`headers` Custom headers you want to respond with.
`customResponseCode` Response code & header you want to send. ex: `404 NOT FOUND` */
short Response::RespondJSON(int clientSocket,short type,json response,std::string customResponseCode,json headers){

    std::string headersDump = "";
    for (auto& el : headers.items()){
        headersDump += el.key();
        headersDump += ": " + el.value().get<std::string>() + (std::string)"\r\n";
    }

    std::string responseType = "200 OK";
    switch (type)
    {
    case 0:
        response["status"] = 200;
        response["message"] = "Action completed successfully.";
        break;
    case 1:
        responseType = "400 CLIENT ERROR";
        response["status"] = 400;
        break;
    case 2:
        responseType = "500 SERVER ERROR";
        response["status"] = 500;
        break;
    case 3:
        responseType = customResponseCode;
        response["status"] = customResponseCode.substr(0,FindSubstringLocation(&customResponseCode," ")-1);
        break;
    default:
        break;
    }
    std::string responseDump = "HTTP/"+std::string(HTTP_VERSION)+" "+responseType+"\r\n"+headersDump+(USE_CORS?"Access-Control-Allow-Origin: *\r\n":"")+"Content-Length:"+std::to_string(response.dump().size())+"\r\nContent-Type: application/json\r\n\r\n"+response.dump()+"\r\n\r\n";

    if(send(clientSocket,responseDump.c_str(),responseDump.size(),0) == -1){
        printf("[-] Failure sending response\n");
        return -1;
    }

    close(clientSocket);

    return 0;
}