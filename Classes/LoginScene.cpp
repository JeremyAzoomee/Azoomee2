#include "LoginScene.h"
#include "AudioMixer.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"
#include "StringMgr.h"
#include "BaseScene.h"
#include "BackEndCaller.h"
#include "TextInputChecker.h"
#include "OfflineHubScene.h"
#include "HQHistoryManager.h"
#include "AnalyticsSingleton.h"

USING_NS_CC;

Scene* LoginScene::createScene(long errorCode)
{
    auto scene = Scene::create();
    auto layer = LoginScene::create();
    scene->addChild(layer);
    
    layer->_errorCode = errorCode;
    layer->shouldDoAutoLogin = false;
    
    return scene;
}

Scene* LoginScene::createSceneWithAutoLogin()
{
    auto scene = Scene::create();
    auto layer = LoginScene::create();
    scene->addChild(layer);
    
    layer->_errorCode = 0;
    layer->shouldDoAutoLogin = true;
    
    return scene;
}

Scene* LoginScene::createSceneWithAutoLoginAndErrorDisplay()
{
    auto scene = Scene::create();
    auto layer = LoginScene::create();
    scene->addChild(layer);
    
    layer->_errorCode = 0;
    layer->shouldDoAutoLogin = true;
    layer->shouldDisplayMessage = true;
    
    return scene;
}

bool LoginScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    CCLOG("Cache folder: %s", FileUtils::getInstance()->getWritablePath().c_str());
    
    AudioMixer::getInstance()->stopBackgroundMusic();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    getUserDefaults();
    addTextboxScene();
    addSideWiresToScreen(this, 0, 2);
    addLabelToScene();
    addButtonsScene();
    
    return true;
}

void LoginScene::onEnterTransitionDidFinish()
{
    currentScreen = emailLoginScreen;
    
    OfflineChecker::getInstance()->setDelegate(this);
    
    if(shouldDoAutoLogin)
    {
        CCLOG("Should do autologin!");
        
        if((username != "")&&(password != ""))
        {
            if(shouldDisplayMessage)
            {
                MessageBox::createWith(ERROR_CODE_LOGGED_YOU_BACK_IN, this);
                return;
            }
            else
            {
                CCLOG("Doing autologin!");
                login();
                return;
            }
        }
    }
    else
    {
        UserDefault* def = UserDefault::getInstance();
        def->setStringForKey("password", "");
        def->flush();
        password = "";
    }
    
    if(_errorCode !=0)
    {
        MessageBox::createWith(_errorCode, emailTextInput, this);
    }
    else
        emailTextInput->focusAndShowKeyboard();
    
    nextButton->setVisible(isValidEmailAddress(username.c_str()));
    
#ifdef autologin
    BackEndCaller::getInstance()->login("tamas.bonis@azoomee.com", "B0Ta1983!");
#endif
}

//----------------- SCENE SETUP ---------------

void LoginScene::getUserDefaults()
{
    UserDefault* def = UserDefault::getInstance();
    username = def->getStringForKey("username", "");
    password = def->getStringForKey("password", "");
    def->flush();
}

void LoginScene::addLabelToScene()
{
    auto versionTitle = createLabelAppVerison(APP_VERSION_NUMBER);
    this->addChild(versionTitle);

    title = createLabelHeader(StringMgr::getInstance()->getStringForKey(LOGINSCENE_EMAIL_LABEL));
    this->addChild(title);
}

void LoginScene::addTextboxScene()
{
    passwordTextInput = TextInputLayer::createWithSize(Size(1500,131), INPUT_IS_PASSWORD);
    passwordTextInput->setDelegate(this);
    passwordTextInput->setEditboxVisibility(false);
    this->addChild(passwordTextInput);
    
    emailTextInput = TextInputLayer::createWithSize(Size(1500,131), INPUT_IS_EMAIL);
    emailTextInput->setDelegate(this);
    emailTextInput->setText(username);
    this->addChild(emailTextInput);
}

void LoginScene::addButtonsScene()
{
    backButton = ElectricDreamsButton::createBackButton();
    backButton->setCenterPosition(Vec2(origin.x +backButton->getContentSize().width*.7, origin.y + visibleSize.height - backButton->getContentSize().height*.7));
    backButton->setDelegate(this);
    this->addChild(backButton);
    
    nextButton = ElectricDreamsButton::createNextButton();
    nextButton->setCenterPosition(Vec2(origin.x + visibleSize.width -nextButton->getContentSize().width*.7, origin.y+ visibleSize.height - nextButton->getContentSize().height*.7));
    nextButton->setDelegate(this);
    this->addChild(nextButton);
}

//------------CHANGE SCREEN VISUALS ON BUTTON PRESS----------------------
void LoginScene::changeElementsToPasswordScreen()
{
    title->setString(StringMgr::getInstance()->getStringForKey(LOGINSCENE_PASSWORD_LABEL));
    username = emailTextInput->getText();
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(username);
    emailTextInput->setEditboxVisibility(false);
    passwordTextInput->setEditboxVisibility(true);
    CCLOG("NEXT NextButton visible false - change to password");
    nextButton->setVisible(false);
    currentScreen = passwordLoginScreen;
    passwordTextInput->focusAndShowKeyboard();
}

void LoginScene::changeElementsToEmailScreen()
{
    title->setString(StringMgr::getInstance()->getStringForKey(LOGINSCENE_EMAIL_LABEL));
    passwordTextInput->setEditboxVisibility(false);
    passwordTextInput->setText("");
    emailTextInput->setEditboxVisibility(true);
    currentScreen = emailLoginScreen;
    CCLOG("NEXT NextButton visible change to email %d",isValidEmailAddress(emailTextInput->getText().c_str()));
    nextButton->setVisible(isValidEmailAddress(emailTextInput->getText().c_str()));
    emailTextInput->focusAndShowKeyboard();
}

void LoginScene::backButtonPressed()
{
    if(currentScreen == emailLoginScreen)
    {
        HQHistoryManager::getInstance()->emptyHistory();
        auto baseScene = BaseScene::createScene();
        Director::getInstance()->replaceScene(baseScene);
    }
    else if(currentScreen == passwordLoginScreen)
        changeElementsToEmailScreen();
}

void LoginScene::nextButtonPressed()
{
    if(currentScreen == emailLoginScreen)
        changeElementsToPasswordScreen();
    else if(currentScreen == passwordLoginScreen)
    {
        password = passwordTextInput->getText();
        login();
    }
}

//------------PRIVATE OTHER FUNCTIONS------------

void LoginScene::login()
{
    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->login(username, password);
}

//-------------DELEGATE FUNCTIONS-------------------
void LoginScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    CCLOG("NEXT Check Input TextBox is %d", isValid);
    nextButton->setVisible(isValid);
}
void LoginScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == nextButton)
        nextButtonPressed();
    else if(button == backButton)
        backButtonPressed();
}
void LoginScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(messageBoxTitle == StringMgr::getInstance()->getErrorMessageWithCode(ERROR_CODE_LOGGED_YOU_BACK_IN)[ERROR_TITLE])
        login();
    else
        emailTextInput->focusAndShowKeyboard();
}

void LoginScene::connectivityStateChanged(bool online)
{
    if(!online)
    {
        Director::getInstance()->replaceScene(OfflineHubScene::createScene());
    }
}
