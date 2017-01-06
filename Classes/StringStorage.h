#include "cocos2d.h"

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
};
