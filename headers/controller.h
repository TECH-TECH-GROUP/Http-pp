#include <database.h>
#include <json.h>
#include <string>

using nlohmann::json;

short HandlePost(int clientSocket, json requestJSON,std::string requestRoute,json headers,std::string apiVersion,pqxx::connection* dbConn);
short HandleGet(int clientSocket, std::string requestRoute,json query,json headers,std::string apiVersion,pqxx::connection* dbConn);
short ResolveRoute(std::string &requestRoute,std::string &apiVersion);

namespace Routes{
    enum{
        EXAMPLE
    };
}