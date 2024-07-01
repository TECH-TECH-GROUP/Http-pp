#include <iostream>
#include <string>

#include "../headers/json.h"

using nlohmann::json;

std::string GetRequestRoute(std::string* request);
int GetRequestType(std::string* request);
int FindSubstringLocation(std::string* str,std::string toFind,int shiftBy = 0);
json GetHeaders(std::string* request);