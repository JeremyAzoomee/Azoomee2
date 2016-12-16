#include "ChildAccountScene.h"
#include "SimpleAudioEngine.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"
#include "DataStorage.h"
#include "ChildSelectorScene.h"

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

//Define Editbox Tag Numbers
#define TAG_CHILDNAME_EDITBOX 31
#define TAG_DAY_EDITBOX 32
#define TAG_MONTH_EDITBOX 33
#define TAG_YEAR_EDITBOX 34

//Define BUTTONS Tag Numbers
#define TAG_CHILD_CLOSE_BUTTON 10
#define TAG_CHILD_NEXT_BUTTON 11
#define TAG_DOB_BACK_BUTTON 12
#define TAG_DOB_NEXT_BUTTON 13
#define TAG_OOMEE_BACK_BUTTON 14
#define TAG_OOMEE_NEXT_BUTTON 15

//Define LABEL Tag Numbers
#define TAG_DOB_LABEL 20
#define TAG_OOMEE_LABEL 21

//Define OOMEE Tag Numbers
#define TAG_OOMEE_0 0
#define TAG_OOMEE_1 1
#define TAG_OOMEE_2 2
#define TAG_OOMEE_3 3
#define TAG_OOMEE_4 4

#define NO_OF_OOMEES 5



Scene* ChildAccountScene::createScene(std::string ChildName, long errorCode)
{
    //Create with errorCode == 0 (Zero) if there are no errors
    
    auto scene = Scene::create();
    auto layer = ChildAccountScene::create();
    scene->addChild(layer);
    layer->setChildName(ChildName);
    layer->addFunctionalElementsToScene();
    
    layer->_errorCode = errorCode;
    
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

void ChildAccountScene::onEnterTransitionDidFinish()
{
    if(_errorCode !=0)
    {
        handleErrorCode(_errorCode);
    }
}

void ChildAccountScene::handleErrorCode(long errorCode)
{
    //#TODO handle modal message strings.
    ModalMessages::getInstance()->createMessageWithSingleButton("ERROR", StringUtils::format("Error Code:%ld",errorCode), "OK");
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
    childAccountContent->setName("childAccountContent");
    this->addChild(childAccountContent);
}

void ChildAccountScene::addLabelsToLayer()
{
    auto addChildNameTitle = Label::create();
    addChildNameTitle->setSystemFontName("fonts/azoomee.ttf");
    addChildNameTitle->setSystemFontSize(90);
    
    //Add diferent text depending on new or editing account
    if(this->isNewChildAccount)
        addChildNameTitle->setString(LABEL_NEW_REQUEST_CHILD_NAME);
    else
        addChildNameTitle->setString(LABEL_EDIT_CHILD_NAME);
    
    addChildNameTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    addChildNameTitle->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(addChildNameTitle);
    
    auto DOB_Title = Label::create();
    DOB_Title->setSystemFontName("fonts/azoomee.ttf");
    DOB_Title->setSystemFontSize(90);
    DOB_Title->setTag(TAG_DOB_LABEL);
    DOB_Title->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    DOB_Title->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(DOB_Title);
    
    auto selectOomeeTitle = Label::create();
    selectOomeeTitle->setSystemFontName("fonts/azoomee.ttf");
    selectOomeeTitle->setSystemFontSize(90);
    selectOomeeTitle->setTag(TAG_OOMEE_LABEL);
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
    _editName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editName->setText(this->passedChildName.c_str());
    _editName->setDelegate(this);
    _editName->setTag(TAG_CHILDNAME_EDITBOX);
    childAccountContent->addChild(_editName);
    

    auto _editDOB_Day = ui::EditBox::create(Size(200,131), "res/login/textarea_bg.png");
    _editDOB_Day->setPosition(Vec2(origin.x+visibleSize.width * 1.5 - 300, origin.y+visibleSize.height*0.5));
    _editDOB_Day->setFont("fonts/azoomee.ttf", 90);
    _editDOB_Day->setFontColor(Color3B::WHITE);
    _editDOB_Day->setMaxLength(2);
    _editDOB_Day->setPlaceHolder(EDITBOX_PLACEHOLDER_DOB_DAY);
    _editDOB_Day->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editDOB_Day->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editDOB_Day->setTag(TAG_DAY_EDITBOX);
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
    _editDOB_Month->setTag(TAG_MONTH_EDITBOX);
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
    _editDOB_Year->setTag(TAG_YEAR_EDITBOX);
    _editDOB_Year->setDelegate(this);
    childAccountContent->addChild(_editDOB_Year);
    
}

void ChildAccountScene::addButtonsToLayer()
{
    // in order they appear on the screen with next, then back, then next etc.
    auto closeBackButton = Sprite::create("res/login/back_btn.png");
    closeBackButton->setPosition(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.5);
    closeBackButton->setTag(TAG_CHILD_CLOSE_BUTTON);
    addListenerToButton(closeBackButton);
    childAccountContent->addChild(closeBackButton);
    
    auto childNameNextButton = Sprite::create("res/login/next_btn.png");
    childNameNextButton->setPosition(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.5);
    childNameNextButton->setTag(TAG_CHILD_NEXT_BUTTON);
    addListenerToButton(childNameNextButton);
    
    //only show button if there is text
    if(this->isNewChildAccount)
        childNameNextButton->setVisible(false);
    
    childAccountContent->addChild(childNameNextButton);
    
    auto DOB_BackButton = Sprite::create("res/login/back_btn.png");
    DOB_BackButton->setPosition(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5);
    DOB_BackButton->setTag(TAG_DOB_BACK_BUTTON);
    addListenerToButton(DOB_BackButton);
    childAccountContent->addChild(DOB_BackButton);
    
    auto DOB_NextButton = Sprite::create("res/login/next_btn.png");
    DOB_NextButton->setPosition(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5);
    DOB_NextButton->setTag(TAG_DOB_NEXT_BUTTON);
    DOB_NextButton->setVisible(false);
    addListenerToButton(DOB_NextButton);
    childAccountContent->addChild(DOB_NextButton);
    
    auto selectOomeeBackButton = Sprite::create("res/login/back_btn.png");
    selectOomeeBackButton->setPosition(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5);
    selectOomeeBackButton->setTag(TAG_OOMEE_BACK_BUTTON);
    addListenerToButton(selectOomeeBackButton);
    childAccountContent->addChild(selectOomeeBackButton);
    
    auto selectOomeeDoneButton = Sprite::create("res/login/next_btn.png");
    selectOomeeDoneButton->setPosition(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5);
    selectOomeeDoneButton->setScale(1.2);
    selectOomeeDoneButton->setVisible(false);
    selectOomeeDoneButton->setTag(TAG_OOMEE_NEXT_BUTTON);
    addListenerToButton(selectOomeeDoneButton);
    childAccountContent->addChild(selectOomeeDoneButton);

}

void ChildAccountScene::addOomeesToLayer()
{
    float gapSize = 50;
    float oomeeWidth = 190;
    float startX = origin.x + visibleSize.width * 2.5 - (oomeeWidth*2 + gapSize*2);
    
    for(int i = 0; i < NO_OF_OOMEES; i++)
    {
        auto oomee = Sprite::create(StringUtils::format("res/childSelection/oomee_%d.png",i));
        oomee->setPosition(startX + ((oomeeWidth+gapSize)*i), origin.y + visibleSize.height * 0.4);
        oomee->setOpacity(0);
        oomee->setTag(i);
        addListenerToButton(oomee);
        childAccountContent->addChild(oomee);
    }
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
        
        if(rect.containsPoint(locationInNode) && spriteImage->isVisible())
        {
            int buttonTag = spriteImage->getTag();
            
            if(buttonTag < NO_OF_OOMEES)
                //then handle an Oomee being selected
                this->selectOomee(spriteImage);
            else
            {
                switch(buttonTag)
                {
                    case(TAG_CHILD_CLOSE_BUTTON): this->backButtonCloseScene(); break;
                    case(TAG_CHILD_NEXT_BUTTON): this->childNameNextButton(spriteImage); break;
                    case(TAG_DOB_BACK_BUTTON): this->moveToChildNameInput(spriteImage); break;
                    case(TAG_DOB_NEXT_BUTTON):  this->moveToOomeeSelection(spriteImage); break;
                    case(TAG_OOMEE_BACK_BUTTON):  this->moveToDOBInput(spriteImage); break;
                    case(TAG_OOMEE_NEXT_BUTTON):  this->doneButton(); break;
                }
            }
            
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, spriteImage);
}

void ChildAccountScene::disableMoveButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(button);
}

