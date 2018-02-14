//
//  SignUpFlowController.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 17/01/2018.
//

#include "SignUpFlowController.h"
#include "DynamicNodeHandler.h"
#include "DynamicNodeDataInputStorage.h"
#include "BackEndCaller.h"
#include "RoutePaymentSingleton.h"
#include <AzoomeeCommon/Input/TextInputChecker.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "SceneManagerScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string SignUpFlowController::kEnterEmailCTAName = "signup_email.json";
const std::string SignUpFlowController::kConfirmEmailCTAName = "signup_email_confirm.json";
const std::string SignUpFlowController::kEnterPasswordCTAName = "signup_password.json";
const std::string SignUpFlowController::kEnterPinCTAName = "signup_pin.json";

SignUpFlowController::SignUpFlowController() noexcept
{
    _flowEntryFile = kEnterEmailCTAName;
    _type = FlowType::SIGNUP;
}

DynamicNodeFlowControllerRef SignUpFlowController::create()
{
    return std::make_shared<SignUpFlowController>();
}

void SignUpFlowController::processAction(const ButtonActionDataRef& actionData)
{
    const std::string& fileName = actionData->getParamForKey(kCTAFilenameKey);
    FlowPath pathAction = convertStringToFlowPath(actionData->getParamForKey(kCTAActionKey));
    
    if( fileName == kEnterEmailCTAName)
    {
        handleEnterEmailFlow(actionData, pathAction);
    }
    else if(fileName == kConfirmEmailCTAName)
    {
        handleConfirmEmailFlow(actionData, pathAction);
    }
    else if(fileName == kEnterPasswordCTAName)
    {
        handleEnterPasswordFlow(actionData, pathAction);
    }
    else if(fileName == kEnterPinCTAName)
    {
        handleEnterPinFlow(actionData, pathAction);
    }
}

void SignUpFlowController::handleEnterEmailFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
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
            const std::string& email = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("email");
            if(isValidEmailAddress(email.c_str()))
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("enterEmail_continue");
                DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(kConfirmEmailCTAName, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            }
            break;
        }
            
        case BACK: case CLOSE:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("enterEmail_close");
            exitFlow();
            break;
        }

    }
}

void SignUpFlowController::handleConfirmEmailFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
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
            AnalyticsSingleton::getInstance()->ctaButtonPressed("confirmEmail_confirm");
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(kEnterPasswordCTAName, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            break;
        }
            
        case BACK:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("confirmEmail_back");
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(kEnterEmailCTAName, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            break;
        }
        case CLOSE:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("confirmEmail_close");
            exitFlow();
            break;
        }
            
    }
}

void SignUpFlowController::handleEnterPasswordFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
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
            const std::string& password = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("password");
            if(isValidPassword(password.c_str(),6))
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("enterPassword_continue");
                DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(kEnterPinCTAName,DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            }
            break;
        }
            
        case BACK:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("enterPassword_back");
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(kConfirmEmailCTAName,DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            break;
        }
        case CLOSE:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("enterPassword_close");
            exitFlow();
            break;
        }
            
    }
}

void SignUpFlowController::handleEnterPinFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
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
            const std::string& pin = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("pin");
            if(isValidPin(pin.c_str()))
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("enterPin_continue");
                signUp();
            }
            break;
        }
            
        case BACK:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("enterPin_back");
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(kEnterPasswordCTAName, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            break;
        }
        case CLOSE:
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("enterPin_close");
            exitFlow();
            break;
        }
            
    }
}

void SignUpFlowController::signUp()
{
    const std::string& email = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("email");
    const std::string& password = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("password");
    const std::string& pin = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("pin");
    if(isValidPin(pin.c_str()) && isValidPassword(password.c_str(), 6) && isValidEmailAddress(email.c_str()))
    {
        AnalyticsSingleton::getInstance()->registerAzoomeeEmail(email);
        BackEndCaller::getInstance()->registerParent(email, password ,pin);
        DynamicNodeDataInputStorage::getInstance()->clearStorage();
        exitFlow();
    }
}

NS_AZOOMEE_END
