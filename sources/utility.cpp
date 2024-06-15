#include "../headers/utility.h"

std::string GetRequestRoute(std::string* request){
    int firstSpace = FindSubstringLocation(request," ");
    std::string route = request->substr(FindSubstringLocation(request,"/"),FindSubstringLocation(request," ",firstSpace+1));
    route = route.substr(0,FindSubstringLocation(&route," "));

    route.pop_back();
    return route;
}

int GetRequestType(std::string* request){

    std::string requestType = request->substr(0,FindSubstringLocation(request," "));

    // Remove the space character from the back
    requestType.pop_back();

    if(requestType == "POST") return 0;
    if(requestType == "GET") return 1;
    if(requestType == "PUT") return 2;
    if(requestType == "DEL") return 3;

    return -1;
}

int FindSubstringLocation(std::string* str,std::string toFind,int shiftBy){
    int foundLoc = str->find(toFind,shiftBy);
    if(foundLoc == std::string::npos)
        return -1;
    return foundLoc + toFind.size();
}