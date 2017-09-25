//
//  DynamicNodeButtonListener.cpp
//  azoomee2
//
//  Created by Macauley on 25/09/2017.
//
//

#include "DynamicNodeButtonListener.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "IAPUpsaleLayer.h"
#include "PreviewLoginSignupMessageBox.h"
#include "DynamicNodeCreator.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<DynamicNodeButtonListener> sDynamicNodeButtonListenerSharedInstance;

DynamicNodeButtonListener* DynamicNodeButtonListener::getInstance()
{
    if(!sDynamicNodeButtonListenerSharedInstance.get())
    {
        sDynamicNodeButtonListenerSharedInstance.reset(new DynamicNodeButtonListener());
    }
    return sDynamicNodeButtonListenerSharedInstance.get();
}

DynamicNodeButtonListener::~DynamicNodeButtonListener(void)
{
    
}

bool DynamicNodeButtonListener::init(void)
{
    return true;
}

void DynamicNodeButtonListener::onButtonPressedCallFunc(Ref* button, ui::Widget::TouchEventType evenType, ButtonActionDataRef buttonAction)
{
    if(buttonAction->getType() == "internal")
    {
        std::string location = buttonAction->getParamForKey("location");
        if(location == "showUpgrade")
        {
            upgradeButtonPressed();
        }
        else
        {
            Director::getInstance()->getScheduler()->schedule(schedule_selector(DynamicNodeButtonListener::closeCTAPopup), this, 0, 0, 0.2, false);
        }
    }
    
}

void DynamicNodeButtonListener::upgradeButtonPressed()
{
    if(ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        
        IAPUpsaleLayer::createRequiresPin();
    }
    else
    {
        PreviewLoginSignupMessageBox::create();
    }
    Director::getInstance()->getScheduler()->schedule(schedule_selector(DynamicNodeButtonListener::closeCTAPopup), this, 0, 0, 0.2, false);
}

void DynamicNodeButtonListener::closeCTAPopup(float dt)
{
    DynamicNodeCreator::getInstance()->resetCTAPopup();
}


NS_AZOOMEE_END
