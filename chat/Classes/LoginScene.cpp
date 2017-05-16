#include "LoginScene.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Input/TextInputChecker.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "ChildSelectorScene.h"

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

bool LoginScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    AudioMixer::getInstance()->stopBackgroundMusic();
    
    getUserDefaults();
    addTextboxScene();
    addSideWiresToScreen(this, 0, 2);
    addLabelToScene();
    addButtonsScene();
    
    return true;
}

void LoginScene::onEnterTransitionDidFinish()
{
    Super::onEnterTransitionDidFinish();
    changeElementsToEmailScreen();
}

void LoginScene::onEnter()
{
    Super::onEnter();
    
    // Register for API events
    AuthAPI::getInstance()->registerObserver(this);
}

void LoginScene::onExit()
{
    Super::onExit();
    
    // Unregister on chat API events
    AuthAPI::getInstance()->removeObserver(this);
}

#pragma mark - UI

void LoginScene::getUserDefaults()
{
    UserDefault* def = UserDefault::getInstance();
    storedUsername = def->getStringForKey("username", "");
    def->flush();
}

void LoginScene::addLabelToScene()
{
    auto versionTitle = createLabelAppVerison(StringUtils::format("Azoomee Chat %s", Azoomee::Chat::Version));
    this->addChild(versionTitle);

    title = createLabelHeader(StringMgr::getInstance()->getStringForKey(LOGINSCENE_EMAIL_LABEL));
    this->addChild(title);
}

void LoginScene::addTextboxScene()
{
    passwordTextInput = TextInputLayer::createWithSize(Size(1500,197), INPUT_IS_PASSWORD);
    passwordTextInput->setDelegate(this);
    passwordTextInput->setEditboxVisibility(false);
    this->addChild(passwordTextInput);
    
    emailTextInput = TextInputLayer::createWithSize(Size(1500,197), INPUT_IS_EMAIL);
    emailTextInput->setDelegate(this);
    emailTextInput->setText(storedUsername);
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

#pragma mark - Interactions

void LoginScene::changeElementsToPasswordScreen()
{
    title->setString(StringMgr::getInstance()->getStringForKey(LOGINSCENE_PASSWORD_LABEL));
    storedUsername = emailTextInput->getText();
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(storedUsername);
    emailTextInput->setEditboxVisibility(false);
    passwordTextInput->setEditboxVisibility(true);
    CCLOG("NEXT NextButton visible false - change to password");
    nextButton->setVisible(false);
    currentScreen = passwordLoginScreen;
    passwordTextInput->focusAndShowKeyboard();
    backButton->setVisible(true);
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
    backButton->setVisible(false);
}

void LoginScene::backButtonPressed()
{
    if(currentScreen == passwordLoginScreen)
    {
        changeElementsToEmailScreen();
    }
}

void LoginScene::nextButtonPressed()
{
    if(currentScreen == emailLoginScreen)
    {
        changeElementsToPasswordScreen();
    }
    else if(currentScreen == passwordLoginScreen)
    {
        login(storedUsername, passwordTextInput->getText());
    }
}

#pragma mark - Login

void LoginScene::login(const std::string& username, const std::string& password)
{
    AuthAPI::getInstance()->loginUser(username, password);
    ModalMessages::getInstance()->startLoading();
}

#pragma mark - TextInputLayerDelegate

void LoginScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    CCLOG("NEXT Check Input TextBox is %d", isValid);
    nextButton->setVisible(isValid);
}

#pragma mark - ElectricDreamsButtonDelegate

void LoginScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == nextButton)
        nextButtonPressed();
    else if(button == backButton)
        backButtonPressed();
}

#pragma mark - AuthAPIObserver

void LoginScene::onAuthAPILogin()
{
    // Login success
    auto childSelectScene = ChildSelectorScene::create();
    Director::getInstance()->replaceScene(childSelectScene);
}

NS_AZOOMEE_CHAT_END
