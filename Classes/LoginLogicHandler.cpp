#include "LoginLogicHandler.h"
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "BackEndCaller.h"
#include "DeepLinkingSingleton.h"
#include "SceneManagerScene.h"
#include "MarketingAssetManager.h"
#include "HQHistoryManager.h"

#include "RoutePaymentSingleton.h"

#include <AzoomeeOomeeMaker/DataObjects/OomeeMakerDataHandler.h>
#include <AzoomeeCommon/Data/User/UserAccountManager.h>
#include <AzoomeeCommon/Data/Rewards/RewardManager.h>

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
    
    //Azoomee::ChildManager::getInstance()->setChildLoggedIn(false);
    UserAccountManager::getInstance()->logout();
    
    if(UserAccountManager::getInstance()->localLogin())
    {
        handleLoginSuccess();
    }
    else if(DeepLinkingSingleton::getInstance()->actionDeepLink())
    {
        return;
    }
    else
    {
        setLoginOrigin(LoginOrigin::LOGOUT);
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
    }
    
    /*if(Azoomee::ParentManager::getInstance()->hasParentLoginDataInUserDefaults())
    {
        Azoomee::ParentManager::getInstance()->retrieveParentLoginDataFromUserDefaults();
        BackEndCaller::getInstance()->getAvailableChildren();
        BackEndCaller::getInstance()->updateBillingData();
        BackEndCaller::getInstance()->getParentDetails();
		AnalyticsSingleton::getInstance()->registerIdentifier(ParentManager::getInstance()->getLoggedInParentId());
        MarketingAssetManager::getInstance()->downloadMarketingAssets();
        OomeeMaker::OomeeMakerDataHandler::getInstance()->getLatestDataAsync();
        return;
    }
    else if(DeepLinkingSingleton::getInstance()->actionDeepLink())
    {
        return;
    }
    else
    {
        setLoginOrigin(LoginOrigin::LOGOUT);
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
    }*/
}

void LoginLogicHandler::forceNewLogin(const LoginOrigin& origin)
{
    Azoomee::ParentManager::getInstance()->clearParentLoginDataFromUserDefaults();
    setLoginOrigin(origin);
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
}

void LoginLogicHandler::setLoginOrigin(const LoginOrigin& origin)
{
    _origin = origin;
}
LoginOrigin LoginLogicHandler::getOrigin() const
{
    return _origin;
}

void LoginLogicHandler::handleLoginSuccess()
{
    AnalyticsSingleton::getInstance()->registerIdentifier(ParentManager::getInstance()->getLoggedInParentId());
    
    if(!ParentManager::getInstance()->isLoggedInParentAnonymous())
    {
        ConfigStorage::getInstance()->setFirstSlideShowSeen();
        
        //if reciept needs validating, do validation here, on success or fail it will fire this function again with reciept deleted
        if(RoutePaymentSingleton::getInstance()->receiptDataFileExists())
        {
            Director::getInstance()->getScheduler()->schedule([&](float dt){
                RoutePaymentSingleton::getInstance()->retryReceiptValidation();
            }, this, 1.0, 0, 0, false, "receiptValidation");
            return;
        }
    }
    
    UserAccountManager::getInstance()->getBillingDataForLoggedInParent(nullptr);
    MarketingAssetManager::getInstance()->downloadMarketingAssets();
    OomeeMaker::OomeeMakerDataHandler::getInstance()->getLatestDataAsync();
    
    
    AnalyticsSingleton::getInstance()->setIsUserAnonymous(ParentManager::getInstance()->isLoggedInParentAnonymous());
    AnalyticsSingleton::getInstance()->signInSuccessEvent();
    
    UserAccountManager::getInstance()->getChildrenForLoggedInParent([this](bool success, long errorCode){
        if(success)
        {
            handleGetChildrenSuccess();
        }
        else
        {
            if(errorCode == 401)
            {
                UserAccountManager::getInstance()->logout();
                setLoginOrigin(LoginOrigin::LOGOUT);
                Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
            }
        }
    });
}

void LoginLogicHandler::handleGetChildrenSuccess()
{
    if(ParentManager::getInstance()->getAmountOfAvailableChildren() == 0)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::AddChild));
    }
    else
    {
        if(ParentManager::getInstance()->isLoggedInParentAnonymous())
        {
            UserDefault* userDefault = UserDefault::getInstance();
            bool anonOnboardingComplete = userDefault->getBoolForKey("anonOnboardingComplete", false);
            if(!anonOnboardingComplete)
            {
                Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::AddChild));
            }
            else
            {
                // auto login default child
                UserAccountManager::getInstance()->loginChild(ParentManager::getInstance()->getChild(0)->getProfileName(),[this](bool success, long error){
                    if(success)
                    {
                        handleChildLoginSuccess();
                    }
                    else
                    {
                        UserAccountManager::getInstance()->logout();
                        this->setLoginOrigin(LoginOrigin::LOGOUT);
                        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
                    }
                });
                return;
            }
        }
        else
        {
            const auto targetScene = (!ParentManager::getInstance()->isPaidUser() && LoginLogicHandler::getInstance()->getOrigin() == LoginOrigin::IAP_PAYWALL) ? SceneNameEnum::IAP : SceneNameEnum::ChildSelector;
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(targetScene));
        }
    }

}

void LoginLogicHandler::handleChildLoginSuccess()
{
    BackEndCaller::getInstance()->getChildInventory();
    OomeeMaker::OomeeMakerDataHandler::getInstance()->getOomeesForChild(ChildManager::getInstance()->getLoggedInChild()->getId(), false);
    BackEndCaller::getInstance()->getSessionCookies();
    HQHistoryManager::getInstance()->emptyHistory();
                           
    // Update rewards feed
    RewardManager::getInstance()->getLatestRewardStrategy();
    RewardManager::getInstance()->checkForPendingRewards();
}

NS_AZOOMEE_END
