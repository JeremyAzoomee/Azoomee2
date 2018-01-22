//
//  AddChildFlowController.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 18/01/2018.
//

#include "AddChildFlowController.h"
#include "BackEndCaller.h"
#include "FlowDataSingleton.h"
#include "DynamicNodeDataInputStorage.h"
#include "DynamicNodeHandler.h"
#include <AzoomeeCommon/Input/TextInputChecker.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <time.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string AddChildFlowController::kAddChildCTAName = "add_child.json";
const std::string AddChildFlowController::kSelectOomeeCTAName = "add_child_oomee.json";
const std::string AddChildFlowController::kNewPaidAccountCTAName = "payment_new_account.json";
DynamicNodeFlowControllerRef AddChildFlowController::create()
{
    return std::make_shared<AddChildFlowController>();
}

void AddChildFlowController::processAction(ButtonActionDataRef actionData)
{
    const std::string& fileName = actionData->getParamForKey(_kCTAFilenameKey);
    if( fileName == kAddChildCTAName)
    {
        handleAddChildFlow(actionData);
    }
    else if(fileName == kSelectOomeeCTAName)
    {
        handleSelectOomeeFlow(actionData);
    }
    else if(fileName == kNewPaidAccountCTAName)
    {
        handleNewPaidAccountFlow(actionData);
    }
}

AddChildFlowController::AddChildFlowController() noexcept
{
    if(FlowDataSingleton::getInstance()->getIAPSuccess() && FlowDataSingleton::getInstance()->isSignupNewProfileFlow())
    {
        _flowEntryFile = kNewPaidAccountCTAName;
    }
    else
    {
        _flowEntryFile = kAddChildCTAName;
    }
    _type = FlowType::ADDCHILD;
}

void AddChildFlowController::handleAddChildFlow(ButtonActionDataRef actionData)
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
            const std::string& profileName = trim(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("name"));
            
            int age = std::atoi(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("age").c_str());
            if(age <= 0)
            {
                return;
            }
            time_t t = time(NULL);
            struct tm time = *localtime(&t);
            int year = 1900 + time.tm_year - age;
            
            if(!isDate(1, 1, year) || !isValidChildName(profileName.c_str()))
            {
                return;
            }
            
            DynamicNodeHandler::getInstance()->createDynamicNodeById(kSelectOomeeCTAName);
            break;
        }
        case CLOSE: case BACK:
        {
            exitFlow();
            break;
        }
            
    }
}

void AddChildFlowController::handleSelectOomeeFlow(ButtonActionDataRef actionData)
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
            int oomeeNum = std::atoi(actionData->getParamForKey("oomeeNum").c_str());
            addChild(oomeeNum);
            break;
        }
        case CLOSE: case BACK:
        {
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(kAddChildCTAName, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            break;
        }
            
    }
}

void AddChildFlowController::handleNewPaidAccountFlow(ButtonActionDataRef actionData)
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
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(kAddChildCTAName, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
            break;
        }
        case CLOSE: case BACK:
        {
            exitFlow();
            break;
        }
            
    }
}

void AddChildFlowController::addChild(int oomeeNum)
{
    const std::string& profileName = trim(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("name"));
    
    int age = std::atoi(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("age").c_str());
    if(age <= 0)
    {
        return;
    }
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    int year = 1900 + time.tm_year - age;
    
    const std::string& DOB = StringUtils::format("%04d-%02d-%02d",year,1,1);
    const std::string& gender = "MALE";
    if(!isDate(1, 1, year))
    {
        return;
    }
    auto backEndCaller = BackEndCaller::getInstance();
    if(FlowDataSingleton::getInstance()->isSignupNewProfileFlow() && ParentDataProvider::getInstance()->getAmountOfAvailableChildren() !=0)
    {
        backEndCaller->updateChild(ParentDataProvider::getInstance()->getIDForAvailableChildren(0), profileName, gender, DOB, oomeeNum);
    }
    else
    {
        backEndCaller->registerChild(profileName, gender, DOB, oomeeNum);
    }
    DynamicNodeDataInputStorage::getInstance()->clearStorage();
    exitFlow();
}

NS_AZOOMEE_END
