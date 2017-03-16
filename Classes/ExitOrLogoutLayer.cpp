#include "ExitOrLogoutLayer.h"
#include "LoginScene.h"
#include "StringMgr.h"
#include "ChildDataProvider.h"
#include "AudioMixer.h"
#include "AnalyticsSingleton.h"
#include "ParentDataParser.h"
#include "ParentDataProvider.h"
#include "ElectricDreamsTextStyles.h"
#include "PaymentSingleton.h"
#include "MessageBox.h"

bool ExitOrLogoutLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    AudioMixer::getInstance()->pauseBackgroundMusic();
    
    createAndFadeInLayer();
    askForPin();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void ExitOrLogoutLayer::askForPin()
{
    AwaitingAdultPinLayer::create()->setDelegate(this);
}

void ExitOrLogoutLayer::createAndFadeInLayer()
{
    backgroundLayer = LayerColor::create(Color4B(0,0,0,255),origin.x+ visibleSize.width, origin.y + visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void ExitOrLogoutLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) 
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

void ExitOrLogoutLayer::addExitOrLogoutUIObjects()
{
    //-------- VERSION NUBMER ---------
    
    auto versionTitle = createLabelAppVerison(APP_VERSION_NUMBER);
    backgroundLayer->addChild(versionTitle);
    
    // ------- CANCEL BUTTON ----------
    
    cancelButton = ElectricDreamsButton::createCancelButton();
    cancelButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2, origin.y + visibleSize.height * 0.3));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("ExitorLogoutCancelButton");
    backgroundLayer->addChild(cancelButton);
    
    // ------- LOG OUT BUTTON ----------
    
    logoutButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_OUT));
    logoutButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2, origin.y + visibleSize.height * 0.6));
    logoutButton->setDelegate(this);
    logoutButton->setMixPanelButtonName("Log Out");
    backgroundLayer->addChild(logoutButton);
    
    // ------- PURCHASE BUTTON ----------
    
    if(ParentDataProvider::getInstance()->getParentBillingStatus() != "SUBSCRIBED")
    {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            ElectricDreamsButton *premiumButton = ElectricDreamsButton::createButtonWithText("I want it all!");
            premiumButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2, origin.y + visibleSize.height * 0.45));
            premiumButton->setDelegate(this);
            backgroundLayer->addChild(premiumButton);
        #endif
    }
}

//---------------------- Actions -----------------

void ExitOrLogoutLayer::removeSelf()
{
    this->removeChild(backgroundLayer);
    this->removeFromParent();
}

//---------------------- public Functions After Setup -----------------------------

void ExitOrLogoutLayer::setCenterPosition(Vec2 position)
{
    this->setPosition(Vec2(position.x - this->getContentSize().width/2, position.y - this->getContentSize().height/2));
}

Vec2 ExitOrLogoutLayer::getCenterPosition()
{
    return Vec2(this->getPositionX() + this->getContentSize().width/2, this->getPositionY() + this->getContentSize().height/2);
}

//----------------------- Delegate Functions ----------------------------

void ExitOrLogoutLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
    {
        AudioMixer::getInstance()->resumeBackgroundMusic();
        removeSelf();
    }
    else if(button == logoutButton)
    {
        AnalyticsSingleton::getInstance()->logoutParentEvent();
        ParentDataParser::getInstance()->logoutChild();
        
        UserDefault::getInstance()->getStringForKey("password", "");
        UserDefault::getInstance()->flush();
        
        AudioMixer::getInstance()->stopBackgroundMusic();
        
        auto loginScene = LoginScene::createScene(0);
        Director::getInstance()->replaceScene(loginScene);
    }
    else
    {
        PaymentSingleton::getInstance()->startAmazonPayment();
    }
}

void ExitOrLogoutLayer::AdultPinCancelled(AwaitingAdultPinLayer* layer)
{
    removeSelf();
}

void ExitOrLogoutLayer::AdultPinAccepted(AwaitingAdultPinLayer* layer)
{
    addExitOrLogoutUIObjects();
}


/*void showDoublePurchase()
{
    auto funcCallAction = CallFunc::create([=](){
        MessageBox::createWith(ERROR_CODE_AMAZON_PURCHASE_DOUBLE, nullptr);
    });
    
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from amazon window, otherwise the app will crash
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseHappened(JNIEnv* env, jobject thiz, jstring requestId, jstring receiptId, jstring amazonUserid);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseHappened(JNIEnv* env, jobject thiz, jstring requestId, jstring receiptId, jstring amazonUserid)
{
    const char* cRequestId = env->GetStringUTFChars(requestId, NULL);
    const char* cReceiptId = env->GetStringUTFChars(receiptId, NULL);
    const char* cAmazonUserid = env->GetStringUTFChars(amazonUserid, NULL);
    
    CCLOG("COCOS2DX: I have the data: requestid: %s, receiptid: %s, amazonuserid: %s", cRequestId, cReceiptId, cAmazonUserid);
    
    PaymentSingleton::getInstance()->amazonPaymentMade(cRequestId, cReceiptId, cAmazonUserid);
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_alreadyPurchased(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_alreadyPurchased(JNIEnv* env, jobject thiz)
{
    CCLOG("COCOS2DX: alreadyPurchased CALLED!!!!!");
    showDoublePurchase();
}

#endif*/
