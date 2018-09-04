//
//  SettingsFriendshipsPage.h
//  azoomee2
//
//  Created by Macauley on 04/09/2018.
//

#ifndef SettingsFriendshipsPage_h
#define SettingsFriendshipsPage_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "SettingsPageHeader.h"

NS_AZOOMEE_BEGIN

class SettingsFriendshipsPage : public cocos2d::ui::Layout, public Azoomee::HttpRequestCreatorResponseDelegate
{
    typedef cocos2d::ui::Layout Super;
private:
    
    SettingsPageHeader* _headerBanner = nullptr;
    cocos2d::ui::ListView* _friendRequestList = nullptr;
    HttpRequestCreator* _pendingFRHttpRequest = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(SettingsFriendshipsPage);
    
    //Delegate Functions
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
};

NS_AZOOMEE_END

#endif /* SettingsFriendshipsPage_h */
