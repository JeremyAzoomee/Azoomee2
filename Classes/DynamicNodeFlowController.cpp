//
//  DynamicNodeFlowController.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 17/01/2018.
//

#include "DynamicNodeFlowController.h"
#include "DynamicNodeHandler.h"

NS_AZOOMEE_BEGIN

const std::string DynamicNodeFlowController::kCTAFilenameKey = "fileName";
const std::string DynamicNodeFlowController::kCTAActionKey = "action";

DynamicNodeFlowControllerRef DynamicNodeFlowController::create()
{
    return std::make_shared<DynamicNodeFlowController>();
}

DynamicNodeFlowController::DynamicNodeFlowController() noexcept
{
    _type = FlowType::NONE;
}

FlowType DynamicNodeFlowController::getType() const
{
    return _type;
}

void DynamicNodeFlowController::processAction(const ButtonActionDataRef& actionData)
{
    // called in DynamicNodeButtonListener
    // override in sub class to initiate processing of flow using given data
}

void DynamicNodeFlowController::exitFlow()
{
    DynamicNodeCreator::getInstance()->resetCTAPopup();
    DynamicNodeHandler::getInstance()->setFlowController(nullptr);
}

FlowPath DynamicNodeFlowController::convertStringToFlowPath(const std::string &flowPathStr)
{
    if(flowPathStr == "next")
    {
        return FlowPath::NEXT;
    }
    if(flowPathStr == "back")
    {
        return FlowPath::BACK;
    }
    if(flowPathStr == "close")
    {
        return FlowPath::CLOSE;
    }
    
    return FlowPath::UNKNOWN;
}
NS_AZOOMEE_END
