#include <database.h>
#include <json.h>


using nlohmann::json;

namespace Validator{
    short ValidateSchema(json &request,const int &clientSocket,const json &schema,pqxx::connection* dbConn);
    template <typename T>
    bool ValidateId(T id,std::string table,pqxx::connection *dbConn);
    bool ValidateAuthHeaders(json &headers,const int &clientSocket);
}

namespace Schemas{
    const json Example = {
        {"string", {
            {"type", "string"},
            {"required", false}
        }},
        {"string_with_enum",{
            {"type","string"},
            {"required",false},
            {"enum",json::array({"admin","client","coach"})}
        }},
        {"number",{
            {"type","number"},
            {"required",false}
        }},
        {"object",{
            {"type","object"},
            {"required",false}
        }},
        {"object",{
            {"type","array"},
            {"required",false}
        }}
    };

}
