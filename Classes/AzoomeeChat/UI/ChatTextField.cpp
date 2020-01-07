#include "ChatTextField.h"


using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

ChatTextField* ChatTextField::create()
{
    ChatTextField* widget = new (std::nothrow) ChatTextField();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

ChatTextField* ChatTextField::create(const std::string &placeholder, const std::string &fontName, int fontSize)
{
    ChatTextField* widget = new (std::nothrow) ChatTextField();
    if (widget && widget->init())
    {
        widget->setFontName(fontName);
        widget->setFontSize(fontSize);
        widget->setPlaceHolder(placeholder);
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool ChatTextField::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent)
{
    // By pass TextField::onTouchBegan
    bool pass = Widget::onTouchBegan(touch, unusedEvent);
    
    // The below is the same as TextField::onTouchBegan
    // but we don't call didNotSelectSelf if it didn't hit.
    if(_hitted)
    {
        if(isFocusEnabled())
        {
            requestFocus();
        }
        
        _textFieldRenderer->attachWithIME();
    }
    return pass;
}

//void ChatTextField::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unusedEvent)
//{
//    ;
//}
//
//void ChatTextField::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unusedEvent)
//{
//    ;
//}

NS_AZOOMEE_CHAT_END
