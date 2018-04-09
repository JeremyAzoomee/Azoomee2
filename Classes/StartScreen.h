//
//  StartScreen.h
//  azoomee2
//
//  Created by Macauley on 12/02/2018.
//

#ifndef StartScreen_h
#define StartScreen_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class StartScreen : public cocos2d::Layer
{
    typedef cocos2d::Layer Super;
private:
    void addBGElements();
    void addImages();
    void addButtons();
    
    cocos2d::Sprite* createSpriteWithBorderRing(const std::string& imageFilename, const std::string& borderFilename, int rotationDir = 1);
    
    static const std::string kAssetLoc;
    
    cocos2d::EventListener* _keyboardListener = nullptr;
    void setKeypadEnabled(bool enabled);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
public:
    static cocos2d::Scene* createScene();
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init() override;
    
    CREATE_FUNC(StartScreen);
};

NS_AZOOMEE_END


#endif /* StartScreen_h */
