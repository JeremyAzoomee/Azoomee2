#ifndef AzoomeeCommon_StringFunctions_h
#define AzoomeeCommon_StringFunctions_h

#include <cocos/cocos2d.h>
#include <string>
#include <vector>


namespace Azoomee
{

std::vector<std::string> splitStringToVector(std::string inputString, std::string separator);
std::string stringToLower(std::string input);
std::string stringToUpper(std::string input);
std::string stringReplace(std::string originalString, std::string stringToReplace, std::string withNewString);
bool isDateStringOlderThanToday(std::string dateToCheck);
std::string getJSONStringFromVectorOfMaps(std::vector<std::map<std::string, std::string>> inputMap);

  
} // Azoomee

#endif

