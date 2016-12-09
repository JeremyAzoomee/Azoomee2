#include "ChildAccountScene.h"
#include "SimpleAudioEngine.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"
#include "DataStorage.h"
#include "ChildSelectorScene.h"
#include <ctime>

USING_NS_CC;

//Text Content defined here, ready to be changed when localisation method defined.
#define LABEL_NEW_REQUEST_CHILD_NAME "Hello! Please add a display name?"
#define LABEL_EDIT_CHILD_NAME "Want to change the display name?"
#define LABEL_NEW_REQUEST_CHILD_BIRTHDAY "Please add date of birth for"
#define LABEL_EDIT_REQUEST_CHILD_BIRTHDAY "Change birthday?"
#define LABEL_NEW_REQUEST_CHILD_OOMEE "Select your favourite Oomee."
#define LABEL_NEW_REQUEST_CHILD_OOMEE_DETAIL "Don't worry you can change it later."
#define LABEL_EDIT_CHILD_OOMEE "Change your Oomee?"

#define EDITBOX_PLACEHOLDER_DOB_DAY "DD"
#define EDITBOX_PLACEHOLDER_DOB_MONTH "MM"
#define EDITBOX_PLACEHOLDER_DOB_YEAR "YYYY"

//define objectNames
#define OBJECTNAME_SCROLL_LAYER "scrollLayer"
#define OBJECTNAME_EDITBOX_CHILDNAME "childName"
#define OBJECTNAME_EDITBOX_DOB_DAY "childDOBDay"
#define OBJECTNAME_EDITBOX_DOB_MONTH "childDOBMonth"
#define OBJECTNAME_EDITBOX_DOB_YEAR "childDOBYear"
#define OBJECTNAME_LABEL_DOB "childDOBLabel"
#define OBJECTNAME_LABEL_SELECT_OOMEE "childSelectOomeeLabel"
#define OBJECTNAME_SPRITE_OOMEE "oomeeSprite"

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
    addOomeesToLayer();
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
    
    auto DOB_Title = Label::create();
    DOB_Title->setSystemFontName("fonts/azoomee.ttf");
    DOB_Title->setSystemFontSize(90);
    DOB_Title->setName(OBJECTNAME_LABEL_DOB);
    DOB_Title->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    DOB_Title->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(DOB_Title);
    
    auto selectOomeeTitle = Label::create();
    selectOomeeTitle->setSystemFontName("fonts/azoomee.ttf");
    selectOomeeTitle->setSystemFontSize(90);
    selectOomeeTitle->setName(OBJECTNAME_LABEL_SELECT_OOMEE);
    if(this->isNewChildAccount)
        selectOomeeTitle->setString(LABEL_NEW_REQUEST_CHILD_OOMEE);
    else
        selectOomeeTitle->setString(LABEL_EDIT_CHILD_OOMEE);
    selectOomeeTitle->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    selectOomeeTitle->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(selectOomeeTitle);
    
    if(this->isNewChildAccount)
    {
        auto selectOomeeDetail = Label::createWithTTF(LABEL_NEW_REQUEST_CHILD_OOMEE_DETAIL, "fonts/azoomee.ttf", 60);
        selectOomeeDetail->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.6);
        selectOomeeDetail->setColor(Color3B::WHITE);
        childAccountContent->addChild(selectOomeeDetail);
    }
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
    _editName->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
    _editName->setText(this->passedChildName.c_str());
    _editName->setDelegate(this);
    _editName->setName(OBJECTNAME_EDITBOX_CHILDNAME);
    childAccountContent->addChild(_editName);
    
    auto _editDOB_Day = ui::EditBox::create(Size(200,131), "res/login/textarea_bg.png");
    _editDOB_Day->setPosition(Vec2(origin.x+visibleSize.width * 1.5 - 300, origin.y+visibleSize.height*0.5));
    _editDOB_Day->setFont("fonts/azoomee.ttf", 90);
    _editDOB_Day->setFontColor(Color3B::WHITE);
    _editDOB_Day->setMaxLength(2);
    _editDOB_Day->setPlaceHolder(EDITBOX_PLACEHOLDER_DOB_DAY);
    _editDOB_Day->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editDOB_Day->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editDOB_Day->setName(OBJECTNAME_EDITBOX_DOB_DAY);
    _editDOB_Day->setDelegate(this);
    childAccountContent->addChild(_editDOB_Day);
    
    auto _editDOB_Month = ui::EditBox::create(Size(200,131), "res/login/textarea_bg.png");
    _editDOB_Month->setPosition(Vec2(origin.x+visibleSize.width * 1.5 - 50, origin.y+visibleSize.height*0.5));
    _editDOB_Month->setFont("fonts/azoomee.ttf", 90);
    _editDOB_Month->setFontColor(Color3B::WHITE);
    _editDOB_Month->setMaxLength(2);
    _editDOB_Month->setPlaceHolder(EDITBOX_PLACEHOLDER_DOB_MONTH);
    _editDOB_Month->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editDOB_Month->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editDOB_Month->setName(OBJECTNAME_EDITBOX_DOB_MONTH);
    _editDOB_Month->setDelegate(this);
    childAccountContent->addChild(_editDOB_Month);
    
    auto _editDOB_Year = ui::EditBox::create(Size(300,131), "res/login/textarea_bg.png");
    _editDOB_Year->setPosition(Vec2(origin.x+visibleSize.width * 1.5 + 250, origin.y+visibleSize.height*0.5));
    _editDOB_Year->setFont("fonts/azoomee.ttf", 90);
    _editDOB_Year->setFontColor(Color3B::WHITE);
    _editDOB_Year->setMaxLength(4);
    _editDOB_Year->setPlaceHolder(EDITBOX_PLACEHOLDER_DOB_YEAR);
    _editDOB_Year->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editDOB_Year->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editDOB_Year->setName(OBJECTNAME_EDITBOX_DOB_YEAR);
    _editDOB_Year->setDelegate(this);
    childAccountContent->addChild(_editDOB_Year);
    
}

