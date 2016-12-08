#include "ChildAccountScene.h"
#include "SimpleAudioEngine.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"
#include "DataStorage.h"

USING_NS_CC;

//Text Content defined here, ready to be changed when localisation method defined.
#define LABEL_NEW_REQUEST_CHILD_NAME "Hello! Please add a display name?"
#define LABEL_EDIT_CHILD_NAME "Want to change the display name?"
#define LABEL_NEW_REQUEST_CHILD_BIRTHDAY "Please add date of birth."
#define LABEL_EDIT_REQUEST_CHILD_BIRTHDAY "Change birthday?"
#define LABEL_NEW_REQUEST_CHILD_OOMEE "Select your favourite Oomee."
#define LABEL_NEW_REQUEST_CHILD_OOMEE_DETAIL "Don't worry you can change it later if you want."
#define LABEL_EDIT_CHILD_OOMEE "Change your Oomee?"

#define EDITBOX_PLACEHOLDER_DOB_DAY "DD"
#define EDITBOX_PLACEHOLDER_DOB_MONTH "MM"
#define EDITBOX_PLACEHOLDER_DOB_YEAR "YYYY"

//define objectNames
#define OBJECTNAME_SCROLL_LAYER "scrollLayer"
#define OBJECTNAME_EDITBOX_CHILDNAME "childName"
#define OBJECTNAME_EDITBOX_DOB_DAY "childDOB_Day"
#define OBJECTNAME_EDITBOX_DOB_MONTH "childDOB_Month"
#define OBJECTNAME_EDITBOX_DOB_YEAR "childDOB_Year"

// NOTE Child Age to be in "yyyy-MM-dd" format

Scene* ChildAccountScene::createSceneWithName(std::string ChildName)
{
    auto scene = Scene::create();
    auto layer = ChildAccountScene::create();
    scene->addChild(layer);
    layer->setChildName(ChildName);
    layer->addFunctionalElementsToScene();
    
    return scene;
}

bool ChildAccountScene::init()
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


void ChildAccountScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
}

void ChildAccountScene::setChildName(std::string ChildName)
{
    this->passedChildName = ChildName;
    
    //if childName is nil, then this is a new account to add.
    if(ChildName.compare("") == 0)
        this->isNewChildAccount = true;
    else
        this->isNewChildAccount = false;
}

void ChildAccountScene::addVisualElementsToScene()
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

void ChildAccountScene::addFunctionalElementsToScene()
{
    addContentLayerToScene();
    addLabelsToLayer();
    addTextBoxesToLayer();
    addButtonsToLayer();
}

void ChildAccountScene::addContentLayerToScene()
{
    //we add an additional layer that will slide on the screen between the username and the password.
    //we add all input elements on this layer.
    
    childAccountContent = Layer::create();
    childAccountContent->setContentSize(Size(visibleSize.width * 3, visibleSize.height));
    childAccountContent->setPosition(Point(origin.x, origin.y));
    childAccountContent->setName(OBJECTNAME_SCROLL_LAYER);
    this->addChild(childAccountContent);
}

void ChildAccountScene::addLabelsToLayer()
{
    auto emailTitle = Label::create();
    emailTitle->setSystemFontName("fonts/azoomee.ttf");
    emailTitle->setSystemFontSize(90);
    
    //Add diferent text depending on new or editing account
    if(this->isNewChildAccount)
        emailTitle->setString(LABEL_NEW_REQUEST_CHILD_NAME);
    else
        emailTitle->setString(LABEL_EDIT_CHILD_NAME);
    
    emailTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    emailTitle->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(emailTitle);
    
    /*auto DOB_Title = Label::create();
    DOB_Title->setSystemFontName("fonts/azoomee.ttf");
    DOB_Title->setSystemFontSize(90);
    
    //Add diferent text depending on new or editing account
    if(this->isNewChildAccount)
        DOB_Title->setString(LABEL_NEW_REQUEST_CHILD_BIRTHDAY);
    else
        DOB_Title->setString(LABEL_EDIT_REQUEST_CHILD_BIRTHDAY);
    
    DOB_Title->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    DOB_Title->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(DOB_Title);*/
    
    /*auto pinTitle = Label::createWithTTF(NEW_REQUEST_CHILD_NAME, "fonts/azoomee.ttf", 90);
    pinTitle->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    pinTitle->setColor(Color3B(28, 244, 244));
    onboardingContent->addChild(pinTitle);
    
    auto pinDetail = Label::createWithTTF(NEW_REQUEST_CHILD_NAME, "fonts/azoomee.ttf", 60);
    pinDetail->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.6);
    pinDetail->setColor(Color3B::WHITE);
    onboardingContent->addChild(pinDetail);*/
}

