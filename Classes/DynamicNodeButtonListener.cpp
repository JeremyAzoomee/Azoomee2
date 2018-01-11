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
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "FlowDataSingleton.h"
#include "HQDataProvider.h"


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
            else if(location == _kButtonLocationAddChild)
            {
                addChild();
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
                AnalyticsSingleton::getInstance()->registerCurrentScene("REPLAY");
                AnalyticsSingleton::getInstance()->ctaButtonPressed("replayContent", ContentHistoryManager::getInstance()->getLastOpenedContent()->getTitle());
                DeepLinkingSingleton::getInstance()->setDeepLink(DeepLinkingSingleton::kPostContentDeeplinkStr + ContentHistoryManager::getInstance()->getLastOpenedContent()->getContentItemId()); // fire up content directly when content opener exists
                closeCTAPopup();
            }
            else
            {
                AnalyticsSingleton::getInstance()->registerCurrentScene("RECOMMENDED");
                HQContentItemObjectRef content = HQDataProvider::getInstance()->getItemDataForSpecificItem(location.substr(DeepLinkingSingleton::kPostContentDeeplinkStr.length()));
                std::string contentTitle = "";
                if(content)
                {
                    contentTitle = content->getTitle();
                }
                AnalyticsSingleton::getInstance()->ctaButtonPressed("OpenRecommendedContent", contentTitle);
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
        else if(buttonAction->getType() == _kButtonTypeCTATransitionParams)
        {
            const std::string& location = buttonAction->getParamForKey("location");
            const std::string& params = DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString();
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(location, params);
        }
        else if(buttonAction->getType() == _kButtonTypeCTATransitionGroup)
        {
            const std::string& location = buttonAction->getParamForKey("location");
            DynamicNodeHandler::getInstance()->createDynamicNodeByGroupId(location);
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
        //ModalMessages::getInstance()->startLoading();
        AnalyticsSingleton::getInstance()->registerAzoomeeEmail(email);
        BackEndCaller::getInstance()->registerParent(email, password ,pin);
        DynamicNodeDataInputStorage::getInstance()->clearStorage();
        closeCTAPopup();
    }
}

void DynamicNodeButtonListener::addChild()
{
    const std::string& profileName = trim(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("name"));
    
    int day = std::atoi(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("day").c_str());
    int month = std::atoi(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("month").c_str());
    int year = std::atoi(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("year").c_str());
    
    const std::string& DOB = StringUtils::format("%04d-%02d-%02d",year,month,day);
    const std::string& gender = "MALE";
    if(!isDate(day, month, year))
    {
        return;
    }
    auto backEndCaller = BackEndCaller::getInstance();
    if((FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow()) && ParentDataProvider::getInstance()->getAmountOfAvailableChildren() !=0)
    {
        backEndCaller->updateChild(ParentDataProvider::getInstance()->getIDForAvailableChildren(0), profileName, gender, DOB, 0);
    }
    else
    {
        backEndCaller->registerChild(profileName, gender, DOB, 0);
    }
    DynamicNodeDataInputStorage::getInstance()->clearStorage();
    closeCTAPopup();
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
