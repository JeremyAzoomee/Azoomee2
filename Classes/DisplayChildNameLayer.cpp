#include "DisplayChildNameLayer.h"
#include "ChildDataProvider.h"
#include "ui/UIScale9Sprite.h"

USING_NS_CC;

Scene* DisplayChildNameLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = DisplayChildNameLayer::create();
    scene->addChild(layer);

    return scene;
}

bool DisplayChildNameLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto childNameLabel = addChildNameToLayer();
    addFrameToLayer(childNameLabel);
    
    return true;
}

//---------------------------------------------------------All methods beyond this line are called internally only-----------------------------------------------

Label* DisplayChildNameLayer::addChildNameToLayer()
{
    auto childNameLabel = Label::createWithTTF(ChildDataProvider::getInstance()->getLoggedInChildName(), "fonts/azoomee.ttf", 70);
    childNameLabel->setColor(Color3B::WHITE);
    childNameLabel->setPosition(Director::getInstance()->getVisibleSize().width / 2, 350);
    childNameLabel->setOpacity(0);
    this->addChild(childNameLabel);
    
    CCLOG("Layer added");
    
    childNameLabel->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    return childNameLabel;
}

void DisplayChildNameLayer::addFrameToLayer(Label *childNameLabel)
{
    Rect spriteRect = Rect(0, 0, 805, 302);
    Rect capInsents = Rect(50, 58, 725, 174);
    
    float offset = 60;
    
    Point position = childNameLabel->getPosition();
    Size size = childNameLabel->getContentSize();
    
    auto labelBackground = ui::Scale9Sprite::create("res/mainhub/logged_in_as.png", spriteRect, capInsents);
    labelBackground->setContentSize(Size(size.width + offset * 2, size.height + offset * 2));
    labelBackground->setPosition(Point(position.x, position.y + offset / 2));
    this->addChild(labelBackground);
    
    labelBackground->setScale(0);
    labelBackground->runAction(Sequence::create(DelayTime::create(0.5), EaseElasticOut::create(ScaleTo::create(0.5, 1)), NULL));
}

std::string DisplayChildNameLayer::getLoggedInChildName()
{
    return ChildDataProvider::getInstance()->getLoggedInChildName();
}
