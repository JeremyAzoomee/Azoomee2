#include "StringFunctions.h"
#include "TextInputChecker.h"

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

int ageFromDOBString(std::string dobString)
{
    std::vector<std::string> dobSplit = splitStringToVector(dobString, "-");
    
    if(dobSplit.size() != 3)
        return -1;
    
    int DOBday = std::atoi(dobSplit.at(2).c_str());
    int DOBmonth = std::atoi(dobSplit.at(1).c_str());
    int DOByear = std::atoi(dobSplit.at(0).c_str());
    
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    
    int currentDay = aTime->tm_mday;
    int currentMonth = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
    int currentYear = aTime->tm_year + 1900; // Year is # years since 1900
    
    if(isDate(DOBday,DOBmonth,DOByear) && isDate(currentDay,currentMonth,currentYear))
    {
        int childAge = currentYear - DOByear;
        
        if(DOBmonth > currentMonth || (DOBmonth == currentMonth && DOBday > currentDay))
            childAge--;
            
        return childAge;
    }
    
    return -1;
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
