//
//  SignUpFlowController.h
//  azoomee2
//
//  Created by Macauley on 17/01/2018.
//

#ifndef SignUpFlowController_h
#define SignUpFlowController_h

#include "DynamicNodeFlowController.h"

NS_AZOOMEE_BEGIN

class SignUpFlowController : public DynamicNodeFlowController
{
private:
    static const std::string kEnterEmailCTAName;
    static const std::string kConfirmEmailCTAName;
    static const std::string kEnterPasswordCTAName;
    static const std::string kEnterPinCTAName;
    static const std::string kPaymentSuccessCTAName;
    
    void handleEnterEmailFlow(ButtonActionDataRef actionData);
    void handleConfirmEmailFlow(ButtonActionDataRef actionData);
    void handleEnterPasswordFlow(ButtonActionDataRef actionData);
    void handleEnterPinFlow(ButtonActionDataRef actionData);
    void handlePaymentSuccessFlow(ButtonActionDataRef actionData);
    
    void signUp();
    
public:
    static DynamicNodeFlowControllerRef create();
    virtual void processAction(ButtonActionDataRef actionData) override;
    SignUpFlowController() noexcept;
};

NS_AZOOMEE_END
#endif /* SignUpFlowController_h */
