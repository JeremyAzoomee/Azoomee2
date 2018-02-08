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
std::string getJSONStringFromVectorOfMaps(std::vector<std::map<std::string, std::string>> inputMap);
std::string getJSONStringFromMap(std::map<std::string, std::string> inputMap);
std::string replaceAll(std::string& str, const std::string& from, const std::string& to);
std::string getValueFromHttpResponseHeaderForKey(const std::string &key, const std::string &responseHeaderString);
std::string& trim(std::string& string);
std::string shortenString(const std::string& string, int maxLength);
std::string joinStrings(const std::vector<std::string>& stringList, const std::string& divider);
    
/// Trim whitespace from the start of string
std::string ltrim(const std::string& s);
/// Trim whitespace from the end of string
std::string rtrim(const std::string& s);
/// Trim whitespace from the start and end of string
std::string trim(const std::string& s);
  
} // Azoomee

#endif

