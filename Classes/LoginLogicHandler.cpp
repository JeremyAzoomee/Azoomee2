#include "LoginLogicHandler.h"
#include "LoginScene.h"
#include "ParentDataParser.h"
#include "BackEndCaller.h"

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
    
    if(ParentDataParser::getInstance()->hasParentLoginDataInUserDefaults())
    {
        ParentDataParser::getInstance()->retrieveParentLoginDataFromUserDefaults();
        BackEndCaller::getInstance()->updateBillingData();
        BackEndCaller::getInstance()->getAvailableChildren();
        return;
    }
    
    auto loginScene = LoginScene::createScene(0);
    Director::getInstance()->replaceScene(loginScene);
}

void LoginLogicHandler::forceNewLogin()
{
    ParentDataParser::getInstance()->clearParentLoginDataFromUserDefaults();
    
    auto loginScene = LoginScene::createScene(getErrorMessageCodeToDisplay());
    Director::getInstance()->replaceScene(loginScene);
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
