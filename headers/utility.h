#include <iostream>
#include <string>

std::string GetRequestRoute(std::string* request);
int GetRequestType(std::string* request);
int FindSubstringLocation(std::string* str,std::string toFind,int shiftBy = 0);