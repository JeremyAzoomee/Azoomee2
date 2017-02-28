#include "ElectricDreamsDecoration.h"

void addSideWiresToScreen(Node* parentLayer, float withDelay, float withDuration)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto wireLeft = Sprite::create("res/decoration/wireLeft.png");
    wireLeft->setPosition(wireLeft->getContentSize().width / -2 + origin.x, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireLeft);
    
    auto wireRight = Sprite::create("res/decoration/wireRight.png");
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
    wireLeft->setPosition(wireLeft->getContentSize().width / 2, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireLeft);
    
    auto wireRight = Sprite::create("res/decoration/wireRight.png");
    wireRight->setPosition(visibleSize.width - wireRight->getContentSize().width / 2, visibleSize.height / 2 + origin.y);
    parentLayer->addChild(wireRight);
}

void addGlowToScreen(Node* parentLayer, float withDelay)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto bgGlow = Sprite::create("res/decoration/bg_glow.png");
    bgGlow->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    bgGlow->setOpacity(0);
    parentLayer->addChild(bgGlow);
    
    bgGlow->runAction(FadeIn::create(withDelay));
}

void addGlowToScreen(Node* parentLayer)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto bgGlow = Sprite::create("res/decoration/bg_glow.png");
    bgGlow->setPosition(visibleSize.width / 2, origin.y + visibleSize.height / 2);
    parentLayer->addChild(bgGlow);
}