void ChildAccountScene::enableMoveButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(button);
}

void ChildAccountScene::backButtonCloseScene()
{
    auto childSelectorScene = ChildSelectorScene::createScene(0);
    Director::getInstance()->replaceScene(childSelectorScene);
}

void ChildAccountScene::childNameNextButton(Node* button)
{
    //Set the childDOB label depending on if new
    setDOBLabel();
    
    //Check if editbox is not null, and does not exist, then move onwards
    
    std::string newChildsName = ((ui::EditBox *)childAccountContent->getChildByTag(TAG_CHILDNAME_EDITBOX))->getText();
    
    if(this->passedChildName == newChildsName && !this-isNewChildAccount)
    {
        //If the childs name has not changed, then move forward.
        //This stops a false error of "CHILD NAME EXISTS"

        moveToDOBInput(button);
    }
    else
    {
        if(childNameExists(newChildsName))
        {
            //#TODO handle modal message strings.
            ModalMessages::getInstance()->createMessageWithSingleButton("Display Name Exists", "Please insert a unique name.", "OK");
        }
        else
            moveToDOBInput(button);
    }
}

void ChildAccountScene::moveToDOBInput(Node* button)
{
    disableMoveButton(button);
    
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(ChildAccountScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    childAccountContent->runAction(sequence);
}

void ChildAccountScene::setDOBLabel()
{
    //Set correct label Depending if new Child
    Label* DOB_Title = ((Label *)childAccountContent->getChildByTag(TAG_DOB_LABEL));
    
    if(this->isNewChildAccount)
    {
        std::string newChildsName = ((ui::EditBox *)childAccountContent->getChildByTag(TAG_CHILDNAME_EDITBOX))->getText();
        
        DOB_Title->setString(StringUtils::format("%s %s.", LABEL_NEW_REQUEST_CHILD_BIRTHDAY,newChildsName.c_str()));
    }
    else
        DOB_Title->setString(LABEL_EDIT_REQUEST_CHILD_BIRTHDAY);
}

void ChildAccountScene::moveToChildNameInput(Node* button)
{
    disableMoveButton(button);
    
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(ChildAccountScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    childAccountContent->runAction(sequence);
}

void ChildAccountScene::moveToOomeeSelection(Node* button)
{
    disableMoveButton(button);
    
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width*2 + origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(ChildAccountScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    childAccountContent->runAction(sequence);
    
    for(int i=0;i<NO_OF_OOMEES;i++)
    {
        cocos2d::Sprite* oomee = (cocos2d::Sprite *)childAccountContent->getChildByTag(i);
        float delayTime = (CCRANDOM_0_1()* 0.5) +1;
        oomee->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    }
}

void ChildAccountScene::selectOomee(cocos2d::Sprite* selectedOomee)
{
    ((cocos2d::Sprite *)childAccountContent->getChildByTag(TAG_OOMEE_NEXT_BUTTON))->setVisible(true);
    
    //Add here what to do when Oomee selected. Currently just set to be bigger.
    for(int i=0;i<NO_OF_OOMEES;i++)
    {
        cocos2d::Sprite* oomee = (cocos2d::Sprite *)childAccountContent->getChildByTag(i);
        oomee->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1)));
    }
    
    selectedOomee->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.2)));
    
    this->selectedOomeeNo = selectedOomee->getTag();
}

void ChildAccountScene::doneButton()
{
    std::string profileName = ((ui::EditBox *)childAccountContent->getChildByTag(TAG_CHILDNAME_EDITBOX))->getText();
    
    int d = atoi(((ui::EditBox *)childAccountContent->getChildByTag(TAG_DAY_EDITBOX))->getText());
    int m = atoi(((ui::EditBox *)childAccountContent->getChildByTag(TAG_MONTH_EDITBOX))->getText());
    int y = atoi(((ui::EditBox *)childAccountContent->getChildByTag(TAG_YEAR_EDITBOX))->getText());
    
    std::string DOB = StringUtils::format("%04d-%02d-%02d",y,m,d);
    std::string gender = "MALE";
    
    // NOTE Child Age to be in "yyyy-MM-dd" format
    
    if(this->isNewChildAccount)
    {
       auto backEndCaller = BackEndCaller::getInstance();
        backEndCaller->registerChild(profileName, gender, DOB, this->selectedOomeeNo);
    }
    else
    {
        //NEED TO SEND CHANGE REQUEST, and handle error passing of this->passedChildName to rebuilt scene.
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

bool ChildAccountScene::isDate()
{
    int d = atoi(((ui::EditBox *)childAccountContent->getChildByTag(TAG_DAY_EDITBOX))->getText());
    int m = atoi(((ui::EditBox *)childAccountContent->getChildByTag(TAG_MONTH_EDITBOX))->getText());
    int y = atoi(((ui::EditBox *)childAccountContent->getChildByTag(TAG_YEAR_EDITBOX))->getText());
    
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
    //Make the next buttons visible if the inputs are in correct format
    
    if(editBox->getTag() == TAG_CHILDNAME_EDITBOX)
        //if there is text, then show the next button.
        ((cocos2d::Sprite *)childAccountContent->getChildByTag(TAG_CHILD_NEXT_BUTTON))->setVisible(text != "");
    else
        ((cocos2d::Sprite *)childAccountContent->getChildByTag(TAG_DOB_NEXT_BUTTON))->setVisible(isDate());
}

void ChildAccountScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    //This function is mandatory for delegate.
    //This function is called if DONE pressed or if keyboard is closed. So cannot be used to detect DONE selection.
}
