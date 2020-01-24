#include "SpecialCalendarEventManager.h"
#include "TimeFunctions.h"
#include "StringFunctions.h"

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

CalenderSeasons SpecialCalendarEventManager::getSeasonFromString(const std::string &seasonString)
{
    const std::string& stringLower = StringFunctions::stringToLower(seasonString);
    if(stringLower == "easter")
    {
        return EASTER;
    }
    if(stringLower == "halloween")
    {
        return HALLOWEEN;
    }
    if(stringLower == "xmas")
    {
        return XMAS;
    }
    return ANY;
}

bool SpecialCalendarEventManager::checkIfInSeason(CalenderSeasons season)
{
    switch (season) {
        case EASTER:
            return isEasterTime();
            break;
        case HALLOWEEN:
            return isHallowenTime();
            break;
        case XMAS:
            return isXmasTime();
            break;
        case ANY:
        default:
            return true;
            break;
    }
}

bool SpecialCalendarEventManager::isXmasTime()
{
#ifdef FORCE_XMAS
    return true;
#endif
    long today = TimeFunctions::getEpochTimeForTodayWithoutYear();
    long xmasDecStart = TimeFunctions::getEpochTimeForDateWithoutYear(11, 22);
    long xmasDecEnd = TimeFunctions::getEpochTimeForDateWithoutYear(12, 31);
    long xmasJanStart = TimeFunctions::getEpochTimeForDateWithoutYear(1, 1);
    long xmasJanEnd = TimeFunctions::getEpochTimeForDateWithoutYear(1, 31);
    
    return (today >= xmasDecStart && (today <= xmasDecEnd)) || (today >= xmasJanStart && today <= xmasJanEnd);
}

bool SpecialCalendarEventManager::isEasterTime()
{
#ifdef FORCE_EASTER
    return true;
#endif
    return true;
}

bool SpecialCalendarEventManager::isHallowenTime()
{
#ifdef FORCE_HALLOWEEN
    return true;
#endif
    long today = TimeFunctions::getEpochTimeForTodayWithoutYear();
    long halloweenStart = TimeFunctions::getEpochTimeForDateWithoutYear(10, 1);
    long hallowenEnd = TimeFunctions::getEpochTimeForDateWithoutYear(11, 22);
    
    return (today >= halloweenStart && (today <= hallowenEnd));
}


NS_AZOOMEE_END
