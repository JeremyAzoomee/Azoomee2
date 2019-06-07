//
//  DynamicNodeButtonListener.cpp
//  azoomee2
//
//  Created by Macauley on 25/09/2017.
//
//

#include "DynamicNodeButtonListener.h"
#include "DynamicNodeHandler.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "PreviewLoginSignupMessageBox.h"
#include "DynamicNodeCreator.h"
#include "SceneManagerScene.h"
#include "DeepLinkingSingleton.h"
#include "ContentHistoryManager.h"
#include "DynamicNodeDataInputStorage.h"
#include <AzoomeeCommon/Data/Urls.h>
#include <AzoomeeCommon/UI/ModalWebview.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Input/TextInputChecker.h>
#include "RoutePaymentSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "FlowDataSingleton.h"
#include "HQDataProvider.h"
#include "ContentOpener.h"


using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<DynamicNodeButtonListener> sDynamicNodeButtonListenerSharedInstance;

DynamicNodeButtonListener* DynamicNodeButtonListener::getInstance()
{
    if(!sDynamicNodeButtonListenerSharedInstance.get())
    {
        sDynamicNodeButtonListenerSharedInstance.reset(new DynamicNodeButtonListener());
    }
    return sDynamicNodeButtonListenerSharedInstance.get();
}

DynamicNodeButtonListener::~DynamicNodeButtonListener(void)
{
    
}

bool DynamicNodeButtonListener::init(void)
{
    return true;
}

void DynamicNodeButtonListener::onButtonPressedCallFunc(Ref* button, ui::Widget::TouchEventType eventType, const ButtonActionDataRef& buttonAction)
{
    _buttonAction = buttonAction;
    
    if(eventType == ui::Widget::TouchEventType::ENDED)
    {
        if(buttonAction->getType() == _kButtonTypeInternal)
        {
            const std::string& location = buttonAction->getParamForKey("location");
            if(location == _kButtonLocationUpgrade)
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("upgrade");
                upgradeButtonPressed();
            }
            else if(location == _kButtonLocationIAP)
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("startIAP");
                RoutePaymentSingleton::getInstance()->startInAppPayment();
                closeCTAPopup();
            }
            else
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("close");
                closeCTAPopup();
            }
        }
        else if(buttonAction->getType() == _kButtonTypeDeepLink)
        {
            const std::string& location = buttonAction->getParamForKey("location");
            if(location == "replay")
            {
                AnalyticsSingleton::getInstance()->registerCurrentScene("REPLAY");
                AnalyticsSingleton::getInstance()->ctaButtonPressed("replayContent", ContentHistoryManager::getInstance()->getLastOpenedContent()->getTitle());
                AnalyticsSingleton::getInstance()->contentItemSelectedOutsideCarouselEvent(ContentHistoryManager::getInstance()->getLastOpenedContent());
                ContentOpener::getInstance()->openContentObject(ContentHistoryManager::getInstance()->getLastOpenedContent());
                closeCTAPopup();
            }
            else
            {
                AnalyticsSingleton::getInstance()->registerCurrentScene("RECOMMENDED");
                HQContentItemObjectRef content = HQDataProvider::getInstance()->getItemDataForSpecificItem(location.substr(DeepLinkingSingleton::kPostContentDeeplinkStr.length()));
                std::string contentTitle = "";
                if(content)
                {
                    contentTitle = content->getTitle();
                }
                AnalyticsSingleton::getInstance()->ctaButtonPressed("OpenRecommendedContent", contentTitle);
                AnalyticsSingleton::getInstance()->contentItemSelectedOutsideCarouselEvent(content);
                ContentOpener::getInstance()->openContentObject(content);
                closeCTAPopup();
            }
        }
        else if(buttonAction->getType() == _kButtonTypeWeb)
        {
            const std::string& location = buttonAction->getParamForKey("location");
            if(location == "privacyPolicy")
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("privacyPolicy");
                ModalWebview::createWithURL(Url::PrivacyPolicyNoLinks);
            }
            else if(location == "termsOfUse")
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("termsOfUse");
                ModalWebview::createWithURL(Url::TermsOfUse);
            }
            else if(location != "")
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("webViewURL");
                ModalWebview::createWithURL(location);
            }
        }
        else if(buttonAction->getType() == _kButtonTypeCTATransition)
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("ctaTransition");
            const std::string& location = buttonAction->getParamForKey("location");
            DynamicNodeHandler::getInstance()->createDynamicNodeById(location);
        }
        else if(buttonAction->getType() == _kButtonTypeCTATransitionParams)
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("ctaTransition");
            const std::string& location = buttonAction->getParamForKey("location");
            const std::string& params = DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString();
            DynamicNodeHandler::getInstance()->createDynamicNodeByIdWithParams(location, params);
        }
        else if(buttonAction->getType() == _kButtonTypeCTATransitionGroup)
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("ctaTransitionGroup");
            const std::string& location = buttonAction->getParamForKey("location");
            DynamicNodeHandler::getInstance()->createDynamicNodeByGroupId(location);
        }
        else if(buttonAction->getType() == _kButtonTypeFlowAction)
        {
            DynamicNodeFlowControllerRef flowController = DynamicNodeHandler::getInstance()->getFlowController();
            if(flowController)
            {
                flowController->processAction(buttonAction);
            }
            else
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("close");
                closeCTAPopup();
            }
        }
        else if(buttonAction->getType() == _kButtonTypeStartFlow)
        {
            const std::string& flow = buttonAction->getParamForKey("flowType");
            if(flow == "signup")
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("startFlowSignUp");
                DynamicNodeHandler::getInstance()->startSignupFlow();
            }
            else if(flow == "iap")
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("startFlowIAP");
                DynamicNodeHandler::getInstance()->startIAPFlow();
            }
            else if(flow == "addchild")
            {
                AnalyticsSingleton::getInstance()->ctaButtonPressed("startFlowAddChild");
                DynamicNodeHandler::getInstance()->startAddChildFlow();
            }
        }
    }
}

void DynamicNodeButtonListener::upgradeButtonPressed()
{
    DynamicNodeHandler::getInstance()->startIAPFlow();
}

void DynamicNodeButtonListener::closeCTAPopup()
{
    DynamicNodeCreator::getInstance()->resetCTAPopup();
}

NS_AZOOMEE_END
