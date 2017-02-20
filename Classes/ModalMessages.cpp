#include "ModalMessages.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "StringStorage.h"
#include "LoginScene.h"
#include "OnboardingScene.h"
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include "MixPanelSingleton.h"

USING_NS_CC;

#define MESSAGE_BOX_PADDING 100
#define MESSAGE_BOX_MINIMUM_WIDTH 1366
#define MESSAGE_BOX_MAXIMUM_WIDTH 2049

static ModalMessages *_sharedModalMessages = NULL;

ModalMessages* ModalMessages::getInstance()
{
    if (! _sharedModalMessages)
    {
        _sharedModalMessages = new ModalMessages();
        _sharedModalMessages->init();
    }
    
    return _sharedModalMessages;
}

ModalMessages::~ModalMessages(void)
{
}

bool ModalMessages::init(void)
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

void ModalMessages::createAndFadeInLayer()
{
    loadingLayer = LayerColor::create(Color4B(0,0,0,150), visibleSize.width, visibleSize.height);
    loadingLayer->setPosition(origin.x, origin.y);
    loadingLayer->setOpacity(0);
    Director::getInstance()->getRunningScene()->addChild(loadingLayer);
    
    addListenerToBackgroundLayer();
    
    loadingLayer->runAction(FadeTo::create(0.5, 200));
}

void ModalMessages::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), loadingLayer);
}

void ModalMessages::removeLayer()
{
    if(loadingLayer) //This might be called when loading is not active, so better to check first
    {
        Director::getInstance()->getRunningScene()->removeChild(loadingLayer);
    }
}

void ModalMessages::startLoading()
{
    createAndFadeInLayer();
    
    //ADDING SOME TEMPORARY CIRCLES AS LOADING, NOT FINAL YET
    
    for(int i = 0; i < 3; i++)
    {
        auto loadingCircle = Sprite::create("res/modal/loading.png");
        loadingCircle->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        loadingCircle->setOpacity(0);
        loadingCircle->setRotation(RandomHelper::random_int(0, 360));
        loadingCircle->setScale(0.6 + i * 0.2);
        
        loadingLayer->addChild(loadingCircle);
        
        int direction = 1;
        if(CCRANDOM_0_1() < 0.5) direction = -1;
        
        loadingCircle->runAction(RepeatForever::create(RotateBy::create(CCRANDOM_0_1() + 1, 360 * direction)));
        loadingCircle->runAction(FadeTo::create(0.5, 255));
    }
}

void ModalMessages::stopLoading()
{
    this->removeLayer();
}

//-------------------- Object To Hide Functions ----------------------

void ModalMessages::hideTextInput(TextInputLayer* textInputToHide)
{
    if(textInputToHide)
    {
        textInputToHide->setEditboxVisibility(false);
        savedTextInputToHide = textInputToHide;
    }
    else
        savedTextInputToHide = nullptr;
}

void ModalMessages::UnHideTextInput()
{
    if(savedTextInputToHide)
        savedTextInputToHide->setEditboxVisibility(true);
}

//------------ PREVIEW MODE MESSAGE BOX ----------------
void ModalMessages::createPreviewLoginSignupMessageBox()
{
    if(messageBoxButtonTitles.size() == 0)
    {
        messageBoxButtonTitles.push_back(LOGIN_BUTTON_TEXT);
        messageBoxButtonTitles.push_back(SIGNUP_BUTTON_TEXT);
        messageBoxButtonTitles.push_back(CANCEL_BUTTON_TEXT);
    }
    
    MessageBox::createWith(PREVIEW_MESSAGEBOX_TITLE, PREVIEW_MESSAGEBOX_BODY, messageBoxButtonTitles, this);
}

//------------- SOMETHING WENT WRONG --------------------
void ModalMessages::createSomethingWentWrongMessage()
{
    std::map<std::string, std::string> errorStringMap = StringStorage::getInstance()->getErrorMessageStrings(-2);
    
    if(messageBoxButtonTitles.size() == 0)
    {
        messageBoxButtonTitles.push_back(errorStringMap[ERROR_BUTTON]);
    }
    
    //setting so delegate can find these
    somethingWentWrongTitle = errorStringMap[ERROR_TITLE];
    somethingWentWrongButton = errorStringMap[ERROR_BODY];
    
    MessageBox::createWith(errorStringMap[ERROR_TITLE], errorStringMap[ERROR_BODY], messageBoxButtonTitles, this);
}

// ------------- DELEGATE FUNCTIONS --------------------

void ModalMessages::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(messageBoxTitle == PREVIEW_MESSAGEBOX_TITLE)
    {
        if(buttonTitle == LOGIN_BUTTON_TEXT)
        {
            Scene *loginScene = LoginScene::createScene(0);
            Director::getInstance()->replaceScene(loginScene);
        }
        else if(buttonTitle == SIGNUP_BUTTON_TEXT)
        {
            Scene *onboardingScene = OnboardingScene::createScene(0);
            Director::getInstance()->replaceScene(onboardingScene);
        }
        else if(buttonTitle == CANCEL_BUTTON_TEXT)
        {
            MixPanelSingleton::getInstance()->mixPanel_previewPopupCancelledEvent();
        }
    }
    else if(messageBoxTitle == somethingWentWrongTitle)
    {
        if(buttonTitle == somethingWentWrongButton)
        {
            HQHistoryManager::getInstance()->emptyHistory();
            auto baseScene = BaseScene::createScene();
            Director::getInstance()->replaceScene(baseScene);
        }
    }
}
