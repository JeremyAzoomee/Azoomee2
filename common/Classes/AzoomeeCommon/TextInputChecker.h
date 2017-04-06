#include <string>

namespace Azoomee
{

bool isCharacter(const char Character);
bool isNumber(const char Character);
bool isValidEmailAddress(const char* EmailAddress);
bool isValidPassword(const char *password);
bool isValidPin(const char *pinNumber);
bool isDate(int day, int month, int year);
bool childNameExists(std::string newChildsName);
bool isValidChildName(const char *childName);
bool hasAzoomeeEmailAddress(std::string emailAddress);
int ageFromDOBString(std::string dobString);
  
}
