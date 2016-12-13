#ifndef __DROPDOWNMENU_H__
#define __DROPDOWNMENU_H__

#include "cocos2d.h"
#include "ui/UIButton.h"

class DropdownMenu: public cocos2d::ui::Button
{
public:

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(DropdownMenu);
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    void setPrompt(std::string promptText);
};

#endif // __DROPDOWNMENU_H__
