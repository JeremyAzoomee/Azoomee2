#include "LoginScene.h"
#include "SimpleAudioEngine.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"
#include "OnboardingScene.h"

USING_NS_CC;

//Text Content defined here, ready to be changed when localisation method defined.
#define LOGIN_BUTTON_TEXT "Log In"
#define SIGN_UP_BUTTON_TEXT "Sign Up"
#define EMAIL_LABEL "Login Email"
#define PASSWORD_LABEL "Password"

//Define Editbox Tag Numbers
#define TAG_EMAIL_EDITBOX 1
#define TAG_PASSWORD_EDITBOX 2

//Define BUTTONS Tag Numbers
#define TAG_LOGIN_BUTTON 10
#define TAG_EMAIL_BACK_BUTTON 11
#define TAG_EMAIL_NEXT_BUTTON 12
#define TAG_PASSWORD_BACK_BUTTON 13
#define TAG_PASSWORD_NEXT_BUTTON 14

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
    emailTitle->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    emailTitle->setColor(Color3B(28, 244, 244));
    loginContent->addChild(emailTitle);
    
    auto passwordTitle = Label::createWithTTF(PASSWORD_LABEL, "fonts/azoomee.ttf", 90);
    passwordTitle->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    passwordTitle->setColor(Color3B(28, 244, 244));
    loginContent->addChild(passwordTitle);
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
    _editName->setPosition(Vec2(origin.x+visibleSize.width * 1.5, origin.y+visibleSize.height*0.5));
    _editName->setFont("fonts/azoomee.ttf", 90);
    _editName->setFontColor(Color3B::WHITE);
    _editName->setMaxLength(100);
    _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editName->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
    _editName->setTag(TAG_EMAIL_EDITBOX);
    _editName->setDelegate(this);
    loginContent->addChild(_editName);
    
    auto _editPassword = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editPassword->setPosition(Vec2(origin.x+visibleSize.width * 2.5, origin.y+visibleSize.height*0.5));
    _editPassword->setFont("fonts/azoomee.ttf", 90);
    _editPassword->setFontColor(Color3B::WHITE);
    _editPassword->setMaxLength(50);
    _editPassword->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editPassword->setTag(TAG_PASSWORD_EDITBOX);
    _editPassword->setDelegate(this);
    loginContent->addChild(_editPassword);
}

void LoginScene::addButtonsToLayer()
{
    // BUTTONS IN ORDER OF LAYOUT
    auto signUpButton = ui::Button::create();
    signUpButton->setTitleText(SIGN_UP_BUTTON_TEXT);
    signUpButton->setTitleFontName("fonts/azoomee.ttf");
    signUpButton->setPosition(Vec2(origin.x+visibleSize.width * 0.5, origin.y+visibleSize.height*0.6));
    signUpButton->setTitleFontSize(90);
    signUpButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
            {
                auto _OnboardingScene = OnboardingScene::createScene(0);
                Director::getInstance()->replaceScene(_OnboardingScene);
                break;
            }
            case ui::Widget::TouchEventType::MOVED:
                break;
            case ui::Widget::TouchEventType::CANCELED:
                break;
            case ui::Widget::TouchEventType::ENDED:
                break;
        }
    });
    loginContent->addChild(signUpButton);
    
    auto loginButton = ui::Button::create();
    loginButton->setTitleText(LOGIN_BUTTON_TEXT);
    loginButton->setTitleFontName("fonts/azoomee.ttf");
    loginButton->setPosition(Vec2(origin.x+visibleSize.width * 0.5, origin.y+visibleSize.height*0.4));
    loginButton->setTitleFontSize(90);
    loginButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
            {
                if ( loginContent->getNumberOfRunningActions() == 0 )
                moveLoginToEmailScreen(loginButton);
                break;
            }
            case ui::Widget::TouchEventType::MOVED:
                break;
            case ui::Widget::TouchEventType::CANCELED:
                break;
            case ui::Widget::TouchEventType::ENDED:
                break;
        }
    });
    loginButton->setTag(TAG_LOGIN_BUTTON);
    loginContent->addChild(loginButton);
    
    auto emailBackButton = Sprite::create("res/login/back_btn.png");
    emailBackButton->setPosition(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5);
    emailBackButton->setTag(TAG_EMAIL_BACK_BUTTON);
    addListenerToButton(emailBackButton);
    loginContent->addChild(emailBackButton);
    
    auto emailNextButton = Sprite::create("res/login/next_btn.png");
    emailNextButton->setPosition(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5);
    emailNextButton->setTag(TAG_EMAIL_NEXT_BUTTON);
    addListenerToButton(emailNextButton);
    emailNextButton->setVisible(false);
    loginContent->addChild(emailNextButton);
    
    auto passwordBackButton = Sprite::create("res/login/back_btn.png");
    passwordBackButton->setPosition(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5);
    passwordBackButton->setTag(TAG_PASSWORD_BACK_BUTTON);
    addListenerToButton(passwordBackButton);
    loginContent->addChild(passwordBackButton);
    
    auto passwordNextButton = Sprite::create("res/login/next_btn.png");
    passwordNextButton->setPosition(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5);
    passwordNextButton->setTag(TAG_PASSWORD_NEXT_BUTTON);
    addListenerToButton(passwordNextButton);
    passwordNextButton->setVisible(false);
    passwordNextButton->setScale(1.2);
    loginContent->addChild(passwordNextButton);
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
                case(TAG_EMAIL_BACK_BUTTON): this->moveToBackFirstScreenEnableLogin(spriteImage); break;
                case(TAG_EMAIL_NEXT_BUTTON): this->moveToPasswordScreen(spriteImage); break;
                case(TAG_PASSWORD_BACK_BUTTON): this->moveToEmailScreen(spriteImage); break;
                case(TAG_PASSWORD_NEXT_BUTTON): this->login(); break;
            }
            
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, spriteImage);
}

