#include "LoginScene.h"
#include "SimpleAudioEngine.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"
#include "OnboardingScene.h"

USING_NS_CC;

//Text Content defined here, ready to be changed when localisation method defined.
#define LOGIN_BUTTON_TEXT "Sign In"
#define EMAIL_LABEL "Please add your email address"
#define PASSWORD_LABEL "Your Azoomee Password"
#define PIN_LABEL "Create your Azoomee App 4 digit pin"
#define PIN_LABEL_DETAIL "Shhh! Don't tell the kids.."

//Define Editbox Tag Numbers
#define TAG_EMAIL_EDITBOX 1
#define TAG_PASSWORD_EDITBOX 2
#define TAG_PIN_EDITBOX 3

//Define BUTTONS Tag Numbers
#define TAG_EMAIL_NEXT_BUTTON 10
#define TAG_PASSWORD_BACK_BUTTON 11
#define TAG_LOGIN_BUTTON 12
#define TAG_PASSWORD_NEXT_BUTTON 13
#define TAG_PIN_BACK_BUTTON 14
#define TAG_PIN_NEXT_BUTTON 15

Scene* LoginScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LoginScene::create();
    scene->addChild(layer);
    return scene;
}

bool LoginScene::init()
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


void LoginScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
}

void LoginScene::addVisualElementsToScene()
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

void LoginScene::addFunctionalElementsToScene()
{
    addContentLayerToScene();
    addLabelsToLayer();
    addTextBoxesToLayer();
    addButtonsToLayer();
}

void LoginScene::addLabelsToLayer()
{
    auto emailTitle = Label::createWithTTF(EMAIL_LABEL, "fonts/azoomee.ttf", 90);
    emailTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    emailTitle->setColor(Color3B(28, 244, 244));
    loginContent->addChild(emailTitle);
    
    auto passwordTitle = Label::createWithTTF(PASSWORD_LABEL, "fonts/azoomee.ttf", 90);
    passwordTitle->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    passwordTitle->setColor(Color3B(28, 244, 244));
    loginContent->addChild(passwordTitle);
    
    auto pinTitle = Label::createWithTTF(PIN_LABEL, "fonts/azoomee.ttf", 90);
    pinTitle->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    pinTitle->setColor(Color3B(28, 244, 244));
    loginContent->addChild(pinTitle);
    
    auto pinDetail = Label::createWithTTF(PIN_LABEL_DETAIL, "fonts/azoomee.ttf", 60);
    pinDetail->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.6);
    pinDetail->setColor(Color3B::WHITE);
    loginContent->addChild(pinDetail);
}

void LoginScene::addContentLayerToScene()
{
    //we add an additional layer that will slide on the screen between the username and the password.
    //we add all input elements on this layer.
    
    loginContent = Layer::create();
    loginContent->setContentSize(Size(visibleSize.width * 3, visibleSize.height));
    loginContent->setPosition(Point(origin.x, origin.y));
    loginContent->setName("loginContent");
    this->addChild(loginContent);
}

void LoginScene::addTextBoxesToLayer()
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
    loginContent->addChild(_editName);
    
    auto _editPassword = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editPassword->setPosition(Vec2(origin.x+visibleSize.width * 1.5, origin.y+visibleSize.height*0.5));
    _editPassword->setFont("fonts/azoomee.ttf", 90);
    _editPassword->setFontColor(Color3B::WHITE);
    _editPassword->setMaxLength(50);
    _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editPassword->setTag(TAG_PASSWORD_EDITBOX);
    _editPassword->setDelegate(this);
    loginContent->addChild(_editPassword);
    
    auto _editPin = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editPin->setPosition(Vec2(origin.x+visibleSize.width * 2.5, origin.y+visibleSize.height*0.5));
    _editPin->setFont("fonts/azoomee.ttf", 90);
    _editPin->setFontColor(Color3B::WHITE);
    _editPin->setMaxLength(4);
    _editPin->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPin->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editPin->setTag(TAG_PIN_EDITBOX);
    _editPin->setDelegate(this);
    loginContent->addChild(_editPin);
}

void LoginScene::addButtonsToLayer()
{
    // BUTTONS IN ORDER OF LAYOUT
    auto emailNextButton = Sprite::create("res/login/next_btn.png");
    emailNextButton->setPosition(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.5);
    emailNextButton->setTag(TAG_EMAIL_NEXT_BUTTON);
    addListenerToButton(emailNextButton);
    emailNextButton->setVisible(false);
    loginContent->addChild(emailNextButton);
    
    auto passwordBackButton = Sprite::create("res/login/back_btn.png");
    passwordBackButton->setPosition(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5);
    passwordBackButton->setTag(TAG_PASSWORD_BACK_BUTTON);
    addListenerToButton(passwordBackButton);
    loginContent->addChild(passwordBackButton);

    auto loginButton = ui::Button::create();
    loginButton->setTitleText(LOGIN_BUTTON_TEXT);
    loginButton->setTitleFontName("fonts/azoomee.ttf");
    loginButton->setPosition(Vec2(origin.x+visibleSize.width * 1.5, origin.y+visibleSize.height*0.42));
    loginButton->setTitleFontSize(60);
    loginButton->setTag(TAG_LOGIN_BUTTON);
    loginButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                login();
                break;
            case ui::Widget::TouchEventType::MOVED:
                break;
            case ui::Widget::TouchEventType::CANCELED:
                break;
            case ui::Widget::TouchEventType::ENDED:
                break;
        }
    });
    loginButton->setVisible(false);
    loginContent->addChild(loginButton);
    
    auto passwordNextButton = Sprite::create("res/login/next_btn.png");
    passwordNextButton->setPosition(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5);
    passwordNextButton->setTag(TAG_PASSWORD_NEXT_BUTTON);
    addListenerToButton(passwordNextButton);
    passwordNextButton->setVisible(false);
    loginContent->addChild(passwordNextButton);
    
    auto pinBackButton = Sprite::create("res/login/back_btn.png");
    pinBackButton->setPosition(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5);
    pinBackButton->setTag(TAG_PIN_BACK_BUTTON);
    addListenerToButton(pinBackButton);
    loginContent->addChild(pinBackButton);
    
    auto pinNextButton = Sprite::create("res/login/next_btn.png");
    pinNextButton->setPosition(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5);
    pinNextButton->setScale(1.2);
    pinNextButton->setTag(TAG_PIN_NEXT_BUTTON);
    addListenerToButton(pinNextButton);
    pinNextButton->setVisible(false);
    loginContent->addChild(pinNextButton);
}

void LoginScene::addListenerToButton(cocos2d::Sprite *spriteImage)
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
                case(TAG_EMAIL_NEXT_BUTTON):
                    if(!this->emailDoneButtonPressed)
                        this->moveToPasswordScreen();
                    break;
                case(TAG_PASSWORD_BACK_BUTTON): this->moveToEmailScreen(); break;
                case(TAG_PASSWORD_NEXT_BUTTON): this->moveToPinScreen(); break;
                case(TAG_PIN_BACK_BUTTON):  this->moveToPasswordScreen(); break;
                case(TAG_PIN_NEXT_BUTTON):  this->signup(); break;
            }
            
            this->emailDoneButtonPressed = false;
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, spriteImage);
}

void LoginScene::moveToPasswordScreen()
{
    loginContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2));
}

void LoginScene::moveToEmailScreen()
{
    //QUESTION - WHY ARE WE BLANKING THE PASSWORD - DOES IT MATTER?
    //((ui::EditBox *)loginContent->getChildByName("passwordField"))->setText("");
    loginContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2));
}

void LoginScene::login()
{
    //ALSO THE LOGIN BUTTON
    std::string username = ((ui::EditBox *)loginContent->getChildByTag(TAG_EMAIL_EDITBOX))->getText();
    std::string password = ((ui::EditBox *)loginContent->getChildByTag(TAG_PASSWORD_EDITBOX))->getText();
    
    //Need to pass SOURCE attribute to server #TODO
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        //SEND INAPP_IOS to Source
    
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //SEND INAPP_ANDROID to Source
    
    #endif
    
    //FOR DEBUG PURPOSES ONLY, PLEASE REMOVE WHEN GETTING INTO PRODUCTION
    
    if(username == "aaa@a.a")
    {
        username = "klaas+ci@azoomee.com";
        password = "test1234";
    }

    //DELETE UNTIL THIS POINT IN PRODUCTION

    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->login(username, password);
}

