//
//  TimeFunctions.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 31/01/2018.
//

#include "TimeFunctions.h"
#include <locale>

NS_AZOOMEE_BEGIN

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

int birthYearFromAge(int age)
{
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    return 1900 + time.tm_year - age;
}

long getEpochTimeForTodayWithoutYear()
{
    time_t epochTimeForNow = std::time(NULL);
    struct tm currentTimeStruct = *gmtime(&epochTimeForNow);
    
    currentTimeStruct.tm_year = 70;
    currentTimeStruct.tm_hour = 12;
    currentTimeStruct.tm_min = 0;
    currentTimeStruct.tm_sec = 0;
    currentTimeStruct.tm_isdst = -1;    //setting time to midday, and turning off daylight saving, as we are curious about the day only
    
    return mktime(&currentTimeStruct);
}

long getEpochTimeForDateWithoutYear(int month, int day)
{
    struct tm currentTimeStruct;
    
    currentTimeStruct.tm_year = 70;
    currentTimeStruct.tm_mon = month - 1;
    currentTimeStruct.tm_mday = day;
    currentTimeStruct.tm_hour = 12;
    currentTimeStruct.tm_min = 0;
    currentTimeStruct.tm_sec = 0;
    currentTimeStruct.tm_isdst = -1;    //setting time to midday, and turning off daylight saving, as we are curious about the day only
    
    return mktime(&currentTimeStruct);
}

NS_AZOOMEE_END