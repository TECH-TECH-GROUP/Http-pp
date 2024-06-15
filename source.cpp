#include <stdio.h>
#include "headers/json.h"
#include "headers/connection.h"

// use this file as a mock service

short HandlePost(json requestJSON);
short HandleGet(json requestJSON);

int main(){
    Server::Listen(HandlePost,HandleGet);
    return 0;
}

short HandlePost(json requestJSON){
    
    return 0;
}

short HandleGet(json requestJSON){
    return 0;
}