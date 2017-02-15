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
    
    onboardingStrings["email"] = "Parents: enter your email address";
    onboardingStrings["password"] = "Create a password";
    onboardingStrings["pintitle"] = "Create a 4 digit PIN";
    onboardingStrings["pinsubtitle"] = "You will need this to access the parent area and exit the app\nShhh! Don't tell the kids...";
    
    return onboardingStrings[labelName];
}

std::string StringStorage::getStringForChildAccount(std::string labelName)
{
    std::map<std::string, std::string> childAccountStrings;
    
    childAccountStrings["requestChildName"] = "What's your child's name";
    childAccountStrings["editChildName"] = "Want to change the display name?";
    childAccountStrings["requestChildBirthDay"] = "Please add date of birth for";
    childAccountStrings["requestChildBirthDaySubTitle"] = "We need this to show content specifically for their age";
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
    loginStrings["emailLabel"] = "Log in. Please enter your email.";
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
            errorStrings[ERROR_BODY] = "Your session has timed out. Please login again.";
            errorStrings[ERROR_BUTTON] = "OK";
            break;
            
        case 403  :
            errorStrings[ERROR_TITLE] = "You Can't Do That";
            errorStrings[ERROR_BODY] = "Please ask a grown-up to help you.";
            errorStrings[ERROR_BUTTON] = "OK";
            break;
            
        case 409  :
            errorStrings[ERROR_TITLE] = "Already Registered";
            errorStrings[ERROR_BODY] = "The email address has already been registered. Please register with a new email address or log in using this email address.";
            errorStrings[ERROR_BUTTON] = "OK";
            break;
            
        case -1  :
            errorStrings[ERROR_TITLE] = "No internet";
            errorStrings[ERROR_BODY] = "No internet connection available, please try again after connecting to the internet.";
            errorStrings[ERROR_BUTTON] = "OK";
            break;
            
        case ERROR_CODE_INVALID_CREDENTIALS:
             errorStrings[ERROR_TITLE] = "That Didn't Work";
             errorStrings[ERROR_BODY] = "There was a problem with your email or password. Please try logging in again.";
             errorStrings[ERROR_BUTTON] = "OK";
             break;
             
        case ERROR_CODE_ANY_OTHER_LOGIN_ERROR :
             errorStrings[ERROR_TITLE] = "Something Went Wrong";
             errorStrings[ERROR_BODY] = "There was a problem logging you in. Please try again.";
             errorStrings[ERROR_BUTTON] = "OK";
             break;
             
        case ERROR_CODE_EMAIL_VARIFICATION_REQUIRED :
             errorStrings[ERROR_TITLE] = "Email Verification Required";
             errorStrings[ERROR_BODY] = "For security reasons, you'll need to verify your email address before you can add more children. Please follow the instructions in the email that we've sent you.";
             errorStrings[ERROR_BUTTON] = "OK";
             break;
             
        case ERROR_CODE_INCORRECT_PIN :
             errorStrings[ERROR_TITLE] = "PIN Error";
             errorStrings[ERROR_BODY] = "Please enter your correct 4 digit PIN to continue.";
             errorStrings[ERROR_BUTTON] = "OK";
             break;
            
        case ERROR_CODE_NAME_EXISTS :
            errorStrings[ERROR_TITLE] = "Name Already Exists";
            errorStrings[ERROR_BODY] = "You already have a child with this name. Please enter a unique name.";
            errorStrings[ERROR_BUTTON] = "OK";
            break;
            
        case ERROR_CODE_BAD_TIME_SETTING :
            errorStrings[ERROR_TITLE] = "The time on your device is wrong";
            errorStrings[ERROR_BODY] = "Please correct the time on your device and try logging in again.";
            errorStrings[ERROR_BUTTON] = "OK";
            break;

        default :
            errorStrings[ERROR_TITLE] = "Something Went Wrong";
            errorStrings[ERROR_BODY] = "No worries, a bunch of well trained Oomees are already working on this issue.";
            errorStrings[ERROR_BUTTON] = "OK";
    }
    
    return errorStrings;
}
