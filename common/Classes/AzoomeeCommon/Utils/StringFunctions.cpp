#include "StringFunctions.h"
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

namespace Azoomee
{

std::vector<std::string> splitStringToVector(std::string inputString, std::string separator)
{
    std::vector<std::string> result;
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = inputString.find(separator, prev);
        if (pos == std::string::npos) pos = inputString.length();
        std::string token = inputString.substr(prev, pos - prev);
        if (!token.empty()) result.push_back(token);
        prev = pos + separator.length();
    }
    while (pos < inputString.length() && prev < inputString.length());
    
    return result;
}

std::string stringToLower(std::string input)
{
    for(int i = 0; i < input.length(); i++)
    {
        input[i] = tolower(input[i]);
    }
    
    return input;
}

std::string stringToUpper(std::string input)
{
    for(int i = 0; i < input.length(); i++)
    {
        input[i] = toupper(input[i]);
    }
    
    return input;
}
  
std::string stringReplace(std::string originalString, std::string stringToReplace, std::string withNewString)
{
  std::string::size_type positionInString = originalString.find(stringToReplace);
  long lengthOfString = stringToReplace.length();
  if ( positionInString != std::string::npos ) {
    originalString.replace( positionInString, lengthOfString, withNewString);
  }
  return originalString;
}

bool isDateStringFormatCorrectForComparison(const std::string &dateToCheck)
{
    struct tm inputTimeStruct = {0};
    return strptime(dateToCheck.c_str(), "%Y-%m-%d", &inputTimeStruct);
}

bool isDateStringOlderThanToday(const std::string &dateToCheck)
{
    time_t epochTimeForNow = std::time(NULL);
    struct tm currentTimeStruct = *gmtime(&epochTimeForNow);
    
    currentTimeStruct.tm_hour = 0;
    currentTimeStruct.tm_min = 0;
    currentTimeStruct.tm_sec = 0;
    currentTimeStruct.tm_isdst = -1;    //setting time to midnight, and turning off daylight saving, as we are curious about the day only
    
    long epochTimeForToday = mktime(&currentTimeStruct);
    
    struct tm inputTimeStruct = {0};
    if(!strptime(dateToCheck.c_str(), "%Y-%m-%d", &inputTimeStruct))
    {
        return true;                    //if input string cannot be converted, we return true.
    }
    else
    {
        inputTimeStruct.tm_isdst = -1;
        long epochTimeForInputDate = mktime(&inputTimeStruct);
        
        return (epochTimeForInputDate < epochTimeForToday);
    }
}

std::string getJSONStringFromVectorOfMaps(std::vector<std::map<std::string, std::string>> inputMapVector)
{
    std::string returnString = "{ \"Elements\" : [";
    
    for(int i = 0; i < inputMapVector.size(); i++)
    {
        if(i != 0) returnString += ", ";
        returnString += "{";
        
        std::map<std::string, std::string> inputMap = inputMapVector.at(i);
        
        int mapCounter = 0;
        for(auto kv : inputMap)
        {
            if(mapCounter != 0) returnString += ", ";
            returnString += "\"" + kv.first + "\" : ";
            returnString += "\"" + kv.second + "\"";
            mapCounter++;
        }
        
        returnString += "}";
    }
    
    returnString += "]}";
    
    return returnString;
}
    
std::string getJSONStringFromMap(std::map<std::string, std::string> inputMap)
{
    std::string returnString = "{";
    
    int mapCounter = 0;
    for(auto kv : inputMap)
    {
        if(mapCounter != 0) returnString += ", ";
        returnString += "\"" + kv.first + "\" : ";
        returnString += "\"" + kv.second + "\"";
        mapCounter++;
    }
    
    returnString += "}";
    return returnString;
}

std::string ltrim(const std::string& str)
{
    std::string s = str;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

std::string rtrim(const std::string& str)
{
    std::string s = str;
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

std::string trim(const std::string& str)
{
    std::string s = str;
    // Note we don't call ltrim and rtrim, so we can avoid an extra copy of str
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}
    
std::string replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return "";
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return str;
}
    
std::string getValueFromHttpResponseHeaderForKey(const std::string &key, const std::string &responseHeaderString)
{
    const std::vector<std::string>& responseHeaderVector = splitStringToVector(responseHeaderString, "\n");
    for(const std::string &currentKey : responseHeaderVector)
    {
        if(currentKey.compare(0, key.length(), key) == 0)
        {
            return trim(splitStringToVector(currentKey, ":").back());
        }
    }
    
    return "";
}
    
std::string& trim(std::string& string)
{
    const char* t = " \t\n\r\f\v";
    string.erase(0, string.find_first_not_of(t));
    string.erase(string.find_last_not_of(t) + 1);
    return string;
}

} // Azoomee
