#include "StringFunctions.h"
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <cctype>

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
        const std::string &token = inputString.substr(prev, pos - prev);
        result.push_back(token);
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
            return ltrim(splitStringToVector(currentKey, ":").back());
        }
    }
    
    return "";
}
    
std::string shortenString(const std::string& string, int maxLength)
{
    if(string.length() <= maxLength)
    {
        return string;
    }
    
    return string.substr(0, maxLength) + "...";
}

std::string joinStrings(const std::vector<std::string>& stringList, const std::string& divider)
{
    std::string listString = "";
    for (std::vector<std::string>::const_iterator strListItt = stringList.begin(); strListItt != stringList.end(); ++strListItt) {
        listString += *strListItt;
        if (strListItt != stringList.end() - 1)
        {
            listString += divider;
        }
    }
    return listString;
}
    
std::string convertStringToAssetSafeString(const std::string& string)
{
    std::string lowerCase = stringToLower(string);
    return replaceAll(lowerCase, " ", "_");
}
    
std::string convertToHQNameString(const std::string& string)
    {
        return stringToUpper(string) + " HQ";
    }
    
bool stringEndsWith(const std::string &strToTest, const std::string &endStr)
{
    return strToTest.size() >= endStr.size() && strToTest.compare(strToTest.size() - endStr.size(), endStr.size(), endStr) == 0;
}
    
bool stringToBool(const std::string& string)
{
    return stringToLower(string) == "true";
}

std::string boolToString(bool boolean)
{
    return boolean ? "true" : "false";
}
	
int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne)
{
	int startSearchPos = 0;
	
	for(int i = 0; i < whichOne; i++)
	{
		if(string.find(whatToFind, startSearchPos) == string.npos)
		{
			return int(string.length());
		}
		else
		{
			startSearchPos = int(string.find(whatToFind, startSearchPos) + 1);
		}
	}
	
	return startSearchPos - 1;
}

} // Azoomee
