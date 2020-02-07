#include "LoginController.h"
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "BackEndCaller.h"
#include "DeepLinkingSingleton.h"
#include "SceneManagerScene.h"
#include "MarketingAssetManager.h"
#include "HQHistoryManager.h"

#include "RoutePaymentSingleton.h"

#include "AzoomeeOomeeMaker/DataObjects/OomeeMakerDataHandler.h"
#include "RewardManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static LoginController *_sharedLoginController = NULL;

LoginController* LoginController::getInstance()
{
    if (! _sharedLoginController)
    {
        _sharedLoginController = new LoginController();
        _sharedLoginController->init();
    }
    
    return _sharedLoginController;
}

LoginController::~LoginController(void)
{
}

bool LoginController::init(void)
{
    return true;
}

void LoginController::doLoginLogic()
{
#ifdef FORGET_USER_DATA
    emptyUserName();
#endif
    
    UserAccountManager::getInstance()->logoutChild();
    
    if(UserAccountManager::getInstance()->localLogin())
    {
        UserAccountManager::getInstance()->getBillingDataForLoggedInParent([this](bool, long){
            UserAccountManager::getInstance()->getChildrenForLoggedInParent([this](bool success, long errorcode){
                if(success)
                {
                    handleLoginSuccess();
                }
                else
                {
                    forceNewLogin();
                }
            });
        });
    }
    else if(DeepLinkingSingleton::getInstance()->actionDeepLink())
    {
        return;
    }
    else
    {
        forceNewLogin(LoginOrigin::LOGOUT);
    }
}

void LoginController::forceNewLogin(const LoginOrigin& origin)
{
    Azoomee::UserAccountManager::getInstance()->logoutParent();
    setLoginOrigin(origin);
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
}

void LoginController::setLoginOrigin(const LoginOrigin& origin)
{
    _origin = origin;
}
LoginOrigin LoginController::getOrigin() const
{
    return _origin;
}

void LoginController::login(const std::string& email, const std::string& password)
{
    UserAccountManager::getInstance()->login(email, password, [this](bool success, long errorcode){
        if(success)
        {
            handleLoginSuccess();
        }
    });
}

void LoginController::anonLogin()
{
    UserAccountManager::getInstance()->anonLogin([this](bool success, long errorcode){
        if(success)
        {
            handleLoginSuccess();
        }
    });
}

void LoginController::childLogin(const std::string& childName)
{
    UserAccountManager::getInstance()->loginChild(childName, [this](bool success, long errorcode){
        if(success)
        {
            handleChildLoginSuccess();
        }
        else
        {
            forceNewLogin();
        }
    });
}

void LoginController::handleLoginSuccess()
{
    AnalyticsSingleton::getInstance()->registerIdentifier(UserAccountManager::getInstance()->getLoggedInParentId());
    
    if(!UserAccountManager::getInstance()->isLoggedInParentAnonymous())
    {
        UserAccountManager::getInstance()->setHasLoggedInOnDevice(true);
        
        //if reciept needs validating, do validation here, on success or fail it will fire this function again with reciept deleted
        if(RoutePaymentSingleton::getInstance()->receiptDataFileExists())
        {
            Director::getInstance()->getScheduler()->schedule([&](float dt){
                RoutePaymentSingleton::getInstance()->retryReceiptValidation();
            }, this, 1.0, 0, 0, false, "receiptValidation");
            return;
        }
    }
    
    MarketingAssetManager::getInstance()->downloadMarketingAssets();
    
    
    AnalyticsSingleton::getInstance()->setIsUserAnonymous(UserAccountManager::getInstance()->isLoggedInParentAnonymous());
    AnalyticsSingleton::getInstance()->signInSuccessEvent();
    handleGetChildrenSuccess();
}

void LoginController::handleGetChildrenSuccess()
{
    if(UserAccountManager::getInstance()->getAmountOfAvailableChildren() == 0)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::AddChild));
    }
    else
    {
        if(UserAccountManager::getInstance()->isLoggedInParentAnonymous())
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
                childLogin(UserAccountManager::getInstance()->getChild(0)->getProfileName());
                return;
            }
        }
        else
        {
            const auto targetScene = (!UserAccountManager::getInstance()->isPaidUser() && LoginController::getInstance()->getOrigin() == LoginOrigin::IAP_PAYWALL) ? SceneNameEnum::IAP : SceneNameEnum::ChildSelector;
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(targetScene));
        }
    }

}

void LoginController::handleChildLoginSuccess()
{
    ChildManager::getInstance()->updateInventory();
    OomeeMaker::OomeeMakerDataHandler::getInstance()->getLatestDataAsync();
    OomeeMaker::OomeeMakerDataHandler::getInstance()->getOomeesForChild(ChildManager::getInstance()->getLoggedInChild()->getId(), false);
    HQHistoryManager::getInstance()->emptyHistory();
                           
    // Update rewards feed
    RewardManager::getInstance()->getLatestRewardStrategy();
    RewardManager::getInstance()->checkForPendingRewards();
    
    SceneNameEnum nextScene = SceneNameEnum::Base;
    
    if(UserAccountManager::getInstance()->isLoggedInParentAnonymous())
    {
        if(LoginController::getInstance()->getOrigin() == LoginOrigin::IAP_PAYWALL)
        {
            nextScene = SceneNameEnum::IAP;
        }
        else if(LoginController::getInstance()->getOrigin() == LoginOrigin::SIGNUP)
        {
            nextScene = SceneNameEnum::Signup;
        }
    }
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(nextScene));
}

NS_AZOOMEE_END
