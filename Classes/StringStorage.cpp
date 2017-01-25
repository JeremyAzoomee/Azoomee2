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

// --------------------- Error Messages ----------------------

std::map<std::string, std::string> StringStorage::getErrorMessageStrings(long errorCode)
{
    std::map<std::string, std::string> errorStrings;
    
    switch(errorCode){
        case 401 :
        case 419 :
            errorStrings[ERROR_TITLE] = "Login Required";
            errorStrings[ERROR_BODY] = "Your session has timed out; please login again.";
            errorStrings[ERROR_BUTTON] = "OK";
            break;
            
        case 403  :
            errorStrings[ERROR_TITLE] = "You Can't Do That";
            errorStrings[ERROR_BODY] = "Please ask a grown-up to help you.";
            errorStrings[ERROR_BUTTON] = "OK";
            break;

        default :
            errorStrings[ERROR_TITLE] = "Something Went Wrong";
            errorStrings[ERROR_BODY] = "There was a problem selecting that child; please try again.";
            errorStrings[ERROR_BUTTON] = "OK";
    }
    
    return errorStrings;
}
