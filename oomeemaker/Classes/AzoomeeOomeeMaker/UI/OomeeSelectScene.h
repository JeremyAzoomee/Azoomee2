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

NS_AZOOMEE_OM_BEGIN

class OomeeSelectScene : public cocos2d::Scene, public OomeeCarouselButtonDelegate
{
    typedef cocos2d::Scene Super;
private:
    cocos2d::Layer* _contentLayer = nullptr;
    
    std::map<std::string, OomeeFigure*> _createdOomees;
    OomeeCarousel* _oomeeCarousel = nullptr;
    
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    
    void setCarouselData();
    
    CREATE_FUNC(OomeeSelectScene);
    
    static void newOomee();
    
    //Delegate functions
    virtual void editOomee(const std::string& oomeeFileName) override;
    virtual void deleteOomee(const std::string& oomeeFilename) override;
    virtual void shareOomee(const std::string& oomeeFilename) override;
    virtual void makeAvatar(const std::string& oomeeFilename) override;
    
    
};

NS_AZOOMEE_OM_END

#endif /* OomeeSelectScene_h */
