//
//  SettingsKidsPage.h
//  azoomee2
//
//  Created by Macauley on 23/08/2018.
//

#ifndef SettingsKidsPage_h
#define SettingsKidsPage_h

#include <AzoomeeCommon/Tinizine.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "SettingsPageHeader.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class SettingsKidsPage : public cocos2d::ui::Layout, public HttpRequestCreatorResponseDelegate
{
    typedef cocos2d::ui::Layout Super;
private:
    
    cocos2d::ui::ListView* _kidList = nullptr;
    SettingsPageHeader* _headerBanner = nullptr;
    cocos2d::ui::Layout* _footerBanner = nullptr;
    
    void addKidsToScrollView();
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(SettingsKidsPage);
    
    //delegate functions
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
};

NS_AZ_END

#endif /* SettingsKidsPage_h */