void LoginScene::disableMoveButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(button);
}

void LoginScene::enableMoveButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(button);
}

void LoginScene::moveLoginToEmailScreen(ui::Button* button)
{
    loginContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2));
}

void LoginScene::moveToBackFirstScreenEnableLogin(Node* button)
{
    disableMoveButton(button);
    //((ui::Button *)loginContent->getChildByTag(TAG_LOGIN_BUTTON))->setEnabled(true);

    auto action = EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(LoginScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    loginContent->runAction(sequence);
}

void LoginScene::moveToPasswordScreen(Node* button)
{
    disableMoveButton(button);
    
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width*2 + origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(LoginScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    loginContent->runAction(sequence);
}

void LoginScene::moveToEmailScreen(Node* button)
{
    disableMoveButton(button);
    
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(LoginScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    loginContent->runAction(sequence);
    
    //remove Password Text, therefore hide Password NextButton
    ((ui::EditBox *)loginContent->getChildByTag(TAG_PASSWORD_EDITBOX))->setText("");
    ((cocos2d::Sprite *)loginContent->getChildByTag(TAG_PASSWORD_NEXT_BUTTON))->setVisible(false);
}

void LoginScene::login()
{
    //LOGIN BUTTON
    std::string username = ((ui::EditBox *)loginContent->getChildByTag(TAG_EMAIL_EDITBOX))->getText();
    std::string password = ((ui::EditBox *)loginContent->getChildByTag(TAG_PASSWORD_EDITBOX))->getText();
    
    //FOR DEBUG PURPOSES ONLY, PLEASE REMOVE WHEN GETTING INTO PRODUCTION
    
   /* if(password == "aaa")
    {
        username = "klaas+ci@azoomee.com";
        password = "test1234";
    }*/

    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->login(username, password);
}

//Editbox Delegate Functions
void LoginScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    //Make the next buttons visible if the inputs are in correct format
    
    if(editBox->getTag() == TAG_EMAIL_EDITBOX)
    {
        //Set the Visibility of the Next Button to visible, if Valid email.
        ((cocos2d::Sprite *)loginContent->getChildByTag(TAG_EMAIL_NEXT_BUTTON))->setVisible(isValidEmailAddress(text.c_str()));
    }
    else if(editBox->getTag() == TAG_PASSWORD_EDITBOX)
    {
        //Set the Visibility of the Next Button to visible, if Valid Password
        ((cocos2d::Sprite *)loginContent->getChildByTag(TAG_PASSWORD_NEXT_BUTTON))->setVisible(isValidPassword(text.c_str()));
    }
}

void LoginScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    //This function only really works correctly for Email_Editbox, as it has an email address
    //Otherwise detect the \n with the text changed in the other EditBoxes
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

bool LoginScene::isValidPassword(const char * password)
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

