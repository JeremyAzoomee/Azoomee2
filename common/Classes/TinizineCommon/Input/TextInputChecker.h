#ifndef TinizineCommon_TextInputChecker_h
#define TinizineCommon_TextInputChecker_h

#include <string>
#include "../Tinizine.h"


NS_TZ_BEGIN

class TextInputChecker
{
public:
    
    static const int kMaxAge;
    static const int kMinNewPasswordLength;
    static const int kMinPasswordEntryLength;
    
    static bool isCharacter(const char Character);
    static bool isNumber(const char Character);
    static bool isValidEmailAddress(const char* EmailAddress);
    static bool isValidPassword(const char *password, int minNoOfChars);
    static bool isValidPin(const char *pinNumber);
    static bool isDate(int day, int month, int year);
    static bool childNameExists(std::string newChildsName);
    static bool isValidChildName(const char *childName);
    static bool hasAzoomeeEmailAddress(std::string emailAddress);
    static int ageFromDOBString(std::string dobString);
    static bool isDateInFuture(std::string dobString);
    static bool isValidVoucher(const std::string& voucher);
    static bool isValidAge(const std::string& ageString);
    
};

NS_TZ_END

#endif
