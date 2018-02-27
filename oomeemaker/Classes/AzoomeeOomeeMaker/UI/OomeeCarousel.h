//
//  OomeeCarousel.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 27/02/2018.
//

#ifndef OomeeCarousel_h
#define OomeeCarousel_h

#include "../AzoomeeOomeeMaker.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_OM_BEGIN

class OomeeCarousel : public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    std::vector<std::string> _oomeeData;
    std::vector<cocos2d::ui::Button*> _carouselButtons;
    int _currentIndex = 0;
    int _visibleRange = 3;
    
    cocos2d::ClippingRectangleNode* _contentNode = nullptr;
    
    cocos2d::ui::Button* _leftMostButton = nullptr;
    cocos2d::ui::Button* _rightMostButton = nullptr;
    
    void setTouchListener();
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    void setOomeeData(const std::vector<std::string>& oomeeFilenames);
    void setVisibleRange(int visibleRange);
    
    
    CREATE_FUNC(OomeeCarousel);
};

NS_AZOOMEE_OM_END

#endif /* OomeeCarousel_h */
