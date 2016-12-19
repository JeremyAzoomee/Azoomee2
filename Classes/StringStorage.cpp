#include "StringStorage.h"

using namespace cocos2d;

static StringStorage *_sharedStringStorage = NULL;

StringStorage* StringStorage::getInstance()
{
    if (! _sharedStringStorage)
    {
        _sharedStringStorage = new StringStorage();
        _sharedStringStorage->init();
    }
    
    return _sharedStringStorage;
}

StringStorage::~StringStorage(void)
{
}

bool StringStorage::init(void)
{
    return true;
}

//--------------------------------------------------Onboarding scene strings

std::string StringStorage::getStringForOnboarding(std::string labelName)
{
    std::map<std::string, std::string> onboardingStrings;
    
    onboardingStrings["email"] = "Registration E-mail Address";
    onboardingStrings["password"] = "Create your Azoomee Password";
    onboardingStrings["pintitle"] = "Create your Azoomee App 4 digit pin";
    onboardingStrings["pinsubtitle"] = "Shhh! Don't tell the kids...";
    
    return onboardingStrings[labelName];
}
