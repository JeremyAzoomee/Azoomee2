//
//  MeHQ.h
//  azoomee2
//
//  Created by Macauley on 22/03/2018.
//

#ifndef MeHQ_h
#define MeHQ_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "HQSceneArtsApp.h"

NS_AZOOMEE_BEGIN

class MeHQ : public cocos2d::Layer
{
    typedef cocos2d::Layer Super;
private:
    cocos2d::ui::Button* _oomeeMakerButton = nullptr;
    HQSceneArtsApp* _artScrollView = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(MeHQ);
};

NS_AZOOMEE_END


#endif /* MeHQ_h */
