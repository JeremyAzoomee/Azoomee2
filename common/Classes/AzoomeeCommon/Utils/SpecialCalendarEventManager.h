#ifndef __SPECIALCALENDAREVENTMANAGER_H__
#define __SPECIALCALENDAREVENTMANAGER_H__

#include "cocos2d.h"
#include "../Azoomee.h"

#define DECORATION_ZORDER 9999 //usually decoration should be put on top of everything, so we define a high zorder number for that.

NS_AZOOMEE_BEGIN

class SpecialCalendarEventManager

{
    
public:
    static SpecialCalendarEventManager* getInstance();
    virtual ~SpecialCalendarEventManager();
    
    bool isXmasTime();
    
private:
    SpecialCalendarEventManager();
    
};

NS_AZOOMEE_END

#endif
