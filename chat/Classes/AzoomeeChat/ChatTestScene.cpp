#include "ChatTestScene.h"
#include "ChatAPI.h"

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN
    
ChatTestScene::~ChatTestScene()
{
    CC_SAFE_RELEASE(_parentLayout);
}

bool ChatTestScene::init()
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
    
    
    
  
    return true;
}
    
void ChatTestScene::onEnter()
{
    Super::onEnter();
    
    ChatAPI::getInstance()->loginUser("mitch@azoomee.com", "test1234");
}

void ChatTestScene::onExit()
{
    Super::onExit();
}
    

NS_AZOOMEE_CHAT_END
