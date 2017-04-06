#include "TextInputChecker.h"
#include "Data/Parent/ParentDataProvider.h"
#include "StringFunctions.h"


namespace Azoomee
{

bool isCharacter(const char Character)
{
    return ( (Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
}
  
bool isNumber(const char Character)
{
    return ( Character >= '0' && Character <= '9');
}

bool isValidEmailAddress(const char * EmailAddress)
{
    if(!EmailAddress) return 0;
    
    int AtOffset = -1;
    int DotOffset = -1;
    
    unsigned int Length = (int)strlen(EmailAddress);
    for(unsigned int i = 0; i < Length; i++)
    {
        if(EmailAddress[i] == '@' && AtOffset != -1) return 0;
        else if(EmailAddress[i] == '@') AtOffset = (int)i;
        else if(EmailAddress[i] == '.') DotOffset = (int)i;
        else if(EmailAddress[i] == ' ') return 0;
    }
    
    if(AtOffset == -1 || DotOffset == -1) return 0;
    if(AtOffset > DotOffset) return 0;
    
    return !(DotOffset >= ((int)Length-1));
}

bool isValidPassword(const char * password)
{
    if(!password) return 0;
    if(strlen(password) < 2) return 0;
    
    bool passwordOK = true;
    
    for(unsigned int i = 0; i < strlen(password); i++)
    {
        if(password[i] == ' ') passwordOK = false;
        else if(password[i] == '\n') passwordOK = false;
        else if(password[i] == '\t') passwordOK = false;
    }
    
    if(passwordOK) return 1;
    else return 0;
}

bool isValidPin(const char * pinNumber)
{
    if(!pinNumber) return 0;
    if(strlen(pinNumber) != 4) return 0;
    
    bool pinOK = true;
    
    for(unsigned int i = 0; i < strlen(pinNumber); i++)
    {
        if(!isNumber(pinNumber[i]))
            pinOK = false;
    }
    
    if(pinOK) return 1;
    else return 0;
}

bool isDate(int day, int month, int year)
{
    if (! (1582<= year )  )//comment these 2 lines out if it bothers you
        return false;
    if (! (1<= month && month<=12) )
        return false;
    if (! (1<= day && day<=31) )
        return false;
    if ( (day==31) && (month==2 || month==4 || month==6 || month==9 || month==11) )
        return false;
    if ( (day==30) && (month==2) )
        return false;
    if ( (month==2) && (day==29) && (year%4!=0) )
        return false;
    if ( (month==2) && (day==29) && (year%400==0) )
        return true;
    if ( (month==2) && (day==29) && (year%100==0) )
        return false;
    if ( (month==2) && (day==29) && (year%4==0)  )
        return true;
    
    return true;
}

bool childNameExists(std::string newChildsName)
{
    //check if child name exists
    bool childNameExists = false;
    
    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        std::string storedChildsName = ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(i);
        
        if(newChildsName.compare(storedChildsName) == 0)
        {
            childNameExists = true;
            break;
        }
    }
    
    return childNameExists;
}

bool isValidChildName(const char * childName)
{
    if(!childName) return 0;
    if(strlen(childName) < 2) return 0;
    
    bool childNameOK = true;
    int NoOfSpaces = 0;
    
    for(unsigned int i = 0; i < strlen(childName); i++)
    {
        if(childName[i] == '\n') childNameOK = false;
        else if(childName[i] == '\t') childNameOK = false;
        
        if(childName[i] == ' ') NoOfSpaces++;
    }
    
    if(NoOfSpaces == strlen(childName)) return false;
    else if(childNameOK) return 1;
    else return 0;
}

bool hasAzoomeeEmailAddress(std::string emailAddress)
{
    auto atPosition = emailAddress.find("@");
    
    if ( atPosition != std::string::npos && stringToLower(emailAddress).find("azoomee", atPosition) != std::string::npos)
        return true;
    else
        return false;
}

int ageFromDOBString(std::string dobString)
{
  std::vector<std::string> dobSplit = splitStringToVector(dobString, "-");
  
  if(dobSplit.size() != 3)
    return -1;
  
  int DOBday = std::atoi(dobSplit.at(2).c_str());
  int DOBmonth = std::atoi(dobSplit.at(1).c_str());
  int DOByear = std::atoi(dobSplit.at(0).c_str());
  
  time_t theTime = time(NULL);
  struct tm *aTime = localtime(&theTime);
  
  int currentDay = aTime->tm_mday;
  int currentMonth = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
  int currentYear = aTime->tm_year + 1900; // Year is # years since 1900
  
  if(isDate(DOBday,DOBmonth,DOByear) && isDate(currentDay,currentMonth,currentYear))
  {
    int childAge = currentYear - DOByear;
    
    if(DOBmonth > currentMonth || (DOBmonth == currentMonth && DOBday > currentDay))
      childAge--;
    
    return childAge;
  }
  
  return -1;
}
  
}
