#include "json.h"
#include <string>

using nlohmann::json;

namespace Server{
    void Listen(int port,short (*HandlePost)(int clientSocket, json requestJSON,std::string requestRoute),short (*HandleGet)(int clientSocket, std::string requestRoute),int backlog = 1313);
    // Route, Type, Need to pass 
    short HandleRequest(int clientSocket,short (*HandlePost)(int clientSocket, json requestJSON, std::string requestRoute),short (*HandleGet)(int clientSocket, std::string requestRoute));
}

namespace Response{
    // Type= 0 ok (200), Type= 1 fail (400), Type= 2 fail(500)
    short RespondJSON(int clientSocket,short type,json response);

}