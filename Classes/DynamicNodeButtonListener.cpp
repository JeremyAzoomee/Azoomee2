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

void DynamicNodeButtonListener::onButtonPressedCallFunc(Ref* button, ui::Widget::TouchEventType eventType, ButtonActionDataRef buttonAction)
{
    if(eventType == ui::Widget::TouchEventType::ENDED)
    {
        if(buttonAction->getType() == _kButtonTypeInternal)
        {
            const std::string& location = buttonAction->getParamForKey("location");
            if(location == _kButtonLocationUpgrade)
            {
                upgradeButtonPressed();
            }
            else
            {
                closeCTAPopup();
            }
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
    closeCTAPopup();
}

void DynamicNodeButtonListener::closeCTAPopup()
{
    DynamicNodeCreator::getInstance()->resetCTAPopup();
}


NS_AZOOMEE_END