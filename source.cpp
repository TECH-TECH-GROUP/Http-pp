#include <stdio.h>
#include "headers/json.h"
#include "headers/connection.h"

// use this file for testing
short HandlePost(int clientSocket, json requestJSON,std::string requestRoute,json headers,std::string apiVersion);
short HandleGet(int clientSocket, std::string requestRoute,json query,json headers,std::string apiVersion);

int main(){
    Server::Listen(1313,HandlePost,HandleGet);
    return 0;
}

short HandlePost(int clientSocket, json requestJSON,std::string requestRoute,json headers,std::string apiVersion){
    json responseJSON = {};
    Response::RespondJSON(clientSocket,0,responseJSON);

    return 0;
}

short HandleGet(int clientSocket, std::string requestRoute,json query,json headers,std::string apiVersion){
    json responseJSON = {};
    Response::RespondJSON(clientSocket,0,responseJSON);

    return 0;
}