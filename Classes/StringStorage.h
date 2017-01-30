#include "cocos2d.h"

//------------ SETTINGS APP EXIT SCREEN --------------

#define EXIT_BUTTON_TEXT "Exit App"
#define LOG_OUT_BUTTON_TEXT "Log Out"

//------------ PIN NUMBER REQUEST AND ERROR ----------------

#define PIN_REQUEST_TITLE "Please enter your PIN to proceed"

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

