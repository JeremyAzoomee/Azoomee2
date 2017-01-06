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

std::string StringStorage::getStringForChildAccount(std::string labelName)
{
    std::map<std::string, std::string> childAccountStrings;
    
    childAccountStrings["requestChildName"] = "Hello! Please add a display name.";
    childAccountStrings["editChildName"] = "Want to change the display name?";
    childAccountStrings["requestChildBirthDay"] = "Please add date of birth for";
    childAccountStrings["editChildBirthDay"] = "Change birthday?";
    childAccountStrings["requestChildOomee"] = "Select your favourite Oomee.";
    childAccountStrings["requestChildOomeeSub"] = "Don't worry you can change it later.";
    childAccountStrings["editChildOomee"] = "Change your Oomee?";
    
    childAccountStrings["phDobDay"] = "DD";
    childAccountStrings["phDobMonth"] = "MM";
    childAccountStrings["phDobYear"] = "YYYY";
    
    return childAccountStrings[labelName];
}

//---------------------------------------------Login scene strings-------------

std::string StringStorage::getStringForLoginScene(std::string labelName)
{
    std::map<std::string, std::string> loginStrings;
    
    loginStrings["signUpLoginLabel"] = "Do you want to";
    loginStrings["loginButton"] = "Log in";
    loginStrings["signupButton"] = "Sign up";
    loginStrings["emailLabel"] = "Login e-mail";
    loginStrings["passwordLabel"] = "Password";
    
    return loginStrings[labelName];
}
