//
//  FriendRequestLayer.h
//  azoomee2
//
//  Created by Macauley on 04/09/2018.
//

#ifndef FriendRequestLayer_h
#define FriendRequestLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <AzoomeeCommon/Data/Parent/FriendRequest.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "SettingsMessageBoxRejectFR.h"

NS_AZOOMEE_BEGIN

enum class InviteState {PENDING, CONFIRMED, REJECTED};

class FriendRequestLayer : public cocos2d::ui::Layout, public Azoomee::HttpRequestCreatorResponseDelegate, public SettingsMessageBoxDelegate
{
    typedef cocos2d::ui::Layout Super;
private:
	FriendRequestRef _friendRequest = nullptr;
    
    InviteState _state = InviteState::PENDING;
    
    cocos2d::ClippingNode* _contentClippingNode = nullptr;
    cocos2d::ui::Layout* _rejectButton = nullptr;
    cocos2d::ui::Layout* _confirmButton = nullptr;
    cocos2d::ui::Layout* _rejectedBanner = nullptr;
    cocos2d::ui::Layout* _confirmedBanner = nullptr;
    
    cocos2d::Label* _senderText = nullptr;
    cocos2d::Label* _recipientText = nullptr;
    cocos2d::Sprite* _arrowIcon = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void changeToState(InviteState state);
    void setChildDetails(const FriendRequestRef& friendRequest);
    
    CREATE_FUNC(FriendRequestLayer);
    
    //Delegate Functions
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
    void onButtonPressed(SettingsMessageBox* pSender, SettingsMessageBoxButtonType type) override;
};

NS_AZOOMEE_END


#endif /* FriendRequestLayer_h */
