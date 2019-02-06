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
#include <AzoomeeCommon/UI/ConfirmCancelMessageBox.h>

NS_AZOOMEE_OM_BEGIN

class OomeeSelectScene : public cocos2d::Scene, public OomeeCarouselButtonDelegate, public ConfirmCancelMessageBoxDelegate
{
    typedef cocos2d::Scene Super;
private:
    cocos2d::Layer* _contentLayer = nullptr;
    
    OomeeCarousel* _oomeeCarousel = nullptr;
    cocos2d::ui::Button* _newOomeeButton = nullptr;
    
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    
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
    
    virtual void onConfirmPressed(ConfirmCancelMessageBox* pSender) override;
    virtual void onCancelPressed(ConfirmCancelMessageBox* pSender) override;
    
    
};

NS_AZOOMEE_OM_END

#endif /* OomeeSelectScene_h */
