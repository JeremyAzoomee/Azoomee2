#ifndef AzoomeeCommon_StringFunctions_h
#define AzoomeeCommon_StringFunctions_h

#include <string>
#include <vector>


namespace Azoomee
{

std::vector<std::string> splitStringToVector(std::string inputString, std::string separator);
std::string stringToLower(std::string input);
std::string stringToUpper(std::string input);
std::string stringReplace(std::string originalString, std::string stringToReplace, std::string withNewString);

  
} // Azoomee

#endif
