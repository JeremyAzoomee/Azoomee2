#include "TextInputChecker.h"

using namespace cocos2d;

bool TextInputChecker::isCharacter(const char Character)
{
    return ( (Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
}
bool TextInputChecker::isNumber(const char Character)
{
    return ( Character >= '0' && Character <= '9');
}

bool TextInputChecker::isValidEmailAddress(const char * EmailAddress)
{
    if(!EmailAddress) return 0;
    if(!isCharacter(EmailAddress[0])) return 0;
    
    int AtOffset = -1;
    int DotOffset = -1;
    
    unsigned int Length = (int)strlen(EmailAddress);
    for(unsigned int i = 0; i < Length; i++)
    {
        if(EmailAddress[i] == '@') AtOffset = (int)i;
        else if(EmailAddress[i] == '.') DotOffset = (int)i;
    }
    
    if(AtOffset == -1 || DotOffset == -1) return 0;
    if(AtOffset > DotOffset) return 0;
    
    return !(DotOffset >= ((int)Length-1));
}

bool TextInputChecker::isValidPassword(const char * password)
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

bool TextInputChecker::isValidPin(const char * pinNumber)
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
