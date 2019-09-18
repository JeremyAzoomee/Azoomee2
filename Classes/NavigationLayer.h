#ifndef __NAVIGATION_LAYER_H__
#define __NAVIGATION_LAYER_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeChat/ChatAPI.h>
#include <AzoomeeChat/Data/Message.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>

NS_AZOOMEE_BEGIN

class NavigationLayer : public cocos2d::Node
{
	typedef cocos2d::Node Super;
public:
    CREATE_FUNC(NavigationLayer);

    virtual bool init() override;
    void startLoadingGroupHQ(std::string uri);
    void changeToScene(const std::string& hqName, float duration);
	
	void onEnter() override;
    void onExit() override;
    
    void showNotificationBadge();
    void hideNotificationBadge();
	
	void setButtonOn(const std::string& hqName);
	
private:
    //MenuItem creation phase
	cocos2d::ui::Button* addMenuItemHolder(const std::string& hqName, float pos);
    cocos2d::Sprite* addMenuItemCircle(const std::string& hqName, cocos2d::Node *toBeAddedTo);
    cocos2d::Sprite* addMenuItemInactive(const std::string& hqName, cocos2d::Node *toBeAddedTo);
    cocos2d::Sprite* addMenuItemActive(const std::string& hqName, cocos2d::Node *toBeAddedTo);
    void addListenerToMenuItem(Node *toBeAddedTo);
    
    void addXmasDecorationToMenuItem(const std::string& hqName, cocos2d::Node *toBeAddedTo);
    
    void runDisplayAnimationForMenuItem(cocos2d::Node* node1, bool quick);
    
    //Handling created menuitems or all menuitems
    void turnOffAllMenuItems();
    void turnOnMenuItem(const std::string& hqName);
	
    void delayedSetButtonOn(float dt);
    
    //chat notifications
    void addNotificationBadgeToChatIcon(cocos2d::Node* chatIcon);
    
};

NS_AZOOMEE_END

#endif
