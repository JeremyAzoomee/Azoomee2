#ifndef AzoomeeChat_FriendListScene_h
#define AzoomeeChat_FriendListScene_h

#include "../AzoomeeChat.h"
#include "../ChatAPI.h"
#include "FriendListView.h"
#include "AvatarWidget.h"
#include "TitleBarWidget.h"
#include <AzoomeeCommon/UI/Scene.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class FriendListScene : public Azoomee::Scene, public ChatAPIObserver
{
    typedef Azoomee::Scene Super;
private:
    
    /// A friend object which represents the current user
    FriendRef _currentUser;
    
    /// Root layout for all elements
    cocos2d::ui::Layout* _rootLayout = nullptr;
    /// Titlebar at the top of the screen
    TitleBarWidget* _titleBar = nullptr;
    
    /// Container layout for the content (always sits below the titlebar)
    cocos2d::ui::Layout* _contentLayout = nullptr;
    /// Sub title bar at the top of the content layout
    cocos2d::ui::Layout* _subTitleBar = nullptr;
    /// Sub title bar border
    cocos2d::ui::Layout* _subTitleBarBorder = nullptr;
    /// List of friends
    FriendListView* _friendListView = nullptr;
    
    
    /// Create the content UI
    void createContentUI(cocos2d::ui::Layout* parent);
    /// Create the UI for the sub title bar
    void createSubTitleBarUI(cocos2d::ui::Layout* parent);
    
    /// Back button was pressed
    void onBackButtonPressed();
    /// Friend was selected
    void onFriendListItemSelected(const FriendRef& friendData);
    
    // - ChatAPIObserver
    void onChatAPIGetFriendList(const FriendList& friendList) override;
    
protected:
    
    /// Called when the content size of this scene has changed
    virtual void onSizeChanged() override;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    CREATE_FUNC(FriendListScene);
};

NS_AZOOMEE_CHAT_END

#endif
