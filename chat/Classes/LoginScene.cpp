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
    
    origin = Vec2::ZERO;
    AudioMixer::getInstance()->stopBackgroundMusic();
    
    visibleSize = getContentSize();
    
    wiresLayer = Layer::create();
    wiresLayer->setPosition(Director::getInstance()->getVisibleOrigin() * -1.0f);
    addChild(wiresLayer);
    
    inputLayer = Layer::create();
    addChild(inputLayer);
    
    getUserDefaults();
    addTextboxScene();
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
    
    changeElementsToEmailScreen();
}

void LoginScene::onExit()
{
    Super::onExit();
    
    // Unregister on chat API events
    AuthAPI::getInstance()->removeObserver(this);
}

#pragma mark - Size Changes

void LoginScene::onSizeChanged()
{
    Super::onSizeChanged();
    
    visibleSize = getContentSize();
    
    // If initialised
    if(emailTextInput)
    {
        wiresLayer->removeAllChildren();
        wiresLayer->setPosition(Director::getInstance()->getVisibleOrigin() * -1.0f);
        addSideWiresToScreen(wiresLayer, 0, 2);
        
        versionLabel->setPosition(visibleSize.width/2, versionLabel->getContentSize().height);
        titleLabel->setPosition(visibleSize.width/2, visibleSize.height * 0.9);
        backButton->setCenterPosition(Vec2(backButton->getContentSize().width*.7, visibleSize.height - backButton->getContentSize().height*.7));
        nextButton->setCenterPosition(Vec2(visibleSize.width -nextButton->getContentSize().width*.7, visibleSize.height - nextButton->getContentSize().height*.7));
        
        // Horribly, we have to re-create the editboxes. If we don't the native OS view gets stuck offset
        std::string username = emailTextInput->getText();
        std::string password = passwordTextInput->getText();
        inputLayer->removeAllChildren();
        addTextboxScene();
        
        emailTextInput->setText(username);
        passwordTextInput->setText(password);
        
        emailTextInput->setEditboxVisibility(currentScreen == LoginScreenLocationEnum::emailLoginScreen);
        passwordTextInput->setEditboxVisibility(currentScreen == LoginScreenLocationEnum::passwordLoginScreen);
    }
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
    versionLabel = createLabelAppVerison(StringUtils::format("Azoomee Chat %s", Azoomee::Chat::Version));
//    versionLabel->setPosition(versionLabel->getPosition() - Director::getInstance()->getVisibleOrigin());
    this->addChild(versionLabel);

    titleLabel = createLabelHeader(_("Log in. Please enter your email."));
//    titleLabel->setPosition(titleLabel->getPosition() - Director::getInstance()->getVisibleOrigin());
    this->addChild(titleLabel);
}

void LoginScene::addTextboxScene()
{
    passwordTextInput = TextInputLayer::createWithSize(Size(1500,197), INPUT_IS_PASSWORD);
    passwordTextInput->setDelegate(this);
    passwordTextInput->setEditboxVisibility(false);
    inputLayer->addChild(passwordTextInput);
    
    emailTextInput = TextInputLayer::createWithSize(Size(1500,197), INPUT_IS_EMAIL);
    emailTextInput->setDelegate(this);
    emailTextInput->setText(storedUsername);
    inputLayer->addChild(emailTextInput);
    
    emailTextInput->setCenterPosition(Vec2(visibleSize.width/2, visibleSize.height*0.70));
    passwordTextInput->setCenterPosition(Vec2(visibleSize.width/2, visibleSize.height*0.70));
}

void LoginScene::addButtonsScene()
{
    backButton = ElectricDreamsButton::createBackButtonGreen();
    backButton->setDelegate(this);
    this->addChild(backButton);
    
    nextButton = ElectricDreamsButton::createNextButtonGreen();
    nextButton->setDelegate(this);
    this->addChild(nextButton);
}

#pragma mark - Interactions

void LoginScene::changeElementsToPasswordScreen()
{
    titleLabel->setString(_("Password"));
    storedUsername = emailTextInput->getText();
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(storedUsername);
    emailTextInput->setEditboxVisibility(false);
    passwordTextInput->setEditboxVisibility(true);
    nextButton->setVisible(false);
    currentScreen = passwordLoginScreen;
    passwordTextInput->focusAndShowKeyboard();
    backButton->setVisible(true);
}

void LoginScene::changeElementsToEmailScreen()
{
    titleLabel->setString(_("Log in. Please enter your email."));
    passwordTextInput->setEditboxVisibility(false);
    passwordTextInput->setText("");
    emailTextInput->setEditboxVisibility(true);
    currentScreen = emailLoginScreen;
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
    nextButton->setVisible(isValid);
}

void LoginScene::textInputReturnPressed(TextInputLayer* inputLayer)
{
    nextButtonPressed();
}

void LoginScene::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    // Nothing
}

void LoginScene::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    // Nothing
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

void LoginScene::onAuthAPIRequestFailed(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(errorCode, nullptr);
    changeElementsToEmailScreen();
}

NS_AZOOMEE_CHAT_END
