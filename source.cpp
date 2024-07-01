#include <stdio.h>
#include "headers/json.h"
#include "headers/connection.h"

// use this file as a mock service
short HandlePost(int clientSocket, json requestJSON,std::string requestRoute,json headers);
short HandleGet(int clientSocket, std::string requestRoute,json headers);

int main(){
    Server::Listen(1313,HandlePost,HandleGet);
    return 0;
}

short HandlePost(int clientSocket, json requestJSON,std::string requestRoute,json headers){
    // This is an example implementation of how you can handle a POST request.

    // Send back a response
    json responseJSON = {};
    Response::RespondJSON(0,0,responseJSON);

    return 0;
}

short HandleGet(int clientSocket, std::string requestRoute,json headers){
    // This is an example implementation of how you can handle a GET request.
    json responseJSON = {};
    responseJSON["message"] = "Cokee";
    Response::RespondJSON(clientSocket,0,responseJSON);

    return 0;
}