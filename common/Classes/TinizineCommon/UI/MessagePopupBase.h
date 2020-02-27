//
//  MessagePopupBase.h
//  TinizineCommon
//
//  Created by Macauley.Scoffins on 06/02/2020.
//

#ifndef MessagePopupBase_h
#define MessagePopupBase_h

#include "../Tinizine.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_TZ_BEGIN

class MessagePopupBase : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
protected:
    typedef std::function<void(MessagePopupBase*)> ButtonPressedCallback;
    cocos2d::ui::Layout* _messageBoxLayout = nullptr;
    
public:
    
    bool init() override;
};

NS_TZ_END

#endif /* MessagePopupBase_h */
