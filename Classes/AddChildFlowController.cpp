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
#include <AzoomeeCommon/Utils/TimeFunctions.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string AddChildFlowController::kAddChildCTAName = "add_child.json";
const std::string AddChildFlowController::kSelectOomeeCTAName = "add_child_oomee.json";

DynamicNodeFlowControllerRef AddChildFlowController::create()
{
    return std::make_shared<AddChildFlowController>();
}

void AddChildFlowController::processAction(const ButtonActionDataRef& actionData)
{
    const std::string& fileName = actionData->getParamForKey(kCTAFilenameKey);
    FlowPath pathAction = convertStringToFlowPath(actionData->getParamForKey(kCTAActionKey));
    
    if( fileName == kAddChildCTAName)
    {
        handleAddChildFlow(actionData, pathAction);
    }
    else if(fileName == kSelectOomeeCTAName)
    {
        handleSelectOomeeFlow(actionData, pathAction);
    }
}

AddChildFlowController::AddChildFlowController() noexcept
{
    _flowEntryFile = kAddChildCTAName;
    _type = FlowType::ADDCHILD;
}

void AddChildFlowController::handleAddChildFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
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
            const std::string& profileName = trim(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("name"));
            
            int age = std::atoi(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("age").c_str());
            if(age <= 0)
            {
                return;
            }
            
            int year = birthYearFromAge(age);
            
            if(!isDate(1, 1, year) || !isValidChildName(profileName.c_str()))
            {
                return;
            }
            
            AnalyticsSingleton::getInstance()->ctaButtonPressed("addChild_next");
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

void AddChildFlowController::handleSelectOomeeFlow(const ButtonActionDataRef& actionData, FlowPath pathAction)
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
    
    int age = std::atoi(DynamicNodeDataInputStorage::getInstance()->getElementFromStorage("age").c_str());
    if(age <= 0)
    {
        return;
    }
    
    int year = birthYearFromAge(age);
    
    const std::string& DOB = StringUtils::format("%04d-%02d-%02d",year,1,1);
    const std::string& gender = "MALE";
    if(!isDate(1, 1, year))
    {
        return;
    }
    
    AnalyticsSingleton::getInstance()->childProfileCreatedEvent(age);
    
    auto backEndCaller = BackEndCaller::getInstance();
    if(FlowDataSingleton::getInstance()->isSignupNewProfileFlow() && ParentManager::getInstance()->getAmountOfAvailableChildren() !=0)
    {
        backEndCaller->updateChild(ParentManager::getInstance()->getChild(0)->getId(), profileName, gender, DOB, oomeeNum);
    }
    else
    {
        backEndCaller->registerChild(profileName, gender, DOB, oomeeNum);
    }
    DynamicNodeDataInputStorage::getInstance()->clearStorage();
    exitFlow();
}

NS_AZOOMEE_END
