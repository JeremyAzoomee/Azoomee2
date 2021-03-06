#ifndef AzoomeeCommon_TextInputChecker_h
#define AzoomeeCommon_TextInputChecker_h

#include <string>


namespace Azoomee
{
const int kMaxAge = 99;
const int kMinNewPasswordLength = 6;
const int kMinPasswordEntryLength = 2;
    
bool isCharacter(const char Character);
bool isNumber(const char Character);
bool isValidEmailAddress(const char* EmailAddress);
bool isValidPassword(const char *password, int minNoOfChars);
bool isValidPin(const char *pinNumber);
bool isDate(int day, int month, int year);
bool childNameExists(std::string newChildsName);
bool isValidChildName(const char *childName);
bool hasAzoomeeEmailAddress(std::string emailAddress);
int ageFromDOBString(std::string dobString);
bool isDateInFuture(std::string dobString);
bool isValidVoucher(const std::string& voucher);
bool isValidAge(const std::string& ageString);
	
}

#endif