void ChildAccountScene::addButtonsToLayer()
{
    // in order they appear on the screen with next, then back, then next etc.
    auto closeBackButton = Sprite::create("res/login/back_btn.png");
    closeBackButton->setPosition(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.5);
    closeBackButton->setName(getBackButtonName(OBJECTNAME_EDITBOX_CHILDNAME));
    addListenerToButton(closeBackButton);
    childAccountContent->addChild(closeBackButton);
    
    auto childNameNextButton = Sprite::create("res/login/next_btn.png");
    childNameNextButton->setPosition(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.5);
    childNameNextButton->setName(getNextButtonName(OBJECTNAME_EDITBOX_CHILDNAME));
    addListenerToButton(childNameNextButton);
    
    //only show button if there is text
    if(this->isNewChildAccount)
        childNameNextButton->setVisible(false);
    
    childAccountContent->addChild(childNameNextButton);
    
    auto DOB_BackButton = Sprite::create("res/login/back_btn.png");
    DOB_BackButton->setPosition(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5);
    DOB_BackButton->setName(getBackButtonName(OBJECTNAME_LABEL_DOB));
    addListenerToButton(DOB_BackButton);
    childAccountContent->addChild(DOB_BackButton);
    
    auto DOB_NextButton = Sprite::create("res/login/next_btn.png");
    DOB_NextButton->setPosition(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5);
    DOB_NextButton->setName(getNextButtonName(OBJECTNAME_LABEL_DOB));
    DOB_NextButton->setVisible(false);
    addListenerToButton(DOB_NextButton);
    childAccountContent->addChild(DOB_NextButton);
    
    auto selectOomeeBackButton = Sprite::create("res/login/back_btn.png");
    selectOomeeBackButton->setPosition(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5);
    selectOomeeBackButton->setName(getBackButtonName(OBJECTNAME_LABEL_SELECT_OOMEE));
    addListenerToButton(selectOomeeBackButton);
    childAccountContent->addChild(selectOomeeBackButton);
    
    auto selectOomeeDoneButton = Sprite::create("res/login/next_btn.png");
    selectOomeeDoneButton->setPosition(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5);
    selectOomeeDoneButton->setScale(1.2);
    selectOomeeDoneButton->setVisible(false);
    selectOomeeDoneButton->setName(getNextButtonName(OBJECTNAME_LABEL_SELECT_OOMEE));
    addListenerToButton(selectOomeeDoneButton);
    childAccountContent->addChild(selectOomeeDoneButton);

}

