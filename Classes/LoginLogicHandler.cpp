#include "LoginLogicHandler.h"
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include "LoginScene.h"
#include "ParentDataParser.h"
#include "BackEndCaller.h"

using namespace cocos2d;

void LoginLogicHandler::doLoginLogic()
{
#ifdef forgetuserdata
    emptyLoginData();
#endif
    
    if(ParentDataParser::getInstance()->hasParentLoginDataInUserDefaults())
    {
        ParentDataParser::getInstance()->retrieveParentLoginDataFromUserDefaults();
        BackEndCaller::getInstance()->getAvailableChildren();
        BackEndCaller::getInstance()->updateBillingData();
        return;
    }
    
    if(hasLoginData())
    {
        HQHistoryManager::getInstance()->emptyHistory();
        auto baseScene = BaseScene::createScene();
        Director::getInstance()->replaceScene(baseScene);
    }
    else
    {
        auto loginScene = LoginScene::createSceneWithAutoLogin();
        Director::getInstance()->replaceScene(loginScene);
    }
}

//-----------------------------------------------------All requests below this line are used internally-------------------------------------------------------

bool LoginLogicHandler::hasLoginData()
{
    UserDefault* def = UserDefault::getInstance();
    std::string username = def->getStringForKey("username", "");
    std::string password = def->getStringForKey("password", "");
    
    if((username == "")||(password == "")) return false;
    else return true;
}

std::string LoginLogicHandler::getLoginName()
{
    return UserDefault::getInstance()->getStringForKey("username", "");
}

std::string LoginLogicHandler::getLoginPassword()
{
    return UserDefault::getInstance()->getStringForKey("password", "");
}

void LoginLogicHandler::emptyLoginData()
{
    UserDefault* def2 = UserDefault::getInstance();
    def2->setStringForKey("username", "");
    def2->setStringForKey("password", "");
    def2->flush();
}
