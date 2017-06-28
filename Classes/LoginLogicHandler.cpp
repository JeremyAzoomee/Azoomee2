#include "LoginLogicHandler.h"
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include "BackEndCaller.h"
#include "DeepLinkingSingleton.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    return true;
}

void LoginLogicHandler::doLoginLogic()
{
#ifdef forgetuserdata
    emptyUserName();
#endif
    
    Azoomee::ChildDataParser::getInstance()->setChildLoggedIn(false);
    
    if(Azoomee::ParentDataParser::getInstance()->hasParentLoginDataInUserDefaults())
    {
        Azoomee::ParentDataParser::getInstance()->retrieveParentLoginDataFromUserDefaults();
        BackEndCaller::getInstance()->getAvailableChildren();
        BackEndCaller::getInstance()->updateBillingData();
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

NS_AZOOMEE_END
