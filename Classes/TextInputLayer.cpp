#include "TextInputLayer.h"

ui::EditBox* TextInputLayer::create(Size inputBoxSize, int textInputType)
{
    Rect spriteRect = Rect(0, 0, 736, 131);
    Rect capInsents = Rect(60, 65, 616, 1);
    editBoxArea = ui::Scale9Sprite::create("res/login/textarea_bg.png", spriteRect, capInsents);
    
    editBox = ui::EditBox::create(inputBoxSize, editBoxArea);
    
    return editBox;
}
