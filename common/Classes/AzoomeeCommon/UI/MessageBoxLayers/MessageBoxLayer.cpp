#include "MessageBoxLayer.h"

namespace Azoomee
{
    
void MessageBoxLayer::setCurrentRunningSceneSize()
{
    currentRunningSceneSize = cocos2d::Director::getInstance()->getRunningScene()->getContentSize();
}
    
void MessageBoxLayer::onSizeChanged()
{
    
}
  
}
