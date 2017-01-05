#include "OnboardingScene.h"
#include "SimpleAudioEngine.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"
#include "LoginScene.h"
#include "TextInputChecker.h"
#include "StringStorage.h"

USING_NS_CC;

Scene* OnboardingScene::createScene(long errorCode)
{
    auto scene = Scene::create();
    auto layer = OnboardingScene::create();
    scene->addChild(layer);
    
    layer->_errorCode = errorCode;
    
    return scene;
}

bool OnboardingScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    addVisualElementsToScene();
    addFunctionalElementsToScene();
    
    return true;
}

void OnboardingScene::onEnterTransitionDidFinish()
{
    if(_errorCode !=0)
    {
        handleErrorCode(_errorCode);
    }
}

//-----------------------------------------All methods beyond this line are called internally----------------------------------------------------

void OnboardingScene::handleErrorCode(long errorCode)
{
    ModalMessages::getInstance()->createMessageWithSingleButton("ERROR", StringUtils::format("Error Code:%ld",errorCode), "OK");
}

void OnboardingScene::addVisualElementsToScene()
{
    auto bg = Sprite::create("res/mainhub/bg_glow.png");
    bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(bg);
    
    auto leftBg = Sprite::create("res/login/wire_left.png");
    leftBg->setPosition(0 + leftBg->getContentSize().width / 2,0);
    this->addChild(leftBg);
    
    auto rightBg = Sprite::create("res/login/wire_right.png");
    rightBg->setPosition(origin.x + visibleSize.width - rightBg->getContentSize().width / 2, 0);
    this->addChild(rightBg);
}

void OnboardingScene::addFunctionalElementsToScene()
{
    addContentLayerToScene();
    addLabelsToLayer();
    addTextBoxesToLayer();
    addButtonsToLayer();
}

void OnboardingScene::addLabelsToLayer()
{
    auto emailTitle = Label::createWithTTF(StringStorage::getInstance()->getStringForOnboarding("email"), "fonts/azoomee.ttf", 90);
    emailTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    emailTitle->setColor(Color3B(28, 244, 244));
    onboardingContent->addChild(emailTitle);
    
    auto passwordTitle = Label::createWithTTF(StringStorage::getInstance()->getStringForOnboarding("password"), "fonts/azoomee.ttf", 90);
    passwordTitle->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    passwordTitle->setColor(Color3B(28, 244, 244));
    onboardingContent->addChild(passwordTitle);
    
    auto pinTitle = Label::createWithTTF(StringStorage::getInstance()->getStringForOnboarding("pintitle"), "fonts/azoomee.ttf", 90);
    pinTitle->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    pinTitle->setColor(Color3B(28, 244, 244));
    onboardingContent->addChild(pinTitle);
    
    auto pinDetail = Label::createWithTTF(StringStorage::getInstance()->getStringForOnboarding("pinsubtitle"), "fonts/azoomee.ttf", 60);
    pinDetail->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.6);
    pinDetail->setColor(Color3B::WHITE);
    onboardingContent->addChild(pinDetail);
}

void OnboardingScene::addContentLayerToScene()
{
    onboardingContent = Layer::create();
    onboardingContent->setContentSize(Size(visibleSize.width * 3, visibleSize.height));
    onboardingContent->setPosition(Point(origin.x, origin.y));
    onboardingContent->setName("onboardingContent");
    this->addChild(onboardingContent);
}

void OnboardingScene::addTextBoxesToLayer()
{
    editBox_email = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    editBox_email->setColor(Color3B::WHITE);
    editBox_email->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.5));
    editBox_email->setFont("fonts/azoomee.ttf", 90);
    editBox_email->setFontColor(Color3B::WHITE);
    editBox_email->setMaxLength(100);
    editBox_email->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    editBox_email->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
    editBox_email->setDelegate(this);
    onboardingContent->addChild(editBox_email);
    
    editBox_password = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    editBox_password->setPosition(Vec2(origin.x+visibleSize.width * 1.5, origin.y+visibleSize.height*0.5));
    editBox_password->setFont("fonts/azoomee.ttf", 90);
    editBox_password->setFontColor(Color3B::WHITE);
    editBox_password->setMaxLength(50);
    editBox_password->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    editBox_password->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    editBox_password->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    editBox_password->setDelegate(this);
    onboardingContent->addChild(editBox_password);
    
    editBox_pin = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    editBox_pin->setPosition(Vec2(origin.x+visibleSize.width * 2.5, origin.y+visibleSize.height*0.5));
    editBox_pin->setFont("fonts/azoomee.ttf", 90);
    editBox_pin->setFontColor(Color3B::WHITE);
    editBox_pin->setMaxLength(4);
    editBox_pin->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    editBox_pin->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    editBox_pin->setInputMode(ui::EditBox::InputMode::NUMERIC);
    editBox_pin->setDelegate(this);
    onboardingContent->addChild(editBox_pin);
}

