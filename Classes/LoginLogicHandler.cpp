#include "LoginLogicHandler.h"
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "BackEndCaller.h"
#include "DeepLinkingSingleton.h"
#include "SceneManagerScene.h"

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
#ifdef FORGET_USER_DATA
    emptyUserName();
#endif
    
    Azoomee::ChildDataParser::getInstance()->setChildLoggedIn(false);
    
    if(Azoomee::ParentDataParser::getInstance()->hasParentLoginDataInUserDefaults())
    {
        Azoomee::ParentDataParser::getInstance()->retrieveParentLoginDataFromUserDefaults();
        BackEndCaller::getInstance()->getAvailableChildren();
        BackEndCaller::getInstance()->updateBillingData();
        BackEndCaller::getInstance()->getParentDetails();
		AnalyticsSingleton::getInstance()->registerIdentifier(ParentDataProvider::getInstance()->getLoggedInParentId());
        return;
    }
    else if(DeepLinkingSingleton::getInstance()->actionDeepLink())
    {
        return;
    }
    else
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
    }
}

void LoginLogicHandler::forceNewLogin()
{
    Azoomee::ParentDataParser::getInstance()->clearParentLoginDataFromUserDefaults();
    
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
}

NS_AZOOMEE_END
