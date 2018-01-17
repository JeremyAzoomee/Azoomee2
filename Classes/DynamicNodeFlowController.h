//
//  DynamicNodeFlowController.h
//  azoomee2
//
//  Created by Macauley on 17/01/2018.
//

#ifndef DynamicNodeFlowController_h
#define DynamicNodeFlowController_h

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>
#include "ButtonActionData.h"

NS_AZOOMEE_BEGIN

class DynamicNodeFlowController;
typedef std::shared_ptr<DynamicNodeFlowController> DynamicNodeFlowControllerRef;

enum FlowType{NONE, SIGNUP, IAP, ADDCHILD};
enum FlowPath{UNKNOWN, NEXT, BACK, CLOSE};

class DynamicNodeFlowController
{
public:
    static DynamicNodeFlowControllerRef create();
    virtual void processAction(ButtonActionDataRef actionData);
    DynamicNodeFlowController() noexcept;
protected:
    void exitFlow();
    FlowPath convertStringToFlowPath(const std::string& flowPathStr);
    
    FlowType _type;
    
    static const std::string _kCTAFilenameKey;
    static const std::string _kCTAActionKey;
};

NS_AZOOMEE_END

#endif /* DynamicNodeFlowController_h */
