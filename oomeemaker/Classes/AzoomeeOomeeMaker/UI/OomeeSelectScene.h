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
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_OM_BEGIN

class OomeeSelectScene : public cocos2d::Scene
{
    typedef cocos2d::Scene Super;
private:
    cocos2d::Layer* _contentLayer = nullptr;
    
    std::map<std::string, OomeeFigure*> _createdOomees;
    cocos2d::ui::ListView* _carousel;
    
    
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    
    CREATE_FUNC(OomeeSelectScene);
    
    static void editOomee(const std::string& oomeeFileName);
    static void newOomee();
};

NS_AZOOMEE_OM_END

#endif /* OomeeSelectScene_h */
