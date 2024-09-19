#include <database.h>
#include <json.h>
#include <string>

using nlohmann::json;

namespace Server{
    void Listen(int port,short (*HandlePost)(int clientSocket, json requestJSON,std::string requestRoute,json headers,std::string apiVersion,pqxx::connection* dbConn),short (*HandleGet)(int clientSocket, std::string requestRoute,json query,json headers,std::string apiVersion,pqxx::connection* dbConn),int backlog = 1313);
    // Route, Type, Need to pass 
    short HandleRequest(int clientSocket,short (*HandlePost)(int clientSocket, json requestJSON,std::string requestRoute,json headers,std::string apiVersion,pqxx::connection* dbConn),short (*HandleGet)(int clientSocket, std::string requestRoute,json query,json headers,std::string apiVersion,pqxx::connection* dbConn),pqxx::connection* dbConn);
}

namespace Response{
    short RespondJSON(int clientSocket,short type,json response = json::parse("{}"),std::string customResponseCode="",json headers = json::parse("{}"));
}