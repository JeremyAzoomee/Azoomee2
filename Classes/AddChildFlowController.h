//
//  AddChildFlowController.h
//  azoomee2
//
//  Created by Macauley on 18/01/2018.
//

#ifndef AddChildFlowController_h
#define AddChildFlowController_h

#include "DynamicNodeFlowController.h"

NS_AZOOMEE_BEGIN

class AddChildFlowController : public DynamicNodeFlowController
{
private:
    static const std::string kAddChildCTAName;
    static const std::string kSelectOomeeCTAName;
    
    void handleAddChildFlow(const ButtonActionDataRef& actionData);
    void handleSelectOomeeFlow(const ButtonActionDataRef& actionData);
    
    void addChild(int oomeeNum);
    
public:
    static DynamicNodeFlowControllerRef create();
    virtual void processAction(const ButtonActionDataRef& actionData) override;
    AddChildFlowController() noexcept;
};

NS_AZOOMEE_END

#endif /* AddChildFlowController_h */
