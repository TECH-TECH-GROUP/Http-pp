#include <stdio.h>
#include "headers/json.h"
#include "headers/connection.h"

// use this file as a mock service

short HandlePost(json requestJSON,std::string requestRoute);
short HandleGet(json requestJSON,std::string requestRoute);

int main(){
    Server::Listen(HandlePost,HandleGet);
    return 0;
}

short HandlePost(json requestJSON,std::string requestRoute){
    // This is an example implementation of how you can handle a POST request.

    // Send back a response
    json responseJSON = {};
    Response::RespondJSON(0,responseJSON);

    return 0;
}

short HandleGet(json requestJSON,std::string requestRoute){
    // This is an example implementation of how you can handle a GET request.

    return 0;
}