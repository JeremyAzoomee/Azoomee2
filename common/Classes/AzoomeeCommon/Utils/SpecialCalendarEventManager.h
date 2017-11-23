#ifndef __SPECIALCALENDAREVENTMANAGER_H__
#define __SPECIALCALENDAREVENTMANAGER_H__

#include "cocos2d.h"
#include "../Azoomee.h"

NS_AZOOMEE_BEGIN

class SpecialCalendarEventManager

{
    
public:
    static SpecialCalendarEventManager* getInstance();
    virtual ~SpecialCalendarEventManager();
    
    bool isXmasTime();
    
private:
    SpecialCalendarEventManager();
    
    long getEpochTimeForTodayWithoutYear();
    long getEpochTimeForDateWithoutYear(int month, int day);
};

NS_AZOOMEE_END

#endif
