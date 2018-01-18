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

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string AddChildFlowController::kAddChildCTAName = "add_child.json";
const std::string AddChildFlowController::kSelectOomeeCTAName = "add_child_oomee.json";

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
}

AddChildFlowController::AddChildFlowController() noexcept
{
    _flowEntryFile = kAddChildCTAName;
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
            
            int day = std::atoi(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("day").c_str());
            int month = std::atoi(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("month").c_str());
            int year = std::atoi(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("year").c_str());
            
            if(!isDate(day, month, year) || !isValidChildName(profileName.c_str()))
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

void AddChildFlowController::addChild(int oomeeNum)
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
