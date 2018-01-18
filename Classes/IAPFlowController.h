//
//  IAPFlowController.h
//  azoomee2
//
//  Created by Macauley on 18/01/2018.
//

#ifndef IAPFlowController_h
#define IAPFlowController_h

#include "DynamicNodeFlowController.h"
#include "AwaitingAdultPinLayer.h"

NS_AZOOMEE_BEGIN

class IAPFlowController : public DynamicNodeFlowController, public AwaitingAdultPinLayerDelegate
{
private:
    static const std::string kIAPUpgradeCTAName;
    static const std::string kCoppaPrivacyCTAName;
    static const std::string kLearnMoreCTAName;
    
    const std::string _kPathIAP = "iap";
    const std::string _kPathRestore = "restore";
    const std::string _kPathLearnMore = "learnMore";
    const std::string _kPathCoppa = "coppa";
    
    ButtonActionDataRef _actionData = nullptr;
    
    void handleIAPUpgradeFlow(ButtonActionDataRef actionData);
    void handleCoppaPrivacyFlow(ButtonActionDataRef actionData);
    void handleLearnMoreFlow(ButtonActionDataRef actionData);
    
    void startIAP();
    
public:
    static DynamicNodeFlowControllerRef create();
    virtual void processAction(ButtonActionDataRef actionData) override;
    IAPFlowController() noexcept;

    // delegate functions
    void AdultPinCancelled(AwaitingAdultPinLayer* layer) override;
    void AdultPinAccepted(AwaitingAdultPinLayer* layer) override;
};

NS_AZOOMEE_END

#endif /* IAPFlowController_h */
