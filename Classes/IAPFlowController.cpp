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

const std::string IAPFlowController::kIAPUpgradeCTAName = "iap_upgrade.json";
const std::string IAPFlowController::kCoppaPrivacyCTAName = "coppa_privacy_notice.json";
const std::string IAPFlowController::kLearnMoreCTAName = "iap_learn_more.json";

DynamicNodeFlowControllerRef IAPFlowController::create()
{
    return std::make_shared<IAPFlowController>();
}
void IAPFlowController::processAction(ButtonActionDataRef actionData)
{
    _actionData = actionData;
    const std::string& fileName = actionData->getParamForKey(_kCTAFilenameKey);
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

void IAPFlowController::handleIAPUpgradeFlow(ButtonActionDataRef actionData)
{
    FlowPath pathAction = convertStringToFlowPath(actionData->getParamForKey(_kCTAActionKey));
    switch(pathAction)
    {
        case UNKNOWN:
        {
            return;
            break;
        }
        case NEXT:
        {
            const std::string& path = actionData->getParamForKey("path");
            if(path == _kPathIAP || path == _kPathRestore)
            {
                startIAP();
            }
            else if(path == _kPathCoppa)
            {
                DynamicNodeHandler::getInstance()->createDynamicNodeById(kCoppaPrivacyCTAName);
            }
            else if(path == _kPathLearnMore)
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

void IAPFlowController::handleCoppaPrivacyFlow(ButtonActionDataRef actionData)
{
    FlowPath pathAction = convertStringToFlowPath(actionData->getParamForKey(_kCTAActionKey));
    switch(pathAction)
    {
        case UNKNOWN:
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

void IAPFlowController::handleLearnMoreFlow(ButtonActionDataRef actionData)
{
    FlowPath pathAction = convertStringToFlowPath(actionData->getParamForKey(_kCTAActionKey));
    switch(pathAction)
    {
        case UNKNOWN:
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
    if(!ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
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
        if(path == _kPathRestore)
        {
            RoutePaymentSingleton::getInstance()->refreshAppleReceiptFromButton();
        }
        else if(path == _kPathIAP)
        {
            RoutePaymentSingleton::getInstance()->startInAppPayment();
        }
    }
    
}

NS_AZOOMEE_END
