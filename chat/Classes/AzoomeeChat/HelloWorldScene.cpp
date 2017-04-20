#include "HelloWorldScene.h"


namespace Azoomee { namespace Chat
{

bool HelloWorldScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    cocos2d::Size visibleSize = getContentSize();
    
    cocos2d::log( "visibleSize: %f, %f", visibleSize.width, visibleSize.height );
    
    Sprite* spriteButton = Sprite::create("res/slideshow/slide_1.jpg");
    spriteButton->setPosition(visibleSize.width / 2,
                              visibleSize.height / 2);
    addChild(spriteButton);
    
    
    
    // Create the layout
    Layout* layout = Layout::create();
    layout->setContentSize(Size(280, 150));
    layout->setPosition(visibleSize.width / 2,
                        visibleSize.height / 2);
    layout->setBackGroundColorType(BackGroundColorType::SOLID);
    layout->setBackGroundColor(Color3B(128, 128, 128));
    addChild(layout);
  
    return true;
}

}}
