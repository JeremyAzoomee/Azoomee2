//
//  DynamicNodeButtonListener.cpp
//  azoomee2
//
//  Created by Macauley on 25/09/2017.
//
//

#include "DynamicNodeButtonListener.h"
#include "DynamicNodeHandler.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "IAPUpsaleLayer.h"
#include "PreviewLoginSignupMessageBox.h"
#include "DynamicNodeCreator.h"
#include "SceneManagerScene.h"
#include "DeepLinkingSingleton.h"
#include "ContentHistoryManager.h"
#include "DynamicNodeDataInputStorage.h"
#include <AzoomeeCommon/Data/Urls.h>
#include <AzoomeeCommon/UI/ModalWebview.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Input/TextInputChecker.h>
#include "RoutePaymentSingleton.h"

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
    _buttonAction = buttonAction;
    
    if(eventType == ui::Widget::TouchEventType::ENDED)
    {
        if(buttonAction->getType() == _kButtonTypeInternal)
        {
            const std::string& location = buttonAction->getParamForKey("location");
            if(location == _kButtonLocationUpgrade)
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("upgrade");
                upgradeButtonPressed();
            }
            else if(location == _kButtonLocationSignUp)
            {
                signUp();
            }
            else if(location == _kButtonLocationIAP)
            {
                inAppPurchaseButtonPressed();
            }
            else
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("close");
                closeCTAPopup();
            }
        }
        else if(buttonAction->getType() == _kButtonTypeDeepLink)
        {
            const std::string& location = buttonAction->getParamForKey("location");
            if(location == "replay")
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("replayContent");
                DeepLinkingSingleton::getInstance()->setDeepLink(DeepLinkingSingleton::kPostContentDeeplinkStr + ContentHistoryManager::getInstance()->getLastOpenedContent()->getContentItemId()); // fire up content directly when content opener exists
                closeCTAPopup();
            }
            else
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("OpenRecommendedContent");
                DeepLinkingSingleton::getInstance()->setDeepLink(location);
                closeCTAPopup();
            }
        }
        else if(buttonAction->getType() == _kButtonTypeWeb)
        {
            const std::string& location = buttonAction->getParamForKey("location");
            if(location == "privacyPolicy")
            {
                ModalWebview::createWithURL(Url::PrivacyPolicy);
            }
            else if(location == "termsOfUse")
            {
                ModalWebview::createWithURL(Url::TermsOfUse);
            }
            else if(location != "")
            {
                ModalWebview::createWithURL(location);
            }
        }
        else if(buttonAction->getType() == _kButtonTypeCTATransition)
        {
            const std::string& location = buttonAction->getParamForKey("location");
            DynamicNodeHandler::getInstance()->createDynamicNodeById(location);
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
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Onboarding));
    }
    closeCTAPopup();
}

void DynamicNodeButtonListener::closeCTAPopup()
{
    DynamicNodeCreator::getInstance()->resetCTAPopup();
}

void DynamicNodeButtonListener::signUp()
{
    const std::string& email = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("email");
    const std::string& password = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("password");
    const std::string& pin = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("pin");
    if(isValidPin(pin.c_str()) && isValidPassword(password.c_str(), 6) && isValidEmailAddress(email.c_str()))
    {
        ModalMessages::getInstance()->startLoading();
        AnalyticsSingleton::getInstance()->registerAzoomeeEmail(email);
        BackEndCaller::getInstance()->registerParent(email, password ,pin);
        DynamicNodeDataInputStorage::getInstance()->clearStorage();
    }
}

void DynamicNodeButtonListener::inAppPurchaseButtonPressed()
{
    if(!ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
    {
        AwaitingAdultPinLayer::create()->setDelegate(this);
    }
    else
    {
        AdultPinAccepted(nullptr);
    }
}

void DynamicNodeButtonListener::AdultPinCancelled(AwaitingAdultPinLayer* layer)
{
}

void DynamicNodeButtonListener::AdultPinAccepted(AwaitingAdultPinLayer* layer)
{
    if(_buttonAction)
    {
        if(_buttonAction->getType() == _kButtonTypeInternal)
        {
            const std::string& location = _buttonAction->getParamForKey("location");
            if(location == _kButtonLocationIAP)
            {
                const std::string& action = _buttonAction->getParamForKey("action");
                if(action == _kButtonActionRestorePurchase)
                {
                    RoutePaymentSingleton::getInstance()->refreshAppleReceiptFromButton();
                }
                else if(action == _kButtonActionStartPayment)
                {
                    RoutePaymentSingleton::getInstance()->startInAppPayment();
                }
            }
        }
    }
        
}

NS_AZOOMEE_END
