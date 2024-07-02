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

json GetHeaders(std::string* request){
    json headers = {};
    int start = FindSubstringLocation(request,"\r\n");
    std::string headersString = request->substr(start,request->find("\r\n\r\n", start) - start);
    std::string line = "";

    while (headersString.size()>0){
        size_t rnFind = headersString.find("\r\n");
        if(rnFind)
            line = headersString.substr(0,FindSubstringLocation(&headersString,"\r\n"));

        const std::string key = line.substr(0,FindSubstringLocation(&line,":")-1);
        std::string value = line.substr(FindSubstringLocation(&line,":")+1,FindSubstringLocation(&line,"\r\n"));
        if(rnFind != -1)
            value = value.erase(value.size()-2,2);
        headers[key] = value;

        if(rnFind == -1)
            break;
        headersString = headersString.substr(FindSubstringLocation(&headersString,"\r\n"));
    }
    
    return headers;
    
}