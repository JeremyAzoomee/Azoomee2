#include "LoginScene.h"
#include "AudioMixer.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"
#include "StringMgr.h"
#include "BaseScene.h"
#include "BackEndCaller.h"
#include "TextInputChecker.h"

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
    
    getUserDefaults();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addVisualElementsToScene();
    addLabelToScene();
    addTextboxScene();
    addButtonsScene();
    
    return true;
}

void LoginScene::onEnterTransitionDidFinish()
{

    
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
                
                //autoLogin(username, password);
                return;
            }
        }
    }
    else
    {
        UserDefault* def = UserDefault::getInstance();
        def->setStringForKey("password", "");
        def->flush();
    }
    
    if(_errorCode !=0)
    {
        MessageBox::createWith(_errorCode, emailTextInput, nullptr);
    }
    
    emailTextInput->focusAndShowKeyboard();
    
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

void LoginScene::addVisualElementsToScene()
{
    addGlowToScreen(this, 1);
    addSideWiresToScreen(this, 0, 2);
}

void LoginScene::addLabelToScene()
{
    auto versionTitle = createLabelAppVerison(APP_VERSION_NUMBER);
    versionTitle->setPosition(origin.x + visibleSize.width/2,versionTitle->getContentSize().height);
    this->addChild(versionTitle);

    title = createLabelHeader(StringMgr::getInstance()->getStringForKey(LOGINSCENE_EMAIL_LABEL));
    title->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.9);
    this->addChild(title);
}

void LoginScene::addTextboxScene()
{
    emailTextInput = TextInputLayer::createWithSize(Size(1500,131), INPUT_IS_EMAIL);
    emailTextInput->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.75));
    emailTextInput->setDelegate(this);
    emailTextInput->setText(username);
    this->addChild(emailTextInput);
    
    passwordTextInput = TextInputLayer::createWithSize(Size(1500,131), INPUT_IS_PASSWORD);
    passwordTextInput->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.75));
    passwordTextInput->setDelegate(this);
    passwordTextInput->setEditboxVisibility(false);
    this->addChild(passwordTextInput);
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
    
    if(username =="")
        nextButton->setVisible(false);

    this->addChild(nextButton);
}

//------------CHANGE INPUT----------------------
void LoginScene::changeElementsToPasswordScreen()
{
    title->setString(StringMgr::getInstance()->getStringForKey(LOGINSCENE_PASSWORD_LABEL));
    username = emailTextInput->getText();
    emailTextInput->setEditboxVisibility(false);
    passwordTextInput->setEditboxVisibility(true);
    nextButton->setVisible(false);
    currentScreen = passwordScreen;
    passwordTextInput->focusAndShowKeyboard();
}

void LoginScene::changeElementsToEmailScreen()
{
    title->setString(StringMgr::getInstance()->getStringForKey(LOGINSCENE_EMAIL_LABEL));
    passwordTextInput->setEditboxVisibility(false);
    passwordTextInput->setText("");
    emailTextInput->setEditboxVisibility(true);
    currentScreen = emailScreen;
    nextButton->setVisible(isValidEmailAddress(emailTextInput->getText().c_str()));
    emailTextInput->focusAndShowKeyboard();
}

void LoginScene::backButtonPressed()
{
    if(currentScreen == emailScreen)
    {
        auto baseScene = BaseScene::createScene();
        Director::getInstance()->replaceScene(baseScene);
    }
    else if(currentScreen == passwordScreen)
        changeElementsToEmailScreen();
}

void LoginScene::nextButtonPressed()
{
    if(currentScreen == emailScreen)
        changeElementsToPasswordScreen();
    else if(currentScreen == passwordScreen)
    {
        auto backEndCaller = BackEndCaller::getInstance();
        backEndCaller->login(username, passwordTextInput->getText());
    }
}

//-------------DELEGATE FUNCTIONS-------------------
void LoginScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    if(currentScreen == emailScreen && inputLayer == emailTextInput)
        nextButton->setVisible(isValid);
    else if(currentScreen == passwordScreen && inputLayer == passwordTextInput)
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
    
}
