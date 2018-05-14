//
//  MeHQGallery.h
//  azoomee2
//
//  Created by Macauley on 14/05/2018.
//

#ifndef MeHQGallery_h
#define MeHQGallery_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/UI/CocosGUI.h>

NS_AZOOMEE_BEGIN

class MeHQGallery : public cocos2d::ui::Layout
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
    
    CREATE_FUNC(MeHQGallery);
};

NS_AZOOMEE_END

#endif /* MeHQGallery_h */
