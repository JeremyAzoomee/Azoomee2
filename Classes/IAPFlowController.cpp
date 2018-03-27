//
//  IAPFlowController.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 18/01/2018.
//

#include "IAPFlowController.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "RoutePaymentSingleton.h"
#include "DynamicNodeHandler.h"
#include "IAPProductDataHandler.h"

NS_AZOOMEE_BEGIN

const std::string IAPFlowController::kIAPUpgradeCTAName = "iap_upgrade_";
const std::string IAPFlowController::kCoppaPrivacyCTAName = "coppa_privacy_notice.json";
const std::string IAPFlowController::kLearnMoreCTAName = "iap_learn_more.json";

DynamicNodeFlowControllerRef IAPFlowController::createWithContext(const std::string& context)
{
   return std::make_shared<IAPFlowController>(context);
}

DynamicNodeFlowControllerRef IAPFlowController::create()
{
    return std::make_shared<IAPFlowController>();
}

void IAPFlowController::processAction(const ButtonActionDataRef& actionData)
{
    _actionData = actionData;
    const std::string& fileName = actionData->getParamForKey(kCTAFilenameKey);
    FlowPath pathAction = convertStringToFlowPath(actionData->getParamForKey(kCTAActionKey));
    
    if( fileName == kIAPUpgradeCTAName + _contextExtention)
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

IAPFlowController::IAPFlowController(const std::string& context) noexcept
{
    _contextExtention = context;
    _flowEntryFile = kIAPUpgradeCTAName + context;
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
            DynamicNodeHandler::getInstance()->createDynamicNodeByGroupIdWithParams(kIAPUpgradeCTAName + _contextExtention, getJSONStringFromMap({
                {"iapPrice",IAPProductDataHandler::getInstance()->getHumanReadableProductPrice()}
            }));
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
            DynamicNodeHandler::getInstance()->createDynamicNodeByGroupIdWithParams(kIAPUpgradeCTAName + _contextExtention, getJSONStringFromMap({
                {"iapPrice",IAPProductDataHandler::getInstance()->getHumanReadableProductPrice()}
            }));
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
        AwaitingAdultPinLayer* pinLayer = AwaitingAdultPinLayer::create();
        pinLayer->setDelegate(this);
        pinLayer->setPinIsForPayment(true);
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