void ChildAccountScene::addTextBoxesToLayer()
{
    auto _editName = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editName->setColor(Color3B::WHITE);
    _editName->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.5));
    _editName->setFont("fonts/azoomee.ttf", 90);
    _editName->setFontColor(Color3B::WHITE);
    _editName->setMaxLength(50);
    _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editName->setText(this->passedChildName.c_str());
    _editName->setDelegate(this);
    _editName->setName(OBJECTNAME_EDITBOX_CHILDNAME);
    _editName->setSwallowTouches(true);

    childAccountContent->addChild(_editName);
    
    auto _editDOB_Day = ui::EditBox::create(Size(200,131), "res/login/textarea_bg.png");
    _editDOB_Day->setPosition(Vec2(origin.x+visibleSize.width * 1.5 - 300, origin.y+visibleSize.height*0.5));
    _editDOB_Day->setFont("fonts/azoomee.ttf", 90);
    _editDOB_Day->setFontColor(Color3B::WHITE);
    _editDOB_Day->setMaxLength(2);
    _editDOB_Day->setPlaceHolder(EDITBOX_PLACEHOLDER_DOB_DAY);
    _editDOB_Day->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editDOB_Day->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editDOB_Day->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editDOB_Day->setName(OBJECTNAME_EDITBOX_DOB_DAY);
    childAccountContent->addChild(_editDOB_Day);
    
    auto _editDOB_Month = ui::EditBox::create(Size(200,131), "res/login/textarea_bg.png");
    _editDOB_Month->setPosition(Vec2(origin.x+visibleSize.width * 1.5 - 50, origin.y+visibleSize.height*0.5));
    _editDOB_Month->setFont("fonts/azoomee.ttf", 90);
    _editDOB_Month->setFontColor(Color3B::WHITE);
    _editDOB_Month->setMaxLength(2);
    _editDOB_Month->setPlaceHolder(EDITBOX_PLACEHOLDER_DOB_MONTH);
    _editDOB_Month->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editDOB_Month->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editDOB_Month->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editDOB_Month->setName(OBJECTNAME_EDITBOX_DOB_MONTH);
    childAccountContent->addChild(_editDOB_Month);
    
    auto _editDOB_Year = ui::EditBox::create(Size(300,131), "res/login/textarea_bg.png");
    _editDOB_Year->setPosition(Vec2(origin.x+visibleSize.width * 1.5 + 250, origin.y+visibleSize.height*0.5));
    _editDOB_Year->setFont("fonts/azoomee.ttf", 90);
    _editDOB_Year->setFontColor(Color3B::WHITE);
    _editDOB_Year->setMaxLength(4);
    _editDOB_Year->setPlaceHolder(EDITBOX_PLACEHOLDER_DOB_YEAR);
    _editDOB_Year->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editDOB_Year->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editDOB_Year->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editDOB_Year->setName(OBJECTNAME_EDITBOX_DOB_YEAR);
    childAccountContent->addChild(_editDOB_Year);
    
    /*auto _editPin = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editPin->setPosition(Vec2(origin.x+visibleSize.width * 2.5, origin.y+visibleSize.height*0.5));
    _editPin->setFont("fonts/azoomee.ttf", 90);
    _editPin->setFontColor(Color3B::WHITE);
    _editPin->setMaxLength(4);
    _editPin->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPin->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editPin->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editPin->setName("pinField");
    onboardingContent->addChild(_editPin);*/
}

void ChildAccountScene::addButtonsToLayer()
{
    // in order they appear on the screen with next, then back, then next etc.
    
    auto childNameNextButton = Sprite::create("res/login/next_btn.png");
    childNameNextButton->setPosition(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.5);
    childNameNextButton->setScale(1.2);
    childNameNextButton->setName(getNextButtonName(OBJECTNAME_EDITBOX_CHILDNAME));
    addListenerToButton(childNameNextButton);
    
    //only show button if there is text
    if(this->isNewChildAccount)
        childNameNextButton->setVisible(false);
    
    childAccountContent->addChild(childNameNextButton);
    
    /*auto DOB_BackButton = Sprite::create("res/login/back_btn.png");
    DOB_BackButton->setPosition(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5);
    childNameNextButton->setName(getBackButtonName(OBJECTNAME_EDITBOX_DOB_YEAR));
    addListenerToButton(DOB_BackButton);
    childAccountContent->addChild(DOB_BackButton);*/
    
    /*auto passwordNextButton = Sprite::create("res/login/next_btn.png");
    passwordNextButton->setPosition(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5);
    passwordNextButton->setTag(3);
    addListenerToButton(passwordNextButton);
    childAccountContent->addChild(passwordNextButton);
    
    auto pinBackButton = Sprite::create("res/login/back_btn.png");
    pinBackButton->setPosition(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5);
    pinBackButton->setTag(4);
    addListenerToButton(pinBackButton);
    childAccountContent->addChild(pinBackButton);
    
    auto pinNextButton = Sprite::create("res/login/next_btn.png");
    pinNextButton->setPosition(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5);
    pinNextButton->setTag(5);
    addListenerToButton(pinNextButton);
    childAccountContent->addChild(pinNextButton);*/

}

void ChildAccountScene::addListenerToButton(cocos2d::Sprite *spriteImage)
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
            if(spriteImage->getName().compare(getNextButtonName(OBJECTNAME_EDITBOX_CHILDNAME).c_str()) == 0)
                this->childNameNextButton();
            
            
            /*int buttonTag = spriteImage->getTag();
            switch(buttonTag)
            {
                case(1): this->childNameNextButton(); break;
                //case(2): this->passwordBackButton(); break;
                //case(3): this->passwordNextButton(); break;
                //case(4): this->pinBackButton(); break;
                //case(5): this->pinNextButton(); break;
            }*/
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, spriteImage);
}

void ChildAccountScene::childNameNextButton()
{
    //Check if editbox is not null, and does not exist, then move onwards
    
    std::string newChildsName = ((ui::EditBox *)childAccountContent->getChildByName(OBJECTNAME_EDITBOX_CHILDNAME))->getText();
    
    if(newChildsName.compare("") == 0)
    {
        //ERROR MESSAGE - Must have a name
        
    }
    else if(this->passedChildName.compare(newChildsName) == 0 && !this-isNewChildAccount)
    {
        //If the childs name has not changed, then move forward.
        //This stops a false error of "CHILD NAME EXISTS"
        
        childAccountContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2));
    }
    else
    {
        //check if child name exists
        bool childNameExists = false;
        
        for(int i = 0; i < DataStorage::getInstance()->getAmountOfAvailableChildren(); i++)
        {
            std::string storedChildsName = DataStorage::getInstance()->getValueFromOneAvailableChild(i, "profileName");
            
            if(newChildsName.compare(storedChildsName) == 0)
            {
                childNameExists = true;
                break;
            }
        }
        
        if(childNameExists)
        {
            //ERROR MESSAGE - Name exists
        }
        else
        {
            childAccountContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2));
        }
    }
}

void ChildAccountScene::passwordBackButton()
{
        ((ui::EditBox *)childAccountContent->getChildByName("passwordField"))->setText("");
        childAccountContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2));
}

void ChildAccountScene::passwordNextButton()
{
    //Check if Password is in correct format
    if(true)
    {
        childAccountContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width*2 + origin.x, origin.y)), 2));
    }
    else
    {
        //ERROR incorrect Password format
    }
}

void ChildAccountScene::pinBackButton()
{
    ((ui::EditBox *)childAccountContent->getChildByName("pinField"))->setText("");
    childAccountContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2));
}

void ChildAccountScene::pinNextButton()
{
    //Check if Email address is in correct format
    if(isValidPin(((ui::EditBox *)childAccountContent->getChildByName("pinField"))->getText()))
    {
        std::string username = ((ui::EditBox *)childAccountContent->getChildByName("usernameField"))->getText();
        std::string password = ((ui::EditBox *)childAccountContent->getChildByName("passwordField"))->getText();
        std::string pin = ((ui::EditBox *)childAccountContent->getChildByName("pinField"))->getText();
        
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

bool ChildAccountScene::isCharacter(const char Character)
{
    //Borrowed from internet With isValidEmailAddress to check email address format
    
    return ( (Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
    //Checks if a Character is a Valid A-Z, a-z Character, based on the ascii value
}
bool ChildAccountScene::isNumber(const char Character)
{
    return ( Character >= '0' && Character <= '9');
    //Checks if a Character is a Valid 0-9 Number, based on the ascii value
}

bool ChildAccountScene::isValidEmailAddress(const char * EmailAddress)
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

bool ChildAccountScene::isValidPin(const char * pinNumber)
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

std::string ChildAccountScene::getNextButtonName(std::string editBoxName)
{
    return StringUtils::format("%sNextButton", editBoxName.c_str());
}

std::string ChildAccountScene::getBackButtonName(std::string editBoxName)
{
    return StringUtils::format("%sBackButton", editBoxName.c_str());
}

//Editbox Delegate Functions
void ChildAccountScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    //if there is text, then show the next button.
    if(text.compare("") !=0)
        ((cocos2d::Sprite *)childAccountContent->getChildByName(getNextButtonName(OBJECTNAME_EDITBOX_CHILDNAME)))->setVisible(true);
    else
        ((cocos2d::Sprite *)childAccountContent->getChildByName(getNextButtonName(OBJECTNAME_EDITBOX_CHILDNAME)))->setVisible(false);
}

void ChildAccountScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    editBox->setSwallowTouches(true);
    
    if(editBox->getName().compare(OBJECTNAME_EDITBOX_CHILDNAME) == 0)
        this->childNameNextButton();
}
