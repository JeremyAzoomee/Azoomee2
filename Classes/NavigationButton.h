//
//  NavigationButton.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 20/08/2019.
//

#ifndef NavigationButton_h
#define NavigationButton_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class NavigationButton : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    static const float kFadeAnimDuration;
    
    cocos2d::ui::ImageView* _circle = nullptr;
    cocos2d::ui::ImageView* _activeIcon = nullptr;
    cocos2d::ui::ImageView* _inactiveIcon = nullptr;
    
public:
    
    bool init() override;
    
    void setCircleColour(const cocos2d::Color3B& colour);
    void setButtonIcons(const std::string& activeIcon, const std::string& inactiveIcon);
    
    void setActive(bool active);
    
    CREATE_FUNC(NavigationButton);
};

NS_AZOOMEE_END

#endif /* NavigationButton_h */
