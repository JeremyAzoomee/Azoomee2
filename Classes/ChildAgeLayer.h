//
//  ChildAgeLayer.h
//  azoomee2
//
//  Created by Macauley on 03/07/2018.
//

#ifndef ChildAgeLayer_h
#define ChildAgeLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include "AddChildLayer.h"

NS_AZOOMEE_BEGIN

class ChildAgeLayer : public AddChildLayer
{
    typedef cocos2d::Layer Super;
private:
    cocos2d::ui::Button* _continueButton = nullptr;
	
	std::vector<cocos2d::ui::Button*> _ageButtons;
	
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(ChildAgeLayer);
    
};

NS_AZOOMEE_END

#endif /* ChildAgeLayer_h */
