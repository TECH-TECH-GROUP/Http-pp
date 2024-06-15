#include "json.h"
#include <string>

using nlohmann::json;

namespace Server{
    void Listen(short (*HandlePost)(json requestJSON),short (*HandleGet)(json requestJSON));
    // Route, Type, Need to pass 
    short HandleRequest(int clientSocket,short (*HandlePost)(json requestJSON),short (*HandleGet)(json requestJSON));
}

namespace Response{
    // Type= 0 ok (200), Type= 1 fail (400), Type= 2 fail(500), Type= 3 custom (custom)
    short RespondJSON(short type,json response);

    const std::string responseJSON = "";
}