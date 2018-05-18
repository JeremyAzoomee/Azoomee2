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
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class MeHQGallery : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    static const float kSideMarginSize;
    static const float kSpaceAboveCarousel;
    static const int kUnitsOnScreen;
    static const float kContentItemMargin;
    
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
