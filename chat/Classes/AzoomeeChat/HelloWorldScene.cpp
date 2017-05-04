#include "HelloWorldScene.h"
#include "../AppDelegate.h"

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN
    
HelloWorldScene::~HelloWorldScene()
{
    CC_SAFE_RELEASE(_parentLayout);
}

bool HelloWorldScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    cocos2d::Size visibleSize = getContentSize();
    cocos2d::log( "visibleSize: %f, %f", visibleSize.width, visibleSize.height );
    
    
    // Create the layout
    _parentLayout = ui::Layout::create();
    _parentLayout->retain();
    _parentLayout->setContentSize(visibleSize);
    _parentLayout->setPosition(Point::ZERO);
    _parentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _parentLayout->setBackGroundColor(Color3B(128, 128, 128));
    _parentLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    addChild(_parentLayout);
    
    
    // Boxes in corners
    
    // Center
    ui::Layout* boxCenter = ui::Layout::create();
    boxCenter->setContentSize(Size(200,200));
    boxCenter->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    boxCenter->setBackGroundColor(Color3B(255, 0, 0));
    
    ui::RelativeLayoutParameter* relativeLayoutParam = ui::RelativeLayoutParameter::create();
    relativeLayoutParam->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    boxCenter->setLayoutParameter(relativeLayoutParam);
    _parentLayout->addChild(boxCenter);
    
    // Top left
    ui::Layout* boxTopLeft = ui::Layout::create();
    boxTopLeft->setContentSize(Size(200,200));
    boxTopLeft->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    boxTopLeft->setBackGroundColor(Color3B(0, 255, 0));
    
    relativeLayoutParam = ui::RelativeLayoutParameter::create();
    relativeLayoutParam->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    boxTopLeft->setLayoutParameter(relativeLayoutParam);
    _parentLayout->addChild(boxTopLeft);
    
    // Bottom right
    ui::Layout* boxBottomRight = ui::Layout::create();
    boxBottomRight->setContentSize(Size(200,200));
    boxBottomRight->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    boxBottomRight->setBackGroundColor(Color3B(0, 0, 255));
    
    relativeLayoutParam = ui::RelativeLayoutParameter::create();
    relativeLayoutParam->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    boxBottomRight->setLayoutParameter(relativeLayoutParam);
    _parentLayout->addChild(boxBottomRight);
    
  
    return true;
}
    
void HelloWorldScene::onEnter()
{
    Super::onEnter();
    
    // Register for window changes
    auto director = cocos2d::Director::getInstance();
    _windowChangedEvent = director->getEventDispatcher()->addCustomEventListener(AppDelegate::EVENT_WINDOW_SIZE_CHANGED,
                                                                                 std::bind(&HelloWorldScene::onWindowChanged, this, std::placeholders::_1));
    _windowChangedEvent->retain();
}

void HelloWorldScene::onExit()
{
    Super::onExit();
    
    // Unregister window changes
    auto director = cocos2d::Director::getInstance();
    director->getEventDispatcher()->removeEventListener(_windowChangedEvent);
    CC_SAFE_RELEASE(_windowChangedEvent);
}

    
void HelloWorldScene::onWindowChanged(cocos2d::EventCustom* event)
{
    cocos2d::Size visibleSize = getContentSize();
    cocos2d::log("onWindowChanged: %f, %f", visibleSize.width, visibleSize.height);
    
    _parentLayout->setContentSize(visibleSize);
}
    

NS_AZOOMEE_CHAT_END
