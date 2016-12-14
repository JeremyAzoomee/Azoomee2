#include "OnboardingScene.h"
#include "SimpleAudioEngine.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"
#include "LoginScene.h"

USING_NS_CC;

//Text Content defined here, ready to be changed when localisation method defined.
#define EMAIL_LABEL "Registration Email Address"
#define PASSWORD_LABEL "Create your Azoomee Password"
#define PIN_LABEL "Create your Azoomee App 4 digit pin"
#define PIN_LABEL_DETAIL "Shhh! Don't tell the kids.."

//Define Editbox Tag Numbers
#define TAG_EMAIL_EDITBOX 1
#define TAG_PASSWORD_EDITBOX 2
#define TAG_PIN_EDITBOX 3

//Define BUTTONS Tag Numbers
#define TAG_EMAIL_BACK_BUTTON 10
#define TAG_EMAIL_NEXT_BUTTON 11
#define TAG_PASSWORD_BACK_BUTTON 12
#define TAG_PASSWORD_NEXT_BUTTON 13
#define TAG_PIN_BACK_BUTTON 14
#define TAG_PIN_NEXT_BUTTON 15

Scene* OnboardingScene::createScene(long errorCode)
{
    //Create with 0 (Zero) if there are no errors
    
    auto scene = Scene::create();
    auto layer = OnboardingScene::create();
    scene->addChild(layer);
    
    if(errorCode !=0)
    {
        layer->handleErrorCode(errorCode);
    }
    
    return scene;
}

bool OnboardingScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addVisualElementsToScene();
    addFunctionalElementsToScene();
    
    return true;
}


void OnboardingScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
}

void OnboardingScene::handleErrorCode(long errorCode)
{
    //#TODO have a Modal layer with error, for now just add a label to the Layer
    
    auto errorDetail = Label::createWithTTF(StringUtils::format("ERROR: %ld",errorCode), "fonts/azoomee.ttf", 60);
    errorDetail->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.6);
    errorDetail->setColor(Color3B::WHITE);
    onboardingContent->addChild(errorDetail);
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
    auto emailTitle = Label::createWithTTF(EMAIL_LABEL, "fonts/azoomee.ttf", 90);
    emailTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    emailTitle->setColor(Color3B(28, 244, 244));
    onboardingContent->addChild(emailTitle);
    
    auto passwordTitle = Label::createWithTTF(PASSWORD_LABEL, "fonts/azoomee.ttf", 90);
    passwordTitle->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    passwordTitle->setColor(Color3B(28, 244, 244));
    onboardingContent->addChild(passwordTitle);
    
    auto pinTitle = Label::createWithTTF(PIN_LABEL, "fonts/azoomee.ttf", 90);
    pinTitle->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    pinTitle->setColor(Color3B(28, 244, 244));
    onboardingContent->addChild(pinTitle);
    
    auto pinDetail = Label::createWithTTF(PIN_LABEL_DETAIL, "fonts/azoomee.ttf", 60);
    pinDetail->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.6);
    pinDetail->setColor(Color3B::WHITE);
    onboardingContent->addChild(pinDetail);
}

void OnboardingScene::addContentLayerToScene()
{
    //we add an additional layer that will slide on the screen between the username and the password.
    //we add all input elements on this layer.
    
    onboardingContent = Layer::create();
    onboardingContent->setContentSize(Size(visibleSize.width * 3, visibleSize.height));
    onboardingContent->setPosition(Point(origin.x, origin.y));
    onboardingContent->setName("onboardingContent");
    this->addChild(onboardingContent);
}

void OnboardingScene::addTextBoxesToLayer()
{
    auto _editName = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editName->setColor(Color3B::WHITE);
    _editName->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.5));
    _editName->setFont("fonts/azoomee.ttf", 90);
    _editName->setFontColor(Color3B::WHITE);
    _editName->setMaxLength(100);
    _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editName->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
    _editName->setTag(TAG_EMAIL_EDITBOX);
    _editName->setDelegate(this);
    onboardingContent->addChild(_editName);
    
    auto _editPassword = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editPassword->setPosition(Vec2(origin.x+visibleSize.width * 1.5, origin.y+visibleSize.height*0.5));
    _editPassword->setFont("fonts/azoomee.ttf", 90);
    _editPassword->setFontColor(Color3B::WHITE);
    _editPassword->setMaxLength(50);
    _editPassword->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editPassword->setTag(TAG_PASSWORD_EDITBOX);
    _editPassword->setDelegate(this);
    onboardingContent->addChild(_editPassword);
    
    auto _editPin = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editPin->setPosition(Vec2(origin.x+visibleSize.width * 2.5, origin.y+visibleSize.height*0.5));
    _editPin->setFont("fonts/azoomee.ttf", 90);
    _editPin->setFontColor(Color3B::WHITE);
    _editPin->setMaxLength(4);
    _editPin->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editPin->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPin->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editPin->setTag(TAG_PIN_EDITBOX);
    _editPin->setDelegate(this);
    onboardingContent->addChild(_editPin);
}

