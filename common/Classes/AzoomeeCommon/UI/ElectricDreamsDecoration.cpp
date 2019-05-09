#include "ElectricDreamsDecoration.h"
#include "Scene.h"
#include <ui/UIScale9Sprite.h>

using namespace cocos2d;


namespace Azoomee
{

void addSideWiresToScreen(Node* parentLayer, float withDelay, float withDuration, float opacity)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = dynamic_cast<Azoomee::Scene*>(Director::getInstance()->getRunningScene()) ? Vec2(0,0) : Director::getInstance()->getVisibleOrigin();
    
    auto wireLeft = Sprite::create("res/decoration/wireLeft.png");
    wireLeft->setName("Wire");
    wireLeft->setPosition(wireLeft->getContentSize().width / -2 + origin.x, visibleSize.height / 2 + origin.y);
    wireLeft->setOpacity(255 * opacity);
    parentLayer->addChild(wireLeft);
    
    auto wireRight = Sprite::create("res/decoration/wireRight.png");
    wireRight->setName("Wire");
    wireRight->setPosition(wireRight->getContentSize().width / 2 + visibleSize.width + origin.x, visibleSize.height / 2 + origin.y);
    wireRight->setOpacity(255 * opacity);
    parentLayer->addChild(wireRight);
    
    wireLeft->runAction(Sequence::create(DelayTime::create(withDelay), EaseOut::create(MoveTo::create(1, Vec2(wireLeft->getContentSize().width / 2+ origin.x, visibleSize.height / 2 + origin.y)), withDuration), NULL));
    
    wireRight->runAction(Sequence::create(DelayTime::create(withDelay), EaseOut::create(MoveTo::create(1, Vec2(visibleSize.width - wireRight->getContentSize().width / 2+ origin.x, visibleSize.height / 2 + origin.y)), withDuration), NULL));
    
}

void addSideWiresToScreen(Node* parentLayer)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = dynamic_cast<Azoomee::Scene*>(Director::getInstance()->getRunningScene()) ? Vec2(0,0) : Director::getInstance()->getVisibleOrigin();
    
    auto wireLeft = Sprite::create("res/decoration/wireLeft.png");
    wireLeft->setName("Wire");
    wireLeft->setPosition(wireLeft->getContentSize().width / 2, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireLeft);
    
    auto wireRight = Sprite::create("res/decoration/wireRight.png");
    wireRight->setName("Wire");
    wireRight->setPosition(visibleSize.width - wireRight->getContentSize().width / 2, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireRight);
}
    
void removeWiresFromScreen(cocos2d::Node* parentLayer)
{
    while(parentLayer->getChildByName("Wire"))
        parentLayer->removeChildByName("Wire");
}
    
cocos2d::Layer* createWindowLayer(float height)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    return createWindowLayer(visibleSize.width * 0.66f, height);
}
    
cocos2d::Layer* createWhiteWindowLayer(float height)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    return createWhiteWindowLayer(visibleSize.width * 0.66f, height);
}

Layer* createWindowLayer(float width, float height)
{
    Layer* newLayer = Layer::create();
    newLayer->setContentSize(Size(width, height));
    
    Rect spriteRect = Rect(0, 0, 455, 268);
    Rect capInsents = Rect(100, 100, 255, 1);
    
    ui::Scale9Sprite* newWindow = ui::Scale9Sprite::create("res/decoration/windowScale9.png", spriteRect, capInsents);
    newWindow->setContentSize(Size(newLayer->getContentSize().width, newLayer->getContentSize().height));
    newWindow->setPosition(newLayer->getContentSize().width/2, newLayer->getContentSize().height/2);

    newLayer->addChild(newWindow);

    return newLayer;
}
cocos2d::Layer* createWhiteWindowLayer(float width, float height)
{
    Layer* newLayer = Layer::create();
    newLayer->setContentSize(Size(width, height));
    
    Rect spriteRect = Rect(0, 0, 467, 230);
    Rect capInsents = Rect(100, 100, 255, 1);
    
    ui::Scale9Sprite* newWindow = ui::Scale9Sprite::create("res/decoration/whiteWindowScale9.png", spriteRect, capInsents);
    newWindow->setContentSize(Size(newLayer->getContentSize().width, newLayer->getContentSize().height));
    newWindow->setPosition(newLayer->getContentSize().width/2, newLayer->getContentSize().height/2);
    
    newLayer->addChild(newWindow);
    
    return newLayer;
}
  
}
