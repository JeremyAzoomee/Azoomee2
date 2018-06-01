#ifndef __SPECIALCALENDAREVENTMANAGER_H__
#define __SPECIALCALENDAREVENTMANAGER_H__

#include "cocos2d.h"
#include "../Azoomee.h"

#define DECORATION_ZORDER 9999 //usually decoration should be put on top of everything, so we define a high zorder number for that.

//#define FORCE_XMAS YES
//#define FORCE_EASTER YES
//#define FORCE_HALLOWEEN YES

NS_AZOOMEE_BEGIN

enum CalenderSeasons {ANY, EASTER, HALLOWEEN, XMAS};

class SpecialCalendarEventManager

{
    
public:
    static SpecialCalendarEventManager* getInstance();
    virtual ~SpecialCalendarEventManager();
    
    bool isXmasTime();
    bool isEasterTime();
    bool isHallowenTime();
    
    CalenderSeasons getSeasonFromString(const std::string& seasonString);
    
    bool checkIfInSeason(CalenderSeasons season);
    
private:
    SpecialCalendarEventManager();
    
};

NS_AZOOMEE_END

#endif