void ChildAccountScene::addOomeesToLayer()
{
    float gapSize = 50;
    
    auto oomee_0 = Sprite::create("res/childSelection/oomee_0.png");
    oomee_0->setPosition(origin.x + visibleSize.width * 2.5 - (oomee_0->getContentSize().width*2 + gapSize*2), origin.y + visibleSize.height * 0.4);
    oomee_0->setOpacity(0);
    oomee_0->setName(StringUtils::format("%s_0",OBJECTNAME_SPRITE_OOMEE));
    addListenerToButton(oomee_0);
    childAccountContent->addChild(oomee_0);
    
    auto oomee_1 = Sprite::create("res/childSelection/oomee_1.png");
    oomee_1->setPosition(origin.x + visibleSize.width * 2.5 - (oomee_0->getContentSize().width + gapSize), origin.y + visibleSize.height * 0.4);
    oomee_1->setOpacity(0);
    oomee_1->setName(StringUtils::format("%s_1",OBJECTNAME_SPRITE_OOMEE));
    addListenerToButton(oomee_1);
    childAccountContent->addChild(oomee_1);
    
    auto oomee_2 = Sprite::create("res/childSelection/oomee_2.png");
    oomee_2->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.4);
    oomee_2->setOpacity(0);
    addListenerToButton(oomee_2);
    oomee_2->setName(StringUtils::format("%s_2",OBJECTNAME_SPRITE_OOMEE));
    childAccountContent->addChild(oomee_2);
    
    auto oomee_3 = Sprite::create("res/childSelection/oomee_3.png");
    oomee_3->setPosition(origin.x + visibleSize.width * 2.5 + (oomee_0->getContentSize().width + gapSize), origin.y + visibleSize.height * 0.4);
    oomee_3->setOpacity(0);
    oomee_3->setName(StringUtils::format("%s_3",OBJECTNAME_SPRITE_OOMEE));
    addListenerToButton(oomee_3);
    childAccountContent->addChild(oomee_3);
    
    auto oomee_4 = Sprite::create("res/childSelection/oomee_4.png");
    oomee_4->setPosition(origin.x + visibleSize.width * 2.5 + (oomee_0->getContentSize().width*2 + gapSize*2), origin.y + visibleSize.height * 0.4);
    oomee_4->setOpacity(0);
    oomee_4->setName(StringUtils::format("%s_4",OBJECTNAME_SPRITE_OOMEE));
    addListenerToButton(oomee_4);
    childAccountContent->addChild(oomee_4);
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
            if(spriteImage->getName().compare(getBackButtonName(OBJECTNAME_EDITBOX_CHILDNAME).c_str()) == 0)
                this->backButtonCloseScene();
            else if(spriteImage->getName().compare(getNextButtonName(OBJECTNAME_EDITBOX_CHILDNAME).c_str()) == 0)
                this->childNameNextButton();
            else if(spriteImage->getName().compare(getBackButtonName(OBJECTNAME_LABEL_DOB).c_str()) == 0)
                this->moveToChildNameInput();
            else if(spriteImage->getName().compare(getNextButtonName(OBJECTNAME_LABEL_DOB).c_str()) == 0)
                this->moveToOomeeSelection();
            else if(spriteImage->getName().compare(getBackButtonName(OBJECTNAME_LABEL_SELECT_OOMEE).c_str()) == 0)
                this->moveToDOBInput();
            else if(spriteImage->getName().compare(getNextButtonName(OBJECTNAME_LABEL_SELECT_OOMEE).c_str()) == 0)
                this->doneButton();
            else if(spriteImage->getName().find(OBJECTNAME_SPRITE_OOMEE, 0) != std::string::npos)
                this->selectOomee(spriteImage);

            
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, spriteImage);
}

void ChildAccountScene::backButtonCloseScene()
{
    auto childSelectorScene = ChildSelectorScene::createScene();
    Director::getInstance()->replaceScene(childSelectorScene);
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

        moveToDOBInput();
    }
    else
    {
        if(childNameExists(newChildsName))
        {
            //ERROR MESSAGE - Name exists
        }
        else
            moveToDOBInput();
    }
}

void ChildAccountScene::moveToDOBInput()
{
    //Set correct label Depending if new Child
    Label* DOB_Title = ((Label *)childAccountContent->getChildByName(OBJECTNAME_LABEL_DOB));
    
    if(this->isNewChildAccount)
    {
        std::string newChildsName = ((ui::EditBox *)childAccountContent->getChildByName(OBJECTNAME_EDITBOX_CHILDNAME))->getText();
        DOB_Title->setString(StringUtils::format("%s %s.", LABEL_NEW_REQUEST_CHILD_BIRTHDAY,newChildsName.c_str()));
    }
    else
        DOB_Title->setString(LABEL_EDIT_REQUEST_CHILD_BIRTHDAY);
    
    childAccountContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2));
}

