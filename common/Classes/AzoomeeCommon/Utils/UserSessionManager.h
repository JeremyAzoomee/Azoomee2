//
//  UserSessionManager.h
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 09/01/2020.
//

#ifndef UserSessionManager_h
#define UserSessionManager_h

#include "../Azoomee.h"
#include <functional>

NS_AZOOMEE_BEGIN

class UserSessionManager
{
    typedef std::function<void(bool)> OnCompleteCallback;
private:
    UserSessionManager();
    
public:
    
    static UserSessionManager* getInstance();
    virtual ~UserSessionManager();
    
    void refreshUserSession(const OnCompleteCallback& callback);
    
};

NS_AZOOMEE_END

#endif /* UserSessionManager_h */
