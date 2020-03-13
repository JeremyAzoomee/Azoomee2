#ifndef AzoomeeChat_ChatTextField_h
#define AzoomeeChat_ChatTextField_h

#include "../AzoomeeChat.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/Tinizine.h>


USING_NS_TZ

NS_AZ_CHAT_BEGIN
    
class ChatTextField : public cocos2d::ui::TextField
{
    typedef cocos2d::ui::TextField Super;
public:
    
    static ChatTextField* create();
    static ChatTextField* create(const std::string& placeholder,
                                 const std::string& fontName,
                                 int fontSize);
    
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;
//    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;
//    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unusedEvent) override;
};

NS_AZ_CHAT_END

#endif
