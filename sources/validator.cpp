#include <connection.h>
#include <validator.h>
#include <utility.h>

short Validator::ValidateSchema(json &request,const int &clientSocket,const json &schema,pqxx::connection* dbConn){
    for (auto &item : request.items()) {
        if (schema.count(item.key()) == 0) {
            printf("Extra field found: %s\n", item.key().c_str());
            Response::RespondJSON(clientSocket,1,{{"reason","Validation failed. Extra field found '"+item.key()+"'."}});
            return -1;
        }
    }

    for(auto &schemaItem:schema.items()){
        try{
            if(!request.contains(schemaItem.key())){
                if(schemaItem.value()["required"]){
                    printf("Missing non optional field: %s\n",schemaItem.key().c_str());
                    Response::RespondJSON(clientSocket,1,{{"reason","Validation failed. Missing non optional field: '"+schemaItem.key()+"'."}});
                    return -1;
                }
                else{
                    if(schemaItem.value()["type"] == "string")
                        request[schemaItem.key()] = "";
                    if(schemaItem.value()["type"] == "number")
                        request[schemaItem.key()] = 0;
                    if(schemaItem.value()["type"] == "object")
                        request[schemaItem.key()] = json::object();
                    if(schemaItem.value()["type"] == "array")
                        request[schemaItem.key()] = json::array();
                }
            }


            // Check for enforced values like [male|female].
            if(schemaItem.value().contains("enum") && schemaItem.value()["required"].get<bool>()){
                bool found = false;
                for(auto &enumValue:schemaItem.value()["enum"]){
                    if(enumValue.get<std::string>() == request[schemaItem.key()].get<std::string>()) found = true;
                }
                if(!found){
                    Response::RespondJSON(clientSocket,1,{{"reason","Validation failed. Field: '"+schemaItem.key()+"' is not of accepted value."}});
                    return -1;
                }
            }

            // If needed, check for existence.
            if(schemaItem.value().contains("checkId") && schemaItem.value()["checkId"] == true){
                if((std::string)request[schemaItem.key()].type_name() == "string"){
                    if(!Validator::ValidateId(request[schemaItem.key()].get<std::string>(),schemaItem.value()["table"].get<std::string>(),dbConn)){
                        Response::RespondJSON(clientSocket,1,{{"reason","Id does not exist."}});
                        return -1;
                    } 
                }
                else{
                    if(!Validator::ValidateId(request[schemaItem.key()].get<long int>(),schemaItem.value()["table"].get<std::string>(),dbConn)){
                        Response::RespondJSON(clientSocket,1,{{"reason","Id does not exist."}});
                        return -1;
                    }
                }
            }


            if(schemaItem.value()["type"] != std::string(request[schemaItem.key()].type_name())){
                // printf("%s is not %s\n",schemaItem.key().c_str(),schemaItem.value()["type"].get<std::string>().c_str());
                Response::RespondJSON(clientSocket,1,{{"reason","Validation failed. Field '" + schemaItem.key() + "' is not of type '" + schemaItem.value()["type"].get<std::string>() + "'."}});
                return -1;
            }
        }
        catch(const std::exception& e){
            Response::RespondJSON(clientSocket,1,{{"reason","Validation failed."}});
            return -1;
        }
    }
    return 0;
}


template <typename T>
bool Validator::ValidateId(T id,std::string table,pqxx::connection *dbConn){
    try{
        pqxx::work worker(*dbConn);
        std::string dbQuery = "SELECT id FROM " + worker.quote_name(table) + " WHERE id = " + worker.quote(id);
        if(worker.exec(dbQuery).empty()) return false;
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return false;
    }
    
    return true;
}

bool Validator::ValidateAuthHeaders(json &headers,const int &clientSocket){
    if(!headers.contains("userId") || !headers.contains("authKey")){
        Response::RespondJSON(clientSocket,1,{{"reason","Auth headers are required."}});
        return false;
    }
    return true;
}