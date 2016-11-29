#include "ChildSelectorScene.h"


USING_NS_CC;

Scene* ChildSelectorScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ChildSelectorScene::create();
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool ChildSelectorScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    auto emailTitle = Label::createWithTTF("Select your kid's profile", "fonts/azoomee.ttf", 90);
    emailTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.9);
    emailTitle->setColor(Color3B(28, 244, 244));
    this->addChild(emailTitle);
    
    addScrollViewForProfiles();
    addProfilesToScrollView();

    return true;
}

void ChildSelectorScene::addScrollViewForProfiles()
{
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width * 0.8, visibleSize.height * 0.6));
    scrollView->setPosition(Point(origin.x + visibleSize.width * 0.1, origin.y + visibleSize.height * 0.15));
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    scrollView->setInnerContainerSize(scrollView->getContentSize());
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(true);
    scrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    scrollView->setBackGroundColor(Color3B::BLUE);
    
    this->addChild(scrollView);
}

void ChildSelectorScene::addProfilesToScrollView()
{
    
}

void ChildSelectorScene::createChildProfileButton()
{
    
}
