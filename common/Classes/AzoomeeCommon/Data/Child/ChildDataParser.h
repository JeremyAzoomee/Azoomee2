#ifndef AzoomeeCommon_ChildDataParser_h
#define AzoomeeCommon_ChildDataParser_h

#include <cocos/cocos2d.h>
#include "../Json.h"


namespace Azoomee
{

class ChildDataParser : public cocos2d::Ref
{
    
public:
    static ChildDataParser* getInstance(void);

    virtual ~ChildDataParser();
    bool init(void);
    
    bool parseChildLoginData(const std::string &responseData);
	
    void setChildLoggedIn(bool loggedIn);
	
	void loginChildOffline(const std::string& childId);
	
	void loginAnonChild(const std::string& loginDataStr);
};

}

#endif
