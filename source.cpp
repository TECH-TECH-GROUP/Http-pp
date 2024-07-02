#include <stdio.h>
#include "headers/json.h"
#include "headers/connection.h"

// use this file for testing
short HandlePost(int clientSocket, json requestJSON,std::string requestRoute,json headers);
short HandleGet(int clientSocket, std::string requestRoute,json headers);

int main(){
    Server::Listen(1313,HandlePost,HandleGet);
    return 0;
}

short HandlePost(int clientSocket, json requestJSON,std::string requestRoute,json headers){
    // Send back a response
    json responseJSON = {};
    printf("%s\n",headers.dump().c_str());
    printf("%s\n",requestJSON.dump().c_str());

    json responseHeaders = {};
    responseHeaders["X-TOKEN"] = "012Sd44ASz%9zzx7713";
    responseHeaders["Concentrated"] = "true";
    Response::RespondJSON(clientSocket,0,responseJSON,"",responseHeaders);

    return 0;
}

short HandleGet(int clientSocket, std::string requestRoute,json headers){
    // This is an example implementation of how you can handle a GET request.
    
    json responseJSON = {};
    json responseHeaders = {};
    responseHeaders["X-TOKEN"] = "012Sd44ASz%9zzx7713";
    responseHeaders["Concentrated"] = "true";

    Response::RespondJSON(clientSocket,0,responseJSON,"",responseHeaders);

    return 0;
}