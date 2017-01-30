#include "StringStorage.h"
#include "ModalMessages.h"

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
            
        case -1  :
            errorStrings[ERROR_TITLE] = "No internet";
            errorStrings[ERROR_BODY] = "No internet connection available, please try again after connecting to the internet.";
            errorStrings[ERROR_BUTTON] = "OK";
            break;
            
        case ERROR_CODE_INVALID_CREDENTIALS:
             errorStrings[ERROR_TITLE] = "That Didn't Work";
             errorStrings[ERROR_BODY] = "There was a problem with either your email or your password; please try logging in again.";
             errorStrings[ERROR_BUTTON] = "OK";
             break;
             
        case ERROR_CODE_ANY_OTHER_LOGIN_ERROR :
             errorStrings[ERROR_TITLE] = "Something Went Wrong";
             errorStrings[ERROR_BODY] = "There was a problem logging you in; please try again.";
             errorStrings[ERROR_BUTTON] = "OK";
             break;
             
        case ERROR_CODE_EMAIL_VARIFICATION_REQUIRED :
             errorStrings[ERROR_TITLE] = "Email Verification Required";
             errorStrings[ERROR_BODY] = "For security reasons, you'll need to verify your email address before you can add more children. Follow the instructions in the email that we've sent you.";
             errorStrings[ERROR_BUTTON] = "OK";
             break;
             
        case ERROR_CODE_INCORRECT_PIN :
             errorStrings[ERROR_TITLE] = "Pin Error";
             errorStrings[ERROR_BODY] = "Please enter your correct 4 digit pin to continue.";
             errorStrings[ERROR_BUTTON] = "OK";
             break;
            
        case ERROR_CODE_NAME_EXISTS :
            errorStrings[ERROR_TITLE] = "Display Name Exists";
            errorStrings[ERROR_BODY] = "Please insert a unique name.";
            errorStrings[ERROR_BUTTON] = "OK";
            break;

        default :
            errorStrings[ERROR_TITLE] = "Something Went Wrong";
            errorStrings[ERROR_BODY] = "No worries, a bunch of well trained Oomees are already working on this issue.";
            errorStrings[ERROR_BUTTON] = "OK";
    }
    
    return errorStrings;
}
