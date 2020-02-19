//
//  OomeeSelectScene.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 21/02/2018.
//

#ifndef OomeeSelectScene_h
#define OomeeSelectScene_h

#include "../AzoomeeOomeeMaker.h"
#include "OomeeFigure.h"
#include "OomeeCarousel.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "../../ConfirmCancelMessageBox.h"

USING_NS_TZ

NS_AZ_OM_BEGIN

class OomeeSelectScene : public cocos2d::Scene, public OomeeCarouselButtonDelegate
{
    typedef cocos2d::Scene Super;
private:
    cocos2d::Layer* _contentLayer = nullptr;
    
    OomeeCarousel* _oomeeCarousel = nullptr;
    cocos2d::ui::Button* _newOomeeButton = nullptr;
	cocos2d::ui::Button* _exitButton = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;
	
    void setCarouselData();
    
    void setCarouselCenterTarget(const std::string& oomeeFilename);
    
    CREATE_FUNC(OomeeSelectScene);
    
    static void newOomee();
    
    void toggleMakeAvatarHiglight();
    
    //Delegate functions
    virtual void editOomee(const std::string& oomeeFileName) override;
    virtual void deleteOomee(const std::string& oomeeFilename) override;
    virtual void shareOomee(const std::string& oomeeFilename) override;
    virtual void makeAvatar(const std::string& oomeeFilename) override;
    
};

NS_AZ_OM_END

#endif /* OomeeSelectScene_h */
