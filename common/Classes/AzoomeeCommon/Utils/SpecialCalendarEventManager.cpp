#include "SpecialCalendarEventManager.h"
#include "TimeFunctions.h"

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


NS_AZOOMEE_END