void OnboardingScene::addButtonsToLayer()
{
    // in order they appear on the screen with next, then back, then next etc.
    auto emailBackButton = Sprite::create("res/login/back_btn.png");
    emailBackButton->setPosition(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.5);
    emailBackButton->setTag(TAG_EMAIL_BACK_BUTTON);
    addListenerToButton(emailBackButton);
    onboardingContent->addChild(emailBackButton);
    
    auto emailNextButton = Sprite::create("res/login/next_btn.png");
    emailNextButton->setPosition(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.5);
    emailNextButton->setTag(TAG_EMAIL_NEXT_BUTTON);
    addListenerToButton(emailNextButton);
    emailNextButton->setVisible(false);
    onboardingContent->addChild(emailNextButton);
    
    auto passwordBackButton = Sprite::create("res/login/back_btn.png");
    passwordBackButton->setPosition(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5);
    passwordBackButton->setTag(TAG_PASSWORD_BACK_BUTTON);
    addListenerToButton(passwordBackButton);
    onboardingContent->addChild(passwordBackButton);
    
    auto passwordNextButton = Sprite::create("res/login/next_btn.png");
    passwordNextButton->setPosition(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5);
    passwordNextButton->setTag(TAG_PASSWORD_NEXT_BUTTON);
    addListenerToButton(passwordNextButton);
    passwordNextButton->setVisible(false);
    onboardingContent->addChild(passwordNextButton);
    
    auto pinBackButton = Sprite::create("res/login/back_btn.png");
    pinBackButton->setPosition(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5);
    pinBackButton->setTag(TAG_PIN_BACK_BUTTON);
    addListenerToButton(pinBackButton);
    onboardingContent->addChild(pinBackButton);
    
    auto pinNextButton = Sprite::create("res/login/next_btn.png");
    pinNextButton->setPosition(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5);
    pinNextButton->setTag(TAG_PIN_NEXT_BUTTON);
    addListenerToButton(pinNextButton);
    pinNextButton->setVisible(false);
    pinNextButton->setScale(1.2);
    onboardingContent->addChild(pinNextButton);

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
        
        if(rect.containsPoint(locationInNode))
        {
            int buttonTag = spriteImage->getTag();
            
            switch(buttonTag)
            {
                case(TAG_EMAIL_BACK_BUTTON): this->closeOnboarding(); break;
                case(TAG_EMAIL_NEXT_BUTTON): this->moveToPasswordScreen(spriteImage); break;
                case(TAG_PASSWORD_BACK_BUTTON): this->moveToEmailScreen(spriteImage); break;
                case(TAG_PASSWORD_NEXT_BUTTON): this->moveToPinScreen(spriteImage); break;
                case(TAG_PIN_BACK_BUTTON): this->moveToPasswordScreen(spriteImage); break;
                case(TAG_PIN_NEXT_BUTTON): this->signup(); break;
            }
            return true;
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
    auto _loginScene = LoginScene::createScene();
    Director::getInstance()->replaceScene(_loginScene);
}

void OnboardingScene::moveToPasswordScreen(Node* button)
{
    disableMoveButton(button);
    
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(OnboardingScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    onboardingContent->runAction(sequence);
    
    //Remove PIN Text, therefore Hide Pin Next Button
    ((ui::EditBox *)onboardingContent->getChildByTag(TAG_PIN_EDITBOX))->setText("");
    ((cocos2d::Sprite *)onboardingContent->getChildByTag(TAG_PIN_NEXT_BUTTON))->setVisible(false);
}

void OnboardingScene::moveToEmailScreen(Node* button)
{
    disableMoveButton(button);
    
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(OnboardingScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    onboardingContent->runAction(sequence);
    
    //Remove password Text, therefore Hide Password Next Button
    ((ui::EditBox *)onboardingContent->getChildByTag(TAG_PASSWORD_EDITBOX))->setText("");
    ((cocos2d::Sprite *)onboardingContent->getChildByTag(TAG_PASSWORD_NEXT_BUTTON))->setVisible(false);
}

void OnboardingScene::moveToPinScreen(Node* button)
{
    disableMoveButton(button);
    
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width*2 + origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(OnboardingScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    onboardingContent->runAction(sequence);
}

void OnboardingScene::signup()
{
    std::string username = ((ui::EditBox *)onboardingContent->getChildByTag(TAG_EMAIL_EDITBOX))->getText();
    std::string password = ((ui::EditBox *)onboardingContent->getChildByTag(TAG_PASSWORD_EDITBOX))->getText();
    std::string pin = ((ui::EditBox *)onboardingContent->getChildByTag(TAG_PIN_EDITBOX))->getText();
    
    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->registerParent(username, password,pin);
}

//Editbox Delegate Functions
void OnboardingScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    //Make the next buttons visible if the inputs are in correct format
    
    if(editBox->getTag() == TAG_EMAIL_EDITBOX)
    {
        //Set the Visibility of the Next Button to visible, if Valid email.
        ((cocos2d::Sprite *)onboardingContent->getChildByTag(TAG_EMAIL_NEXT_BUTTON))->setVisible(isValidEmailAddress(text.c_str()));
    }
    else if(editBox->getTag() == TAG_PASSWORD_EDITBOX)
    {
        //Set the Visibility of the Next Button to visible, if Valid Password
        ((cocos2d::Sprite *)onboardingContent->getChildByTag(TAG_PASSWORD_NEXT_BUTTON))->setVisible(isValidPassword(text.c_str()));
    }
    else if(editBox->getTag() == TAG_PIN_EDITBOX)
     {
     //Set the Visibility of the Next Button to visible, if Valid PIN
     ((cocos2d::Sprite *)onboardingContent->getChildByTag(TAG_PIN_NEXT_BUTTON))->setVisible(isValidPin(text.c_str()));
     }
}

void OnboardingScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    //This function only really works correctly for Email_Editbox, as it has an email address
    //Otherwise detect the \n with the text changed in the other EditBoxes
}

//INPUT CHECKING FUNCTIONS
bool OnboardingScene::isCharacter(const char Character)
{
    //Borrowed from internet With isValidEmailAddress to check email address format
    
    return ( (Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
    //Checks if a Character is a Valid A-Z, a-z Character, based on the ascii value
}
bool OnboardingScene::isNumber(const char Character)
{
    return ( Character >= '0' && Character <= '9');
    //Checks if a Character is a Valid 0-9 Number, based on the ascii value
}

bool OnboardingScene::isValidEmailAddress(const char * EmailAddress)
{
    //Borrowed from internet With isValidEmailAddress to check email address format
    
    if(!EmailAddress) // If cannot read the Email Address...
        return 0;
    if(!isCharacter(EmailAddress[0])) // If the First character is not A-Z, a-z
        return 0;
    int AtOffset = -1;
    int DotOffset = -1;
    unsigned int Length = (int)strlen(EmailAddress); // Length = StringLength (strlen) of EmailAddress
    for(unsigned int i = 0; i < Length; i++)
    {
        if(EmailAddress[i] == '@') // If one of the characters is @, store it's position in AtOffset
            AtOffset = (int)i;
        else if(EmailAddress[i] == '.') // Same, but with the dot
            DotOffset = (int)i;
    }
    if(AtOffset == -1 || DotOffset == -1) // If cannot find a Dot or a @
        return 0;
    if(AtOffset > DotOffset) // If the @ is after the Dot
        return 0;
    return !(DotOffset >= ((int)Length-1)); //Chech there is some other letters after the Dot
}

bool OnboardingScene::isValidPassword(const char * password)
{
    //check at least 2 chars and no white space
    if(!password) // If cannot read the password
        return 0;
    
    if(strlen(password) < 2) // ensure there are 2 or more characters
        return 0;
    
    bool passwordOK = true;
    
    for(unsigned int i = 0; i < strlen(password); i++)
    {
        if(password[i] == ' ')
            passwordOK = false;
        else if(password[i] == '\n')
            passwordOK = false;
        else if(password[i] == '\t')
            passwordOK = false;
    }
    
    if(passwordOK) // If all characters are not white space
        return 1;
    else
        return 0;
    
}

bool OnboardingScene::isValidPin(const char * pinNumber)
{
    if(!pinNumber) // If cannot read the Email Address...
        return 0;

    if(strlen(pinNumber) != 4) // ensure there are 4 characters
        return 0;
    
    bool pinOK = true;
    
    for(unsigned int i = 0; i < strlen(pinNumber); i++)
    {
        if(!isNumber(pinNumber[i])) // the charactor is not a number, set to false.
            pinOK = false;
    }
    if(pinOK) // If all characters are numbers return true.
        return 1;
    else
        return 0;

}
