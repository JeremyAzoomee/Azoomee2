#include "ElectricDreamsDecoration.h"
#include <ui/UIScale9Sprite.h>

using namespace cocos2d;


namespace Azoomee
{

void addSideWiresToScreen(Node* parentLayer, float withDelay, float withDuration)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto wireLeft = Sprite::create("res/decoration/wireLeft.png");
    wireLeft->setName("Wire");
    wireLeft->setPosition(wireLeft->getContentSize().width / -2 + origin.x, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireLeft);
    
    auto wireRight = Sprite::create("res/decoration/wireRight.png");
    wireRight->setName("Wire");
    wireRight->setPosition(wireRight->getContentSize().width / 2 + visibleSize.width + origin.x, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireRight);
    
    wireLeft->runAction(Sequence::create(DelayTime::create(withDelay), EaseOut::create(MoveTo::create(1, Vec2(wireLeft->getContentSize().width / 2+ origin.x, visibleSize.height / 2 + origin.y)), withDuration), NULL));
    
    wireRight->runAction(Sequence::create(DelayTime::create(withDelay), EaseOut::create(MoveTo::create(1, Vec2(visibleSize.width - wireRight->getContentSize().width / 2+ origin.x, visibleSize.height / 2 + origin.y)), withDuration), NULL));
    
}

void addSideWiresToScreen(Node* parentLayer)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto wireLeft = Sprite::create("res/decoration/wireLeft.png");
    wireLeft->setName("Wire");
    wireLeft->setPosition(wireLeft->getContentSize().width / 2, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireLeft);
    
    auto wireRight = Sprite::create("res/decoration/wireRight.png");
    wireRight->setName("Wire");
    wireRight->setPosition(visibleSize.width - wireRight->getContentSize().width / 2, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireRight);
}
    
void addMainHubSideWiresToScreen(cocos2d::Node* parentLayer, float withDelay, float withDuration)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto wireLeft = Sprite::create("res/decoration/wireLeft_MainHub.png");
    wireLeft->setName("Wire");
    wireLeft->setPosition(wireLeft->getContentSize().width / -2 + origin.x, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireLeft);
    
    auto wireRight = Sprite::create("res/decoration/wireRight_MainHub.png");
    wireRight->setName("Wire");
    wireRight->setPosition(wireRight->getContentSize().width / 2 + visibleSize.width + origin.x, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireRight);
    
    wireLeft->runAction(Sequence::create(DelayTime::create(withDelay), EaseOut::create(MoveTo::create(1, Vec2(wireLeft->getContentSize().width / 2+ origin.x, visibleSize.height / 2 + origin.y)), withDuration), NULL));
    
    wireRight->runAction(Sequence::create(DelayTime::create(withDelay), EaseOut::create(MoveTo::create(1, Vec2(visibleSize.width - wireRight->getContentSize().width / 2+ origin.x, visibleSize.height / 2 + origin.y)), withDuration), NULL));
}

void addMainHubSideWiresToScreen(cocos2d::Node* parentLayer)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto wireLeft = Sprite::create("res/decoration/wireLeft_MainHub.png");
    wireLeft->setName("Wire");
    wireLeft->setPosition(wireLeft->getContentSize().width / 2, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireLeft);
    
    auto wireRight = Sprite::create("res/decoration/wireRight_MainHub.png");
    wireRight->setName("Wire");
    wireRight->setPosition(visibleSize.width - wireRight->getContentSize().width / 2, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireRight);

}
    
void removeWiresFromScreen(cocos2d::Node* parentLayer)
{
    while(parentLayer->getChildByName("Wire"))
        parentLayer->getChildByName("Wire");
}

void addGlowToScreen(Node* parentLayer, float withDelay)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto bgGlow = createGlow();
    bgGlow->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    bgGlow->setOpacity(0);
    parentLayer->addChild(bgGlow);
    
    bgGlow->runAction(FadeIn::create(withDelay));
}

void addGlowToScreen(Node* parentLayer)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto bgGlow = createGlow();
    bgGlow->setPosition(visibleSize.width / 2, origin.y + visibleSize.height / 2);
    parentLayer->addChild(bgGlow);
}

Sprite* createGlow()
{
    auto bgGlow = Sprite::create("res/decoration/bg_glow.png");

    return bgGlow;
}

void addFullScreenGlowToScreen(Node* parentLayer)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto fullscreenGlow = Sprite::create("res/decoration/fullscreen_glow.png");
    fullscreenGlow->setScaleX(visibleSize.width / fullscreenGlow->getContentSize().width);
    fullscreenGlow->setScaleY(visibleSize.height / fullscreenGlow->getContentSize().height);
    fullscreenGlow->setPosition(visibleSize.width / 2, origin.y + visibleSize.height / 2);
    parentLayer->addChild(fullscreenGlow);
}
    
cocos2d::Layer* createWindowLayer(float height)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    return createWindowLayer(visibleSize.width*.66, height);
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
  
}
