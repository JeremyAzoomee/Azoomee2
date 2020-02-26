//
//  OomeeCarouselButton.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/03/2018.
//

#ifndef OomeeCarouselButton_h
#define OomeeCarouselButton_h

#include "../AzoomeeOomeeMaker.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/LazyLoadingButton.h>
#include "../DataObjects/OomeeFigureData.h"

USING_NS_TZ

NS_AZ_OM_BEGIN

class OomeeCarouselButtonDelegate
{
public:
    virtual void deleteOomee(const std::string& oomeeFilename) = 0;
    virtual void editOomee(const std::string& oomeeFilename) = 0;
    virtual void shareOomee(const std::string& oomeeFilename) = 0;
    virtual void makeAvatar(const std::string& oomeeFilename) = 0;
};

class OomeeCarouselButton : public LazyLoadingButton
{
    typedef LazyLoadingButton Super;
private:
    OomeeCarouselButtonDelegate* _delegate = nullptr;
    
    cocos2d::ui::Button* _deleteButton = nullptr;
    cocos2d::ui::Button* _makeAvatarButton = nullptr;
    cocos2d::ui::Button* _shareButton = nullptr;
    
    cocos2d::Sprite* _innerCircle = nullptr;
    cocos2d::Sprite* _outerCircle = nullptr;
	
	cocos2d::Sprite* _tutInnerCircle = nullptr;
	cocos2d::Sprite* _tutOuterCircle = nullptr;
	
	cocos2d::LayerGradient* _mainCircleColour = nullptr;
	cocos2d::LayerGradient* _highlightCircleColour = nullptr;
    
    bool _inFocus = false;
    std::string _oomeeFileName;
    
    OomeeFigureDataRef _oomeeData = nullptr;
    
    void animateButtonsIn(float duration);
    void animateButtonsOut(float duration);
    
    
public:
    
    virtual bool init() override;
    
    virtual void onEnter() override;
	
    void setOomeeData(const std::string& filename);
    std::string getOomeeFilename() const;
    void setDelegate(OomeeCarouselButtonDelegate* delegate);
    
    void setInFocus(bool inFocus);
    bool getInFocus() const;
	
	void setHighlightCircleColour(const cocos2d::Color4B& startGradColour, const cocos2d::Color4B& endGradColour, const cocos2d::Vec2& gradDir);
	
    void enableHighlight(bool enable);
	void enableTutorialHighlight(bool enable);
    
    CREATE_FUNC(OomeeCarouselButton);
};

NS_AZ_OM_END


#endif /* OomeeCarouselButton_h */
