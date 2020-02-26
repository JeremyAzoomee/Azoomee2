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

USING_NS_TZ

NS_AZ_OM_BEGIN

class OomeeCarousel : public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    static const std::string kNewOomeeTag;
    
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
	
	std::vector<std::string> getOomeeData() const;
	
    void centerButtons();
    void moveCarouselLeft();
    void moveCarouselRight();
    
    void centerOnOomee(const std::string& targetOomee);
    
    void setButtonDelegate(OomeeCarouselButtonDelegate* delegate);
    OomeeCarouselButton* getCenterButton();
    
    CREATE_FUNC(OomeeCarousel);
};

NS_AZ_OM_END

#endif /* OomeeCarousel_h */
