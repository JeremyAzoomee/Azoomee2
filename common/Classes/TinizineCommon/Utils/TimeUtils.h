//
//  TimeUtils.h
//  TinizineCommon
//
//  Created by Macauley on 31/01/2018.
//

#ifndef TimeUtils_h
#define TimeUtils_h

#include "../Tinizine.h"
#include <cocos/cocos2d.h>
#include <time.h>
#include <chrono>

NS_TZ_BEGIN

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

NS_TZ_END

#endif /* TimeUtils_h */
