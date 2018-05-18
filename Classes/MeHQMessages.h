//
//  MeHQMessages.h
//  azoomee2
//
//  Created by Macauley on 14/05/2018.
//

#ifndef MeHQMessages_h
#define MeHQMessages_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class MeHQMessages : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    cocos2d::ui::Layout* _contentLayer = nullptr;
    
protected:
    virtual void onSizeChanged() override;
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(MeHQMessages);
};

NS_AZOOMEE_END

#endif /* MeHQMessages_h */
