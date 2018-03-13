//
//  TimeFunctions.h
//  AzoomeeCommon
//
//  Created by Macauley on 31/01/2018.
//

#ifndef TimeFunctions_h
#define TimeFunctions_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>
#include <time.h>

NS_AZOOMEE_BEGIN

bool isDateStringOlderThanToday(const std::string &dateToCheck);
bool isDateStringFormatCorrectForComparison(const std::string &dateToCheck);
int birthYearFromAge(int age);
long getEpochTimeForTodayWithoutYear();
long getEpochTimeForDateWithoutYear(int month, int day);
std::string getTimeStringForFileName();

NS_AZOOMEE_END

#endif /* TimeFunctions_h */
