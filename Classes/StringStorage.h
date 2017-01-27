#include "cocos2d.h"

//------------ SETTINGS APP EXIT SCREEN --------------

#define EXIT_BUTTON_TEXT "Exit App"
#define LOG_OUT_BUTTON_TEXT "Log Out"

//------------ PIN NUMBER REQUEST AND ERROR ----------------

#define PIN_REQUEST_TITLE "Please enter your pin to proceed"

//------------- ERROR MESSAGE EMAIL VERIFICATION REQUIRED ------

#define EMAIL_NOT_VERIFIED_ERROR_TITLE_TEXT "Email Verification Required"
#define EMAIL_NOT_VERIFIED_ERROR_BODY_TEXT "For security reasons, you'll need to verify your email address before you can add more children. Follow the instructions in the email that we've sent you."
#define EMAIL_NOT_VERIFIED_ERROR_BUTTON_TEXT "OK"

//------------- ERROR MAP TITLES --------------

#define ERROR_TITLE "errorTitle"
#define ERROR_BODY "errorBody"
#define ERROR_BUTTON "errorButton"

//--------------- PREVIEW MODE MESSAGE BOX ------------
#define SIGNUP_BUTTON_TEXT "Sign up"
#define LOGIN_BUTTON_TEXT "Log in"
#define CANCEL_BUTTON_TEXT "Cancel"
#define PREVIEW_MESSAGEBOX_TITLE "Sign Up or Log In"
#define PREVIEW_MESSAGEBOX_BODY "You need to log in or sign up to watch content."

class StringStorage : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static StringStorage* getInstance(void);
    
public:
    virtual ~StringStorage();
    bool init(void);
    
    //Onboarding scene strings
    std::string getStringForOnboarding(std::string labelName);
    
    //ChildAccount scene strings
    std::string getStringForChildAccount(std::string labelName);
    
    //Login scene strings
    std::string getStringForLoginScene(std::string labelName);
    
    std::map<std::string, std::string> getErrorMessageStrings(long errorCode);
};

