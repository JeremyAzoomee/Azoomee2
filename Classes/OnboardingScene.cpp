#include "OnboardingScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"

USING_NS_CC;

//Text Content defined here, ready to be changed when localisation method defined.
#define EMAIL_LABEL "Registration Email Address"
#define PASSWORD_LABEL "Create your Azoomee Password"
#define PIN_LABEL "Create your Azoomee App 4 digit pin"
#define PIN_LABEL_DETAIL "Shhh! Don't tell the kids.."

Scene* OnboardingScene::createSceneWithEmail(std::string EmailAddress)
{
    auto scene = Scene::create();
    auto layer = OnboardingScene::create();
    scene->addChild(layer);
    
    //Call FunctionalElements Here to add the email address incase it is added in login.
    layer->addFunctionalElementsToScene(EmailAddress);
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
    //addFunctionalElementsToScene();
    
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

void OnboardingScene::addFunctionalElementsToScene(std::string EmailAddress)
{
    addContentLayerToScene();
    addLabelsToLayer();
    addTextBoxesToLayer(EmailAddress);
    addButtonsToLayer();
    
    checkIfEmailAlreadyEntered();
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

void OnboardingScene::addTextBoxesToLayer(std::string EmailAddress)
{
    auto _editName = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editName->setColor(Color3B::WHITE);
    _editName->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.5));
    _editName->setFont("fonts/azoomee.ttf", 90);
    _editName->setFontColor(Color3B::WHITE);
    _editName->setMaxLength(100);
    _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editName->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
    _editName->setText(EmailAddress.c_str());
    _editName->setName("usernameField");

    onboardingContent->addChild(_editName);
    
    auto _editPassword = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editPassword->setPosition(Vec2(origin.x+visibleSize.width * 1.5, origin.y+visibleSize.height*0.5));
    _editPassword->setFont("fonts/azoomee.ttf", 90);
    _editPassword->setFontColor(Color3B::WHITE);
    _editPassword->setMaxLength(50);
    _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editPassword->setName("passwordField");
    onboardingContent->addChild(_editPassword);
    
    auto _editPin = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editPin->setPosition(Vec2(origin.x+visibleSize.width * 2.5, origin.y+visibleSize.height*0.5));
    _editPin->setFont("fonts/azoomee.ttf", 90);
    _editPin->setFontColor(Color3B::WHITE);
    _editPin->setMaxLength(4);
    _editPin->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPin->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editPin->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editPin->setName("pinField");
    onboardingContent->addChild(_editPin);
}

void OnboardingScene::addButtonsToLayer()
{
    // in order they appear on the screen with next, then back, then next etc.
    
    auto emailNextButton = Sprite::create("res/login/next_btn.png");
    emailNextButton->setPosition(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.5);
    emailNextButton->setScale(1.2);
    emailNextButton->setTag(1);
    addListenerToButton(emailNextButton);
    onboardingContent->addChild(emailNextButton);
    
    auto passwordBackButton = Sprite::create("res/login/back_btn.png");
    passwordBackButton->setPosition(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5);
    passwordBackButton->setTag(2);
    addListenerToButton(passwordBackButton);
    onboardingContent->addChild(passwordBackButton);
    
    auto passwordNextButton = Sprite::create("res/login/next_btn.png");
    passwordNextButton->setPosition(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5);
    passwordNextButton->setTag(3);
    addListenerToButton(passwordNextButton);
    onboardingContent->addChild(passwordNextButton);
    
    auto pinBackButton = Sprite::create("res/login/back_btn.png");
    pinBackButton->setPosition(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5);
    pinBackButton->setTag(4);
    addListenerToButton(pinBackButton);
    onboardingContent->addChild(pinBackButton);
    
    auto pinNextButton = Sprite::create("res/login/next_btn.png");
    pinNextButton->setPosition(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5);
    pinNextButton->setTag(5);
    addListenerToButton(pinNextButton);
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
                case(1): this->emailNextButton(); break;
                case(2): this->passwordBackButton(); break;
                case(3): this->passwordNextButton(); break;
                case(4): this->pinBackButton(); break;
                case(5): this->pinNextButton(); break;
            }
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, spriteImage);
}

void OnboardingScene::checkIfEmailAlreadyEntered()
{
    //if the scene starts with a valid email address, move to password
    if(isValidEmailAddress(((ui::EditBox *)onboardingContent->getChildByName("usernameField"))->getText()))
    {
        onboardingContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2));
    }
}

void OnboardingScene::emailNextButton()
{
    //Check if Email address is in correct format
    if(isValidEmailAddress(((ui::EditBox *)onboardingContent->getChildByName("usernameField"))->getText()))
    {
           onboardingContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2));
    }
    else
    {
        //ERROR incorrect email address
        
    }
}

void OnboardingScene::passwordBackButton()
{
        ((ui::EditBox *)onboardingContent->getChildByName("passwordField"))->setText("");
        onboardingContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2));
}

void OnboardingScene::passwordNextButton()
{
    //Check if Password is in correct format
    if(true)
    {
        onboardingContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width*2 + origin.x, origin.y)), 2));
    }
    else
    {
        //ERROR incorrect Password format
    }
}

void OnboardingScene::pinBackButton()
{
    ((ui::EditBox *)onboardingContent->getChildByName("pinField"))->setText("");
    onboardingContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2));
}

void OnboardingScene::pinNextButton()
{
    //Check if Email address is in correct format
    if(isValidPin(((ui::EditBox *)onboardingContent->getChildByName("pinField"))->getText()))
    {
        std::string username = ((ui::EditBox *)onboardingContent->getChildByName("usernameField"))->getText();
        std::string password = ((ui::EditBox *)onboardingContent->getChildByName("passwordField"))->getText();
        std::string pin = ((ui::EditBox *)onboardingContent->getChildByName("pinField"))->getText();
        
        //Need to pass SOURCE attribute to server #TODO
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            //SEND INAPP_IOS to Source
        
        #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            //SEND INAPP_ANDROID to Source
        
        #endif
        
        //FOR DEBUG PURPOSES ONLY, PLEASE REMOVE WHEN GETTING INTO PRODUCTION
        
        if(username == "aaa")
        {
            username = "klaas+ci@azoomee.com";
            password = "test1234";
        }
        
        //DELETE UNTIL THIS POINT IN PRODUCTION
        
        auto backEndCaller = BackEndCaller::getInstance();
        backEndCaller->login(username, password);
    }
    else
    {
        //ERROR incorrect pin number
        
    }
}

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
    unsigned int Length = strlen(EmailAddress); // Length = StringLength (strlen) of EmailAddress
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
