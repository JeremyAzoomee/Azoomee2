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
#include <AzoomeeCommon/Input/TextInputChecker.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string SignUpFlowController::_kEnterEmailCTAName = "signup_email.json";
const std::string SignUpFlowController::_kConfirmEmailCTAName = "signup_email_confirm.json";
const std::string SignUpFlowController::_kEnterPasswordCTAName = "signup_password.json";
const std::string SignUpFlowController::_kEnterPinCTAName = "signup_pin.json";

SignUpFlowController::SignUpFlowController() noexcept
{
    _type = FlowType::SIGNUP;
}

DynamicNodeFlowControllerRef SignUpFlowController::create()
{
    return std::make_shared<SignUpFlowController>();
}

void SignUpFlowController::processAction(ButtonActionDataRef actionData)
{
    const std::string& fileName = actionData->getParamForKey(_kCTAFilenameKey);
    if( fileName == _kEnterEmailCTAName)
    {
        handleEnterEmailFlow(actionData);
    }
    else if(fileName == _kConfirmEmailCTAName)
    {
        handleConfirmEmailFlow(actionData);
    }
    else if(fileName == _kEnterPasswordCTAName)
    {
        handleEnterPasswordFlow(actionData);
    }
    else if(fileName == _kEnterPinCTAName)
    {
        handleEnterPinFlow(actionData);
    }
}

void SignUpFlowController::handleEnterEmailFlow(ButtonActionDataRef actionData)
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
            const std::string& email = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("email");
            if(isValidEmailAddress(email.c_str()))
            {
                DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(_kConfirmEmailCTAName, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            }
            break;
        }
            
        case BACK:
        {
            exitFlow();
            break;
        }
        case CLOSE:
        {
            exitFlow();
            break;
        }

    }
}

void SignUpFlowController::handleConfirmEmailFlow(ButtonActionDataRef actionData)
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
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(_kEnterPasswordCTAName, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            break;
        }
            
        case BACK:
        {
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(_kEnterEmailCTAName, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            break;
        }
        case CLOSE:
        {
            exitFlow();
            break;
        }
            
    }
}

void SignUpFlowController::handleEnterPasswordFlow(ButtonActionDataRef actionData)
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
            const std::string& password = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("password");
            if(isValidPassword(password.c_str(),6))
            {
                DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(_kEnterPinCTAName,DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            }
            break;
        }
            
        case BACK:
        {
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(_kConfirmEmailCTAName,DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            break;
        }
        case CLOSE:
        {
            exitFlow();
            break;
        }
            
    }
}

void SignUpFlowController::handleEnterPinFlow(ButtonActionDataRef actionData)
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
            const std::string& pin = DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("pin");
            if(isValidPin(pin.c_str()))
            {
                signUp();
            }
            break;
        }
            
        case BACK:
        {
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(_kEnterPasswordCTAName, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            break;
        }
        case CLOSE:
        {
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
        //ModalMessages::getInstance()->startLoading();
        AnalyticsSingleton::getInstance()->registerAzoomeeEmail(email);
        BackEndCaller::getInstance()->registerParent(email, password ,pin);
        DynamicNodeDataInputStorage::getInstance()->clearStorage();
        exitFlow();
    }
}

NS_AZOOMEE_END
