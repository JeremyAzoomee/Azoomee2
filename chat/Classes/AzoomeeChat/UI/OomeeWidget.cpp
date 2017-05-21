#include "OomeeWidget.h"
#include <AzoomeeCommon/UI/Style.h>


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

bool OomeeWidget::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Style::Color::kermitGreenTwo);
  
    return true;
}

void OomeeWidget::onEnter()
{
    Super::onEnter();
}

void OomeeWidget::onExit()
{
    Super::onExit();
}


NS_AZOOMEE_CHAT_END
