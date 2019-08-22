//
//  HQPage.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/08/2019.
//

#include "HQPage.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool HQPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}
void HQPage::onEnter()
{
    Super::onEnter();
}
void HQPage::onExit()
{
    Super::onExit();
}
void HQPage::onSizeChanged()
{
    Super::onSizeChanged();
}

void HQPage::setContentSelectedCallback(const ContentSelectedCallback& callback)
{
    _contentSceletedCallback = callback;
}

NS_AZOOMEE_END
