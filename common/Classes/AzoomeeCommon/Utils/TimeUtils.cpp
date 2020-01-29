//
//  TimeUtils.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 31/01/2018.
//

#include "TimeUtils.h"
#include <locale>
#include <chrono>

NS_AZOOMEE_BEGIN

bool TimeUtils::isDateStringFormatCorrectForComparison(const std::string &dateToCheck)
{
    struct tm inputTimeStruct = {0};
    return strptime(dateToCheck.c_str(), "%Y-%m-%d", &inputTimeStruct);
}

bool TimeUtils::isDateStringOlderThanToday(const std::string &dateToCheck)
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

int TimeUtils::birthYearFromAge(int age)
{
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    return 1900 + time.tm_year - age;
}

long TimeUtils::getEpochTimeForTodayWithoutYear()
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

long TimeUtils::getEpochTimeForDateWithoutYear(int month, int day)
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

std::string TimeUtils::getTimeStringForFileName()
{
    auto t = std::time(nullptr);
    
    std::ostringstream oss;
    oss << t;
    
    return oss.str();
}

std::string TimeUtils::getMillisecondTimestampString()
{
    auto millis = getCurrentTimeMillis().count();
    std::stringstream ss;
    ss << millis;
    return ss.str();
}

std::chrono::milliseconds TimeUtils::getCurrentTimeMillis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}


NS_AZOOMEE_END