void OnboardingScene::addButtonsToLayer()
{
    // in order they appear on the screen with next, then back, then next etc.
    buttonBackEmail = Sprite::create("res/login/back_btn.png");
    buttonBackEmail->setPosition(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.5);
    addListenerToButton(buttonBackEmail);
    onboardingContent->addChild(buttonBackEmail);
    
    buttonNextEmail = Sprite::create("res/login/next_btn.png");
    buttonNextEmail->setPosition(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.5);
    addListenerToButton(buttonNextEmail);
    buttonNextEmail->setVisible(false);
    onboardingContent->addChild(buttonNextEmail);
    
    buttonBackPassword = Sprite::create("res/login/back_btn.png");
    buttonBackPassword->setPosition(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5);
    addListenerToButton(buttonBackPassword);
    onboardingContent->addChild(buttonBackPassword);
    
    buttonNextPassword = Sprite::create("res/login/next_btn.png");
    buttonNextPassword->setPosition(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5);
    addListenerToButton(buttonNextPassword);
    buttonNextPassword->setVisible(false);
    onboardingContent->addChild(buttonNextPassword);
    
    buttonBackPin = Sprite::create("res/login/back_btn.png");
    buttonBackPin->setPosition(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5);
    addListenerToButton(buttonBackPin);
    onboardingContent->addChild(buttonBackPin);
    
    buttonNextPin = Sprite::create("res/login/next_btn.png");
    buttonNextPin->setPosition(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5);
    addListenerToButton(buttonNextPin);
    buttonNextPin->setVisible(false);
    buttonNextPin->setScale(1.2);
    onboardingContent->addChild(buttonNextPin);

}

void OnboardingScene::addListenerToButton(cocos2d::Sprite *spriteImage)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Rect rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
        
        if(rect.containsPoint(locationInNode) && spriteImage->isVisible())
        {
            if(spriteImage == buttonBackEmail) this->closeOnboarding();
            else if(spriteImage == buttonNextPin) this->signup();
            else this->moveToScreen(spriteImage);
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, spriteImage);
}

void OnboardingScene::disableMoveButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(button);
}

void OnboardingScene::enableMoveButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(button);
}

void OnboardingScene::closeOnboarding()
{
    auto _loginScene = LoginScene::createScene(0);
    Director::getInstance()->replaceScene(_loginScene);
}

void OnboardingScene::cleanPasswordScreen()
{
    editBox_pin->setText("");
    buttonNextPin->setVisible(false);
}

void OnboardingScene::cleanEmailScreen()
{
    editBox_password->setText("");
    buttonNextPassword->setVisible(false);
}

void OnboardingScene::cleanPinScreen()
{
    editBox_pin->setText("");
    buttonNextPin->setVisible(false);
}

void OnboardingScene::moveToScreen(cocos2d::Sprite *sender)
{
    Point target;
    
    if((sender == buttonNextEmail)||(sender == buttonBackPin))
    {
        target = Point(-visibleSize.width + origin.x, origin.y);
        cleanPasswordScreen();
    }
    
    if(sender == buttonBackPassword)
    {
        target = Point(origin.x, origin.y);
        cleanEmailScreen();
    }
    
    if(sender == buttonNextPassword)
    {
        target = Point(-visibleSize.width * 2 + origin.x, origin.y);
        cleanPinScreen();
    }
    
    disableMoveButton((Node *) sender);
    
    auto action = EaseInOut::create(MoveTo::create(1, target), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(OnboardingScene::enableMoveButton, this, sender));
    
    auto sequence = Sequence::create(action, callback, NULL);
    onboardingContent->runAction(sequence);
}

void OnboardingScene::signup()
{
    std::string username = editBox_email->getText();
    std::string password = editBox_password->getText();
    std::string pin = editBox_pin->getText();
    
    BackEndCaller::getInstance()->registerParent(username, password,pin);
}

//Editbox Delegate Functions
void OnboardingScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    
    if(editBox == editBox_email)
    {
        bool canGoForward = isValidEmailAddress(text.c_str());
        buttonNextEmail->setVisible(canGoForward);
    }
    else if(editBox == editBox_password)
    {
        bool canGoForward = isValidPassword(text.c_str());
        buttonNextPassword->setVisible(canGoForward);
    }
    else if(editBox == editBox_pin)
    {
        bool canGoForward = isValidPin(text.c_str());
        buttonNextPin->setVisible(canGoForward);
    }

}

void OnboardingScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    //This function only really works correctly for Email_Editbox, as it has an email address
    //Otherwise detect the \n with the text changed in the other EditBoxes
}
