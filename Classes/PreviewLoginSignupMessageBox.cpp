#include "PreviewLoginSignupMessageBox.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "LoginLogicHandler.h"
#include "SceneManagerScene.h"
#include "DynamicNodeHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN


bool PreviewLoginSignupMessageBox::init()
{
    if( !Layer::init() )
    {
        return false;
    }
    
    MessageBox::createWith(PREVIEW_SIGNIN_LOGIN, this);
    Director::getInstance()->getRunningScene()->addChild(this);
    return true;
}

void PreviewLoginSignupMessageBox::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(buttonTitle == MessageBox::kLogin)
    {
        LoginLogicHandler::getInstance()->forceNewLogin();
    }
    else if(buttonTitle == MessageBox::kCancel)
    {
        AnalyticsSingleton::getInstance()->previewPopupCancelledEvent();
    }
    else if(buttonTitle == MessageBox::kSignUp)
    {
        DynamicNodeHandler::getInstance()->startSignupFlow();
    }
}

NS_AZOOMEE_END
