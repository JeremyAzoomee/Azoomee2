//
//  NavigationBar.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 20/08/2019.
//

#ifndef NavigationBar_h
#define NavigationBar_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "NavigationButton.h"

NS_AZOOMEE_BEGIN

enum class HQType {GAME, VIDEO, CHAT, OOMEE};

class NavigationBar : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<bool(HQType)> HQSelectedCallback;
private:
    
    NavigationButton* _gameHQButton = nullptr;
    NavigationButton* _videoHQButton = nullptr;
    NavigationButton* _chatHQButton = nullptr;
    NavigationButton* _oomeeHQButton = nullptr;
    
    cocos2d::Sprite* _chatNotificationIcon = nullptr;
    
    HQSelectedCallback _callback = nullptr;
    
public:
    
    static const std::map<std::string, HQType> kHQNameToTypeConv;
    static const std::map<HQType,std::string> kHQTypeToNameConv;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setHQSelectedCallback(const HQSelectedCallback& callback);
    void setChatNotificationEnabled(bool enabled);
    void toggleHQSelected(const HQType& hq);
    void toggleHQSelected(const std::string& hqName);
    
    CREATE_FUNC(NavigationBar);
};

NS_AZOOMEE_END

#endif /* NavigationBar_h */
