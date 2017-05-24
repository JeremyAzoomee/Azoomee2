#include "LoginLogicHandler.h"
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include "BackEndCaller.h"
#include "DeepLinkingSingleton.h"
#include "SceneManagerScene.h"

using namespace cocos2d;

static LoginLogicHandler *_sharedLoginLogicHandler = NULL;

LoginLogicHandler* LoginLogicHandler::getInstance()
{
    if (! _sharedLoginLogicHandler)
    {
        _sharedLoginLogicHandler = new LoginLogicHandler();
        _sharedLoginLogicHandler->init();
    }
    
    return _sharedLoginLogicHandler;
}

LoginLogicHandler::~LoginLogicHandler(void)
{
}

bool LoginLogicHandler::init(void)
{
    errorMsgCode = 0;
    return true;
}

void LoginLogicHandler::doLoginLogic()
{
#ifdef forgetuserdata
    emptyUserName();
#endif
    
    if(Azoomee::ParentDataParser::getInstance()->hasParentLoginDataInUserDefaults())
    {
        Azoomee::ParentDataParser::getInstance()->retrieveParentLoginDataFromUserDefaults();
        BackEndCaller::getInstance()->updateBillingData();
        BackEndCaller::getInstance()->getAvailableChildren();
        return;
    }
    else if(DeepLinkingSingleton::getInstance()->actionDeepLink())
        return;
    else
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Login));
    }
}

void LoginLogicHandler::forceNewLogin()
{
    Azoomee::ParentDataParser::getInstance()->clearParentLoginDataFromUserDefaults();
    
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(Login));
}

void LoginLogicHandler::setErrorMessageCodeToDisplay(long errorMessageCode)
{
    errorMsgCode = errorMessageCode;
}

long LoginLogicHandler::getErrorMessageCodeToDisplay()
{
    long rtrValue = errorMsgCode;
    errorMsgCode = 0;
    return rtrValue;
}
