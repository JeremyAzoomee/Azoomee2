#ifndef AzoomeeChat_TitleBarWidget_h
#define AzoomeeChat_TitleBarWidget_h

#include "../AzoomeeChat.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "AvatarWidget.h"


NS_AZOOMEE_CHAT_BEGIN
    
class TitleBarWidget : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    /// The back button
    cocos2d::ui::Button* _backButton = nullptr;
    
    /// Layout to hold all title content (label, image, avatar)
    cocos2d::ui::Layout* _titleLayout = nullptr;
    
    /// Title label
    cocos2d::ui::Text* _titleLabel = nullptr;
    /// Title image
    cocos2d::ui::ImageView* _titleImage = nullptr;
    
    /// User's avatar
    AvatarWidget* _avatarWidget = nullptr;
    
    /// The alert button
    cocos2d::ui::Button* _alertButton = nullptr;
    
    /// Create a drop shadow which overhangs underneath the bounds of this bar
    /// heightPercent: height of the drop shadow as a % of it's parent
    void createDropShadow(float heightPercent);
    
    /// Update the layout of the title depending on what is being shown
    void updateTitleLayout();
    
protected:
    
    virtual void onSizeChanged() override;
    
public:
    
    /// Set the title string
    void setTitleString(const std::string& title);
    /// Set the title font color
    void setTitleColor(const cocos2d::Color3B& color);
    
    /// Set the title image
    void setTitleImage(const std::string& imagePath);
    
    /// Set the title avatar
    void setTitleAvatar(const FriendRef& friendData);
    
    /// Show or hide the alert button
    void showAlertButton(bool enable);
    
    /// Register for backbutton click events
    void addBackButtonEventListener(const cocos2d::ui::Widget::ccWidgetClickCallback& callback);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    CREATE_FUNC(TitleBarWidget);
};

NS_AZOOMEE_CHAT_END

#endif