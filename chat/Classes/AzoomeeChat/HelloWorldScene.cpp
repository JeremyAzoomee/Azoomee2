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
    
    Sprite* spriteButton = Sprite::create("res/login/next_btn.png");
    spriteButton->setPosition(visibleSize.width / 2,
                              visibleSize.height / 2);
    addChild(spriteButton);
  
    return true;
}

}}