void LoginScene::moveToPinScreen()
{
    loginContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width*2 + origin.x, origin.y)), 2));
}

void LoginScene::signup()
{
    
}

//Editbox Delegate Functions
void LoginScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    //Need to detect if DONE is selected by detecting \n pressed
    //Remove the linebreak and move to next screen.
    if(text.find("\n") != std::string::npos)
    {
        std::string tempstring = StringUtils::format("%s",text.c_str());
        
        editBox->setText(replace(tempstring, "\n", "").c_str());
        
        //Due to error with Editbox, this cleans the input.
        editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        editBox->setInputMode(ui::EditBox::InputMode::ANY);
        editBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        
        //Return/Enter/Send Key pressed, action to next screen
        //Email Editbox is detected with Editbox return
        if(editBox->getTag() == TAG_PASSWORD_EDITBOX)
        {
            //DO NOTHING - Design Decision, as there are 2 options - Login and Move to Pin screen
            editBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
        }
        else if(editBox->getTag() == TAG_PIN_EDITBOX && isValidPin(text.c_str()))
            this->signup();
    }
    
    if(editBox->getTag() == TAG_EMAIL_EDITBOX)
    {
        //Set the Visibility of the Next Button to visible, if Valid email.
        ((cocos2d::Sprite *)loginContent->getChildByTag(TAG_EMAIL_NEXT_BUTTON))->setVisible(isValidEmailAddress(text.c_str()));
    }
    else if(editBox->getTag() == TAG_PASSWORD_EDITBOX)
    {
        //Set the Visibility of the Next Button to visible, if Valid Password
        if(isValidPassword(text.c_str()))
        {
            ((cocos2d::Sprite *)loginContent->getChildByTag(TAG_PASSWORD_NEXT_BUTTON))->setVisible(true);
            ((cocos2d::Sprite *)loginContent->getChildByTag(TAG_LOGIN_BUTTON))->setVisible(true);
        }
        else
        {
            ((cocos2d::Sprite *)loginContent->getChildByTag(TAG_PASSWORD_NEXT_BUTTON))->setVisible(false);
            ((cocos2d::Sprite *)loginContent->getChildByTag(TAG_LOGIN_BUTTON))->setVisible(false);
        }
    }
    if(editBox->getTag() == TAG_PIN_EDITBOX)
    {
        //Set the Visibility of the Next Button to visible, if Valid PIN
        ((cocos2d::Sprite *)loginContent->getChildByTag(TAG_PIN_NEXT_BUTTON))->setVisible(isValidPin(text.c_str()));
    }
}

void LoginScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    //This function only really works correctly for Email_Editbox, as it has an email address
    //Otherwise detect the \n with the text changed in the other EditBoxes
    
    if(editBox->getTag() == TAG_EMAIL_EDITBOX && isValidEmailAddress(editBox->getText()))
    {
        this->emailDoneButtonPressed = true;
        this->moveToPasswordScreen();
    }
    else if (editBox->getTag() == TAG_EMAIL_EDITBOX)
    {
        //#TODO add error email is not correct format
        
    }
}

std::string LoginScene::replace(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return "";
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return str;
}

//Valid Input Buttons
bool LoginScene::isCharacter(const char Character)
{
    //Borrowed from internet With isValidEmailAddress to check email address format
    
    return ( (Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
    //Checks if a Character is a Valid A-Z, a-z Character, based on the ascii value
}
bool LoginScene::isNumber(const char Character)
{
    return ( Character >= '0' && Character <= '9');
    //Checks if a Character is a Valid 0-9 Number, based on the ascii value
}

bool LoginScene::isValidEmailAddress(const char * EmailAddress)
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

bool LoginScene::isValidPassword(const char * password)
{
    //check at least 2 chars and no white space
    if(!password) // If cannot read the password
        return 0;
    
    if(strlen(password) >= 2) // ensure there are 2 or more characters
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

bool LoginScene::isValidPin(const char * pinNumber)
{
    if(!pinNumber) // If cannot read the password
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

