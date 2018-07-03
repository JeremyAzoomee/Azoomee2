//
//  AddChildLayer.h
//  azoomee2
//
//  Created by Macauley on 03/07/2018.
//

#ifndef AddChildLayer_h
#define AddChildLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include "ChildCreator.h"
#include "AddChildScene.h"

NS_AZOOMEE_BEGIN

class AddChildLayer : public cocos2d::Layer
{
    typedef cocos2d::Layer Super;
private:
    ChildCreatorRef _childCreator = nullptr;
    AddChildSceneDelegate* _delegate = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void setChildCreator(const ChildCreatorRef& childCreator);
    void setDelegate(AddChildSceneDelegate* delegate);
    
    CREATE_FUNC(AddChildLayer);
    
}

NS_AZOOMEE_END

#endif /* AddChildLayer_h */
