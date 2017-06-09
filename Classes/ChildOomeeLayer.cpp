#include "ChildOomeeLayer.h"
#include "ui/UIScale9Sprite.h"

#define FONT_REGULAR "fonts/Sofia Pro Soft Regular.otf"
#define FONT_BOLD "fonts/Sofia Pro Soft Bold.otf"

bool ChildOomeeLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addExitOrLogoutUIObjects();
    
    return true;
}

//----------------Add UI Objects-------------

void ChildOomeeLayer::addExitOrLogoutUIObjects()
{
    Rect spriteRect = Rect(0, 0, 206, 218);
    Rect capInsents = Rect(50, 50, 100, 1);
    
    ui::Scale9Sprite* childFrame = ui::Scale9Sprite::create("res/settings/child_frame.png", spriteRect, capInsents);
    childFrame->setContentSize(Size(800,1200));
    childFrame->setAnchorPoint(Vec2(0,0));
    
    this->setContentSize(childFrame->getContentSize());
    
    childFrame->setPosition(Vec2(0, 0));
    
    this->addChild(childFrame);
}

