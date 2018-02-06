//
//  IAPFlowController.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 18/01/2018.
//

#include "IAPFlowController.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "RoutePaymentSingleton.h"
#include "DynamicNodeHandler.h"

NS_AZOOMEE_BEGIN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const std::string IAPFlowController::kIAPUpgradeCTAName = "iap_upgrade_android.json";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const std::string IAPFlowController::kIAPUpgradeCTAName = "iap_upgrade_ios.json";
#else
const std::string IAPFlowController::kIAPUpgradeCTAName = "iap_upgrade.json";
#endif
const std::string IAPFlowController::kCoppaPrivacyCTAName = "coppa_privacy_notice.json";
const std::string IAPFlowController::kLearnMoreCTAName = "iap_learn_more.json";

DynamicNodeFlowControllerRef IAPFlowController::create()
{
    return std::make_shared<IAPFlowController>();
}
void IAPFlowController::processAction(const ButtonActionDataRef& actionData)
{
    _actionData = actionData;
    const std::string& fileName = actionData->getParamForKey(kCTAFilenameKey);
    FlowPath pathAction = convertStringToFlowPath(actionData->getParamForKey(kCTAActionKey));
    
    if( fileName == kIAPUpgradeCTAName)
    {
        handleIAPUpgradeFlow(actionData, pathAction);
    }
    else if(fileName == kCoppaPrivacyCTAName)
    {
        handleCoppaPrivacyFlow(actionData, pathAction);
    }
    else if(fileName == kLearnMoreCTAName)
    {
        handleLearnMoreFlow(actionData, pathAction);
    }
}

IAPFlowController::IAPFlowController() noexcept
{
    _flowEntryFile = kIAPUpgradeCTAName;
    _type = FlowType::IAP;
}

void IAPFlowController::handleIAPUpgradeFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
{
    switch(pathAction)
    {
        default: case UNKNOWN:
        {
            return;
            break;
        }
        case NEXT:
        {
            const std::string& path = actionData->getParamForKey("path");
            if(path == kPathIAP || path == kPathRestore)
            {
                startIAP();
            }
            else if(path == kPathCoppa)
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("upgrade_coppaPrivacy");
                DynamicNodeHandler::getInstance()->createDynamicNodeById(kCoppaPrivacyCTAName);
            }
            else if(path == kPathLearnMore)
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("upgrade_learnMore");
                DynamicNodeHandler::getInstance()->createDynamicNodeById(kLearnMoreCTAName);
            }
            break;
        }
        case CLOSE: case BACK:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("upgrade_close");
            exitFlow();
            break;
        }
            
    }
}

void IAPFlowController::handleCoppaPrivacyFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
{
    switch(pathAction)
    {
        default: case UNKNOWN:
        {
            return;
            break;
        }
        case BACK:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("coppaPrivacy_back");
            DynamicNodeHandler::getInstance()->createDynamicNodeById(kIAPUpgradeCTAName);
            break;
        }
        case CLOSE: case NEXT:
        {
            exitFlow();
            break;
        }
            
    }
}

void IAPFlowController::handleLearnMoreFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
{
    switch(pathAction)
    {
        default: case UNKNOWN:
        {
            return;
            break;
        }
        case BACK:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("learnMore_back");
            DynamicNodeHandler::getInstance()->createDynamicNodeById(kIAPUpgradeCTAName);
            break;
        }
        case CLOSE: case NEXT:
        {
            exitFlow();
            break;
        }
            
    }
}

void IAPFlowController::startIAP()
{
    if(ParentDataProvider::getInstance()->isUserLoggedIn())
    {
        AwaitingAdultPinLayer::create()->setDelegate(this);
    }
    else
    {
        AdultPinAccepted(nullptr);
    }
}

void IAPFlowController::AdultPinCancelled(AwaitingAdultPinLayer* layer)
{
}

void IAPFlowController::AdultPinAccepted(AwaitingAdultPinLayer* layer)
{
    if(_actionData)
    {
        const std::string& path = _actionData->getParamForKey("path");
        if(path == kPathRestore)
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("refreshPayment");
            RoutePaymentSingleton::getInstance()->refreshAppleReceiptFromButton();
            exitFlow();
        }
        else if(path == kPathIAP)
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("startIAP");
            RoutePaymentSingleton::getInstance()->startInAppPayment();
            exitFlow();
        }
    }
    
}

NS_AZOOMEE_END
