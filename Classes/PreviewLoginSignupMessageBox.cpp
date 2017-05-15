#include "PreviewLoginSignupMessageBox.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "LoginLogicHandler.h"
#include "OrientationChangeScene.h"

using namespace Azoomee;


bool PreviewLoginSignupMessageBox::init()
{
    if( !Super::init() )
    {
        return false;
    }
    
    addButtonWithTitle(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_IN));
    addButtonWithTitle(StringMgr::getInstance()->getStringForKey(BUTTON_SIGN_UP));
    initMessageBoxLayer(StringMgr::getInstance()->getStringForKey(PREVIEW_MESSAGEBOX_TITLE_LABEL),
                        StringMgr::getInstance()->getStringForKey(PREVIEW_MESSAGEBOX_BODY_LABEL),nullptr);
    return true;
}

void PreviewLoginSignupMessageBox::onCancelPressed()
{
    AnalyticsSingleton::getInstance()->previewPopupCancelledEvent();
    Super::onCancelPressed();
}

void PreviewLoginSignupMessageBox::onButtonPressed(int buttonSelect)
{
    if( buttonSelect == 0 ) // log-in
    {
        LoginLogicHandler::getInstance()->forceNewLogin();
    }
    else if( buttonSelect == 1 ) // signup
    {
        auto orientationChangeScene = OrientationChangeScene::createScene(true, ONBOARDING_SCENE, 0);
        Director::getInstance()->replaceScene(orientationChangeScene);
    }
}
