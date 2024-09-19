#include <iostream>
#include <string>
#include <json.h>

using nlohmann::json;

std::string GetRequestRoute(std::string* request);
int GetRequestType(std::string* request);
int FindSubstringLocation(std::string* str,std::string toFind,int shiftBy = 0);
json GetHeaders(std::string* request);
json GetQuery(std::string *url);
std::string GetApiVersionRequest(std::string* requestRoute);