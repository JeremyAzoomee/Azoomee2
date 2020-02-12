//
//  TimeUtils.h
//  AzoomeeCommon
//
//  Created by Macauley on 31/01/2018.
//

#ifndef TimeUtils_h
#define TimeUtils_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>
#include <time.h>
#include <chrono>

NS_AZOOMEE_BEGIN

class TimeUtils
{
public:
    
    static bool isDateStringOlderThanToday(const std::string &dateToCheck);
    static bool isDateStringFormatCorrectForComparison(const std::string &dateToCheck);
    static int birthYearFromAge(int age);
    static long getEpochTimeForTodayWithoutYear();
    static long getEpochTimeForDateWithoutYear(int month, int day);
    static std::string getTimeStringForFileName();
    static std::string getMillisecondTimestampString();
    static std::chrono::milliseconds getCurrentTimeMillis();
    
};

NS_AZOOMEE_END

#endif /* TimeUtils_h */