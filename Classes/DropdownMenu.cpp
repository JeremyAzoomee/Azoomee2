#include "DropdownMenu.h"
#include "ui/UIScrollView.h"

USING_NS_CC;

bool DropdownMenu::init()
{
    if ( !ui::Button::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    return true;
}

void DropdownMenu::setPrompt(std::string promptText)
{
    this->setTitleFontSize(90);
    this->setTitleText(promptText);
    
    this->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                break;
            default:
                break;
        }
    });
}
