//
//  IAPFlowController.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 18/01/2018.
//

#include "IAPFlowController.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
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
    if( fileName == kIAPUpgradeCTAName)
    {
        handleIAPUpgradeFlow(actionData);
    }
    else if(fileName == kCoppaPrivacyCTAName)
    {
        handleCoppaPrivacyFlow(actionData);
    }
    else if(fileName == kLearnMoreCTAName)
    {
        handleLearnMoreFlow(actionData);
    }
}

IAPFlowController::IAPFlowController() noexcept
{
    _flowEntryFile = kIAPUpgradeCTAName;
    _type = FlowType::IAP;
}

void IAPFlowController::handleIAPUpgradeFlow(const ButtonActionDataRef& actionData)
{
    FlowPath pathAction = convertStringToFlowPath(actionData->getParamForKey(kCTAActionKey));
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
                DynamicNodeHandler::getInstance()->createDynamicNodeById(kCoppaPrivacyCTAName);
            }
            else if(path == kPathLearnMore)
            {
                DynamicNodeHandler::getInstance()->createDynamicNodeById(kLearnMoreCTAName);
            }
            break;
        }
        case CLOSE: case BACK:
        {
            exitFlow();
            break;
        }
            
    }
}

void IAPFlowController::handleCoppaPrivacyFlow(const ButtonActionDataRef& actionData)
{
    FlowPath pathAction = convertStringToFlowPath(actionData->getParamForKey(kCTAActionKey));
    switch(pathAction)
    {
        default: case UNKNOWN:
        {
            return;
            break;
        }
        case BACK:
        {
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

void IAPFlowController::handleLearnMoreFlow(const ButtonActionDataRef& actionData)
{
    FlowPath pathAction = convertStringToFlowPath(actionData->getParamForKey(kCTAActionKey));
    switch(pathAction)
    {
        default: case UNKNOWN:
        {
            return;
            break;
        }
        case BACK:
        {
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
    if(!ParentDataProvider::getInstance()->isNoUserLoggedIn())
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
            RoutePaymentSingleton::getInstance()->refreshAppleReceiptFromButton();
            exitFlow();
        }
        else if(path == kPathIAP)
        {
            RoutePaymentSingleton::getInstance()->startInAppPayment();
            exitFlow();
        }
    }
    
}

NS_AZOOMEE_END
