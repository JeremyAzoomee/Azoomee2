#include "StringFunctions.h"
#include <cstdlib>

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
    
bool isDateStringOlderThanToday(std::string dateToCheck)
{
    std::vector<std::string> dobSplit = splitStringToVector(dateToCheck, "-");
    
    if(dobSplit.size() != 3)
        return false;
    
    int nextBilldate = std::atoi(dobSplit.at(0).c_str()) * 10000 + std::atoi(dobSplit.at(1).c_str()) * 100 + std::atoi(dobSplit.at(2).c_str());
    
    time_t now = time (0);
    tm nowtm = *(gmtime (&now));
    
    int today = nowtm.tm_year * 10000 +19000000 + (nowtm.tm_mon + 1) * 100 + nowtm.tm_mday;
    
    if(nextBilldate <=today)
        return true;
    else
        return false;
}


} // Azoomee
