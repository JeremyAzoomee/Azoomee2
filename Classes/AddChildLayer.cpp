//
//  AddChildLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 03/07/2018.
//

#include "AddChildLayer.h"

NS_AZOOMEE_BEGIN

bool AddChildLayer::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}
void AddChildLayer::onEnter()
{
    Super::onEnter();
}

void AddChildLayer::setChildCreator(const ChildCreatorRef& childCreator)
{
    _childCreator = childCreator;
}

void AddChildLayer::setDelegate(AddChildSceneDelegate* delegate)
{
    _delegate = delegate;
}

NS_AZOOMEE_END
