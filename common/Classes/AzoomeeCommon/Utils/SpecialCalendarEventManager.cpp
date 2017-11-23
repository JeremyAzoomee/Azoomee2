#include "SpecialCalendarEventManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<SpecialCalendarEventManager> _sharedSpecialCalendarEventManager;

SpecialCalendarEventManager* SpecialCalendarEventManager::getInstance()
{
    if (! _sharedSpecialCalendarEventManager.get())
    {
        _sharedSpecialCalendarEventManager.reset(new SpecialCalendarEventManager);
    }
    
    return _sharedSpecialCalendarEventManager.get();
}

SpecialCalendarEventManager::~SpecialCalendarEventManager()
{
}

SpecialCalendarEventManager::SpecialCalendarEventManager()
{
}

bool SpecialCalendarEventManager::isXmasTime()
{
    long today = getEpochTimeForTodayWithoutYear();
    long xmasDecStart = getEpochTimeForDateWithoutYear(11, 22);
    long xmasDecEnd = getEpochTimeForDateWithoutYear(12, 31);
    long xmasJanStart = getEpochTimeForDateWithoutYear(1, 1);
    long xmasJanEnd = getEpochTimeForDateWithoutYear(1, 31);
    
    return (today >= xmasDecStart && (today <= xmasDecEnd)) || (today >= xmasJanStart && today <= xmasJanEnd);
}

long SpecialCalendarEventManager::getEpochTimeForTodayWithoutYear()
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

long SpecialCalendarEventManager::getEpochTimeForDateWithoutYear(int month, int day)
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
