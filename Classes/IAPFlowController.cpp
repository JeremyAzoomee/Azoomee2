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
#include <cocos/deprecated/CCString.h>

NS_AZOOMEE_BEGIN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const std::string IAPFlowController::kIAPUpgradeCTAName = "iap_upgrade_android";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const std::string IAPFlowController::kIAPUpgradeCTAName = "iap_upgrade_ios";
#else
const std::string IAPFlowController::kIAPUpgradeCTAName = "iap_upgrade";
#endif
const std::string IAPFlowController::kCoppaPrivacyCTAName = "coppa_privacy_notice.json";
const std::string IAPFlowController::kLearnMoreCTAName = "iap_learn_more.json";
const std::string IAPFlowController::kAgeGateCTAName = "iap_age_gate.json";

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
    else if(fileName == kAgeGateCTAName)
    {
        handleAgeGateFlow(actionData, pathAction);
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
        case CLOSE:
        case BACK:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("upgrade_close");
            exitFlow();
            break;
        }
        default:
        case UNKNOWN:
        {
            return;
            break;
        }
    }
}

void IAPFlowController::handleCoppaPrivacyFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
{
    switch(pathAction)
    {
        case BACK:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("coppaPrivacy_back");
            DynamicNodeHandler::getInstance()->createDynamicNodeByGroupIdWithParams(kIAPUpgradeCTAName, getJSONStringFromMap({
                {"iapPrice",IAPProductDataHandler::getInstance()->getHumanReadableProductPrice()}
            }));
            break;
        }
        case CLOSE:
        case NEXT:
        {
            exitFlow();
            break;
        }
        default:
        case UNKNOWN:
        {
            return;
            break;
        }
    }
}

void IAPFlowController::handleLearnMoreFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
{
    switch(pathAction)
    {
        case BACK:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("learnMore_back");
            DynamicNodeHandler::getInstance()->createDynamicNodeByGroupIdWithParams(kIAPUpgradeCTAName, getJSONStringFromMap({
                {"iapPrice",IAPProductDataHandler::getInstance()->getHumanReadableProductPrice()}
            }));
            break;
        }
        case CLOSE:
        case NEXT:
        {
            exitFlow();
            break;
        }
        default:
        case UNKNOWN:
        {
            return;
            break;
        }
    }
}

void IAPFlowController::handleAgeGateFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
{
    switch(pathAction)
    {
        case NEXT:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("ageGate_next");
            if(actionData->getParamForKey("value") == cocos2d::StringUtils::format("%d",_targetVal))
            {
                AdultPinAccepted(nullptr);
            }
            else
            {
                startAgeGate();
            }
            break;
        }
        case CLOSE:
        case BACK:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("ageGate_back");
            DynamicNodeHandler::getInstance()->createDynamicNodeByGroupIdWithParams(kIAPUpgradeCTAName, getJSONStringFromMap({
                {"iapPrice",IAPProductDataHandler::getInstance()->getHumanReadableProductPrice()}
            }));
            break;
        }
        default:
        case UNKNOWN:
        {
            return;
            break;
        }
    }
}

void IAPFlowController::startAgeGate()
{
    const int val1Set[4] = {4,5,6,7};
    const int val2Set[3] = {3,4,5};
    
    srand(time(NULL));
    
    int val1 = val1Set[rand() % 4];
    int val2 = val2Set[rand() % 3];
    _targetVal =  val1 * val2;
    
    int randAns1 = val1Set[rand() % 4] * val2Set[rand() % 3];
    while(randAns1 == _targetVal)
    {
        randAns1 = val1Set[rand() % 4] * val2Set[rand() % 3];
    }
    int randAns2 = val1Set[rand() % 4] * val2Set[rand() % 3];
    while(randAns2 == _targetVal || randAns2 == randAns1)
    {
        randAns2 = val1Set[rand() % 4] * val2Set[rand() % 3];
    }
    
    std::vector<int> ans = {_targetVal, randAns1, randAns2};
    std::random_shuffle(ans.begin(), ans.end());
    
    const std::map<std::string,std::string>& paramMap = {
        {"val1",cocos2d::StringUtils::format("%d",val1)},
        {"val2",cocos2d::StringUtils::format("%d",val2)},
        {"ans1",cocos2d::StringUtils::format("%d",ans[0])},
        {"ans2",cocos2d::StringUtils::format("%d",ans[1])},
        {"ans3",cocos2d::StringUtils::format("%d",ans[2])},
        {"path",_actionData->getParamForKey("path")}
    };
    AnalyticsSingleton::getInstance()->ctaButtonPressed("startAgeGate");
    DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(kAgeGateCTAName, getJSONStringFromMap(paramMap));
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
        startAgeGate();
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
