#ifndef AzoomeeChat_FriendListSceneArtPreview_h
#define AzoomeeChat_FriendListSceneArtPreview_h

#include "../AzoomeeChat.h"
#include "../ChatAPI.h"
#include "FriendListView.h"
#include "AvatarWidget.h"
#include "TitleBarWidget.h"
#include <AzoomeeCommon/UI/Scene.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/PrivacyLayer.h>

NS_AZOOMEE_CHAT_BEGIN
    
class FriendListSceneArtPreview : public Azoomee::Scene, public ChatAPIObserver
{
    typedef Azoomee::Scene Super;
private:
    
    /// A friend object which represents the current user
    FriendRef _currentUser;
    
    /// Root layout for all elements
    cocos2d::ui::Layout* _rootLayout = nullptr;
    /// Back button
    cocos2d::ui::Button* _backButton = nullptr;
    
    /// Container layout for the content (always sits below the titlebar)
    cocos2d::ui::Layout* _contentLayout = nullptr;
    /// Layout for art image being set from art app
    cocos2d::ui::Layout* _artPreviewLayout = nullptr;
    /// Sub title bar at the top of the content layout
    cocos2d::ui::Layout* _subTitleBar = nullptr;
    /// Sub title bar border
    cocos2d::ui::Layout* _subTitleBarBorder = nullptr;
    /// List of friends displayed to the user
    FriendListView* _friendListView = nullptr;
    /// List of friends we last recieved from server
    FriendList _friendListData;
    
    
    /// Create the content UI
    void createContentUI(cocos2d::ui::Layout* parent);
    
    void createArtPreviewUI(cocos2d::ui::Layout* parent);
    
    /// Create the UI for the sub title bar
    void createSubTitleBarUI(cocos2d::ui::Layout* parent);
    
    /// Back button was pressed
    void onBackButtonPressed();
    /// Friend was selected
    void onFriendListItemSelected(const FriendRef& friendData);
    
    // - ChatAPIObserver
    void onChatAPIGetFriendList(const FriendList& friendList, int amountOfNewMessages) override;
    void onChatAPIErrorRecieved(const std::string& requestTag, long errorCode) override;
    
protected:
    
    /// Called when the content size of this scene has changed
    virtual void onSizeChanged() override;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    CREATE_FUNC(FriendListSceneArtPreview);
};

NS_AZOOMEE_CHAT_END

#endif
