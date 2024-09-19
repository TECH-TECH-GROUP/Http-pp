#include <connection.h>
#include <controller.h>
#include <utility.h>


short HandlePost(int clientSocket, json requestJSON,std::string requestRoute,json headers,std::string apiVersion,pqxx::connection* dbConn){
    short resolvedRoute = ResolveRoute(requestRoute,apiVersion);
    json response;

    switch (resolvedRoute){
    
    default:
        return Response::RespondJSON(clientSocket,1,{{"message","Requested route does not exist."}});
    }

    return 0;
}

short HandleGet(int clientSocket, std::string requestRoute,json query,json headers,std::string apiVersion,pqxx::connection* dbConn){
    short resolvedRoute = ResolveRoute(requestRoute,apiVersion);
    json response;
    switch (resolvedRoute){

    default:
        return Response::RespondJSON(clientSocket,1,{{"message","Requested route does not exist."}});
    }
}


/*
### Route resolver
Resolves route requested by the client.
*/
short ResolveRoute(std::string &requestRoute,std::string &apiVersion){

    if(FindSubstringLocation(&requestRoute,"test/example")!=-1 && apiVersion == "v1")
        return 0;
    return -1;
}
