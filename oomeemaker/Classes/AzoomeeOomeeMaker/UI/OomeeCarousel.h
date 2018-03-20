//
//  OomeeCarousel.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 27/02/2018.
//

#ifndef OomeeCarousel_h
#define OomeeCarousel_h

#include "../AzoomeeOomeeMaker.h"
#include "OomeeCarouselButton.h"
#include <AzoomeeCommon/UI/LazyLoadingButton.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_OM_BEGIN

class OomeeCarousel : public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    std::vector<std::string> _oomeeData;
    std::vector<LazyLoadingButton*> _carouselButtons;
    int _currentIndex = 0;
    int _visibleRange = 3;
    float _spacing = 0.0f;
    
    OomeeCarouselButtonDelegate* _buttonDelegate = nullptr;
    
    cocos2d::ClippingRectangleNode* _contentNode = nullptr;
    
    LazyLoadingButton* _leftMostButton = nullptr;
    LazyLoadingButton* _rightMostButton = nullptr;
    LazyLoadingButton* _centerButton = nullptr;
    
    void setTouchListener();
    
    
    void rotateButtonsLeft();
    void rotateButtonsRight();
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    virtual void update(float deltaT) override;
    
    void setOomeeData(const std::vector<std::string>& oomeeFilenames);
    void setVisibleRange(int visibleRange);
    
    void centerButtons();
    void setButtonDelegate(OomeeCarouselButtonDelegate* delegate);
    
    CREATE_FUNC(OomeeCarousel);
};

NS_AZOOMEE_OM_END

#endif /* OomeeCarousel_h */