bool ChildAccountScene::isDate()
{
    int d = atoi(((ui::EditBox *)childAccountContent->getChildByName(OBJECTNAME_EDITBOX_DOB_DAY))->getText());
    int m = atoi(((ui::EditBox *)childAccountContent->getChildByName(OBJECTNAME_EDITBOX_DOB_MONTH))->getText());
    int y = atoi(((ui::EditBox *)childAccountContent->getChildByName(OBJECTNAME_EDITBOX_DOB_YEAR))->getText());
    
    if (! (1582<= y )  )//comment these 2 lines out if it bothers you
        return false;
    if (! (1<= m && m<=12) )
        return false;
    if (! (1<= d && d<=31) )
        return false;
    if ( (d==31) && (m==2 || m==4 || m==6 || m==9 || m==11) )
        return false;
    if ( (d==30) && (m==2) )
        return false;
    if ( (m==2) && (d==29) && (y%4!=0) )
        return false;
    if ( (m==2) && (d==29) && (y%400==0) )
        return true;
    if ( (m==2) && (d==29) && (y%100==0) )
        return false;
    if ( (m==2) && (d==29) && (y%4==0)  )
        return true;
    
    return true;
}

void ChildAccountScene::moveToChildNameInput()
{
    childAccountContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2));
}

void ChildAccountScene::moveToOomeeSelection()
{
    childAccountContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width*2 + origin.x, origin.y)), 2));
    
    for(int i=0;i<5;i++)
    {
        cocos2d::Sprite* oomee = (cocos2d::Sprite *)childAccountContent->getChildByName(StringUtils::format("%s_%d",OBJECTNAME_SPRITE_OOMEE,i));
        float delayTime = CCRANDOM_0_1()+1;
        oomee->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    }
}

void ChildAccountScene::selectOomee(cocos2d::Sprite* selectedOomee)
{
    
    ((cocos2d::Sprite *)childAccountContent->getChildByName(getNextButtonName(OBJECTNAME_LABEL_SELECT_OOMEE)))->setVisible(true);
    
    //Add here what to do when Oomee selected. Currently just set to be bigger.
    for(int i=0;i<5;i++)
    {
        cocos2d::Sprite* oomee = (cocos2d::Sprite *)childAccountContent->getChildByName(StringUtils::format("%s_%d",OBJECTNAME_SPRITE_OOMEE,i));
        oomee->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1)));
    }
    
    selectedOomee->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.2)));
    
    this->selectedOomeeName = selectedOomee->getName();
}

void ChildAccountScene::doneButton()
{
    // NOTE Child Age to be in "yyyy-MM-dd" format
    
    //#TODO need to save selections
    auto childSelectorScene = ChildSelectorScene::createScene();
    Director::getInstance()->replaceScene(childSelectorScene);
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

bool ChildAccountScene::childNameExists(std::string newChildsName)
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
    
    return childNameExists;
}

//Editbox Delegate Functions
void ChildAccountScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
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
        
        //Return/Enter/Send Key pressed, action to next screen
        if(editBox->getName().compare(OBJECTNAME_EDITBOX_CHILDNAME) == 0)
            this->childNameNextButton();
    }
    
    
    if(editBox->getName().compare(OBJECTNAME_EDITBOX_CHILDNAME) == 0)
    {
        //if there is text, then show the next button.
        if(text.compare("") !=0)
            ((cocos2d::Sprite *)childAccountContent->getChildByName(getNextButtonName(OBJECTNAME_EDITBOX_CHILDNAME)))->setVisible(true);
        else
            ((cocos2d::Sprite *)childAccountContent->getChildByName(getNextButtonName(OBJECTNAME_EDITBOX_CHILDNAME)))->setVisible(false);
    }
    else
    {
        if(isDate())
            ((cocos2d::Sprite *)childAccountContent->getChildByName(getNextButtonName(OBJECTNAME_LABEL_DOB)))->setVisible(true);
        else
            ((cocos2d::Sprite *)childAccountContent->getChildByName(getNextButtonName(OBJECTNAME_LABEL_DOB)))->setVisible(false);
    }
}

void ChildAccountScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    //This function is mandatory for delegate.
    //This function is called if DONE pressed or if keyboard is closed. So cannot be used to detect DONE selection.
}

std::string ChildAccountScene::replace(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return "";
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return str;
}
