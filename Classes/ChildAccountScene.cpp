#include "ChildAccountScene.h"
#include "SimpleAudioEngine.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"
#include "DataStorage.h"
#include "ChildSelectorScene.h"
#include "TextInputChecker.h"
#include "StringStorage.h"

USING_NS_CC;

#define NO_OF_OOMEES 5

Scene* ChildAccountScene::createScene(std::string ChildName, long errorCode)
{
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
    
    return true;
}

void ChildAccountScene::onEnterTransitionDidFinish()
{
    if(_errorCode !=0)
    {
        handleErrorCode(_errorCode);
    }
}

//--------------------------------------Only internal calls beyond this line-------------------------------------------------------

void ChildAccountScene::handleErrorCode(long errorCode)
{
    ModalMessages::getInstance()->createMessageWithSingleButton("ERROR", StringUtils::format("Error Code:%ld",errorCode), "OK");
}

void ChildAccountScene::setChildName(std::string ChildName)
{
    this->passedChildName = ChildName;

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
        addChildNameTitle->setString(StringStorage::getInstance()->getStringForChildAccount("requestChildName"));
    else
        addChildNameTitle->setString(StringStorage::getInstance()->getStringForChildAccount("editChildName"));
    
    addChildNameTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    addChildNameTitle->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(addChildNameTitle);
    
    labelDob = Label::create();
    labelDob->setSystemFontName("fonts/azoomee.ttf");
    labelDob->setSystemFontSize(90);
    labelDob->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    labelDob->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(labelDob);
    
    labelOomee = Label::create();
    labelOomee->setSystemFontName("fonts/azoomee.ttf");
    labelOomee->setSystemFontSize(90);
    if(this->isNewChildAccount)
        labelOomee->setString(StringStorage::getInstance()->getStringForChildAccount("requestChildOomee"));
    else
        labelOomee->setString(StringStorage::getInstance()->getStringForChildAccount("editChildName"));
    labelOomee->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    labelOomee->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(labelOomee);
    
    if(this->isNewChildAccount)
    {
        auto selectOomeeDetail = Label::createWithTTF(StringStorage::getInstance()->getStringForChildAccount("requestChildOomeeSub"), "fonts/azoomee.ttf", 60);
        selectOomeeDetail->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.6);
        selectOomeeDetail->setColor(Color3B::WHITE);
        childAccountContent->addChild(selectOomeeDetail);
    }
}

void ChildAccountScene::addTextBoxesToLayer()
{
    editBox_childName = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    editBox_childName->setColor(Color3B::WHITE);
    editBox_childName->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.5));
    editBox_childName->setFont("fonts/azoomee.ttf", 90);
    editBox_childName->setFontColor(Color3B::WHITE);
    editBox_childName->setMaxLength(50);
    editBox_childName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    editBox_childName->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
    editBox_childName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    editBox_childName->setText(this->passedChildName.c_str());
    editBox_childName->setDelegate(this);
    childAccountContent->addChild(editBox_childName);
    
    editBox_day = ui::EditBox::create(Size(200,131), "res/login/textarea_bg.png");
    editBox_day->setPosition(Vec2(origin.x+visibleSize.width * 1.5 - 300, origin.y+visibleSize.height*0.5));
    editBox_day->setFont("fonts/azoomee.ttf", 90);
    editBox_day->setFontColor(Color3B::WHITE);
    editBox_day->setMaxLength(2);
    editBox_day->setPlaceHolder(StringStorage::getInstance()->getStringForChildAccount("phDobDay").c_str());
    editBox_day->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    editBox_day->setInputMode(ui::EditBox::InputMode::NUMERIC);
    editBox_day->setDelegate(this);
    childAccountContent->addChild(editBox_day);
    
    editBox_month = ui::EditBox::create(Size(200,131), "res/login/textarea_bg.png");
    editBox_month->setPosition(Vec2(origin.x+visibleSize.width * 1.5 - 50, origin.y+visibleSize.height*0.5));
    editBox_month->setFont("fonts/azoomee.ttf", 90);
    editBox_month->setFontColor(Color3B::WHITE);
    editBox_month->setMaxLength(2);
    editBox_month->setPlaceHolder(StringStorage::getInstance()->getStringForChildAccount("phDobMonth").c_str());
    editBox_month->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    editBox_month->setInputMode(ui::EditBox::InputMode::NUMERIC);
    editBox_month->setDelegate(this);
    childAccountContent->addChild(editBox_month);
    
    editBox_year = ui::EditBox::create(Size(300,131), "res/login/textarea_bg.png");
    editBox_year->setPosition(Vec2(origin.x+visibleSize.width * 1.5 + 250, origin.y+visibleSize.height*0.5));
    editBox_year->setFont("fonts/azoomee.ttf", 90);
    editBox_year->setFontColor(Color3B::WHITE);
    editBox_year->setMaxLength(4);
    editBox_year->setPlaceHolder(StringStorage::getInstance()->getStringForChildAccount("phDobYear").c_str());
    editBox_year->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    editBox_year->setInputMode(ui::EditBox::InputMode::NUMERIC);
    editBox_year->setDelegate(this);
    childAccountContent->addChild(editBox_year);
    
}

void ChildAccountScene::addButtonsToLayer()
{
    // in order they appear on the screen with next, then back, then next etc.
    buttonCloseChild = Sprite::create("res/login/back_btn.png");
    buttonCloseChild->setPosition(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.5);
    addListenerToButton(buttonCloseChild);
    childAccountContent->addChild(buttonCloseChild);
    
    buttonNextChild = Sprite::create("res/login/next_btn.png");
    buttonNextChild->setPosition(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.5);
    addListenerToButton(buttonNextChild);
    
    //only show button if there is text
    if(this->isNewChildAccount)
        buttonNextChild->setVisible(false);
    
    childAccountContent->addChild(buttonNextChild);
    
    buttonBackDob = Sprite::create("res/login/back_btn.png");
    buttonBackDob->setPosition(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5);
    addListenerToButton(buttonBackDob);
    childAccountContent->addChild(buttonBackDob);
    
    buttonNextDob = Sprite::create("res/login/next_btn.png");
    buttonNextDob->setPosition(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5);
    buttonNextDob->setVisible(false);
    addListenerToButton(buttonNextDob);
    childAccountContent->addChild(buttonNextDob);
    
    buttonBackOomee = Sprite::create("res/login/back_btn.png");
    buttonBackOomee->setPosition(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5);
    addListenerToButton(buttonBackOomee);
    childAccountContent->addChild(buttonBackOomee);
    
    buttonNextOomee = Sprite::create("res/login/next_btn.png");
    buttonNextOomee->setPosition(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5);
    buttonNextOomee->setScale(1.2);
    buttonNextOomee->setVisible(false);
    addListenerToButton(buttonNextOomee);
    childAccountContent->addChild(buttonNextOomee);
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
            if(spriteImage == buttonCloseChild) this->backButtonCloseScene();
            else if(spriteImage == buttonNextChild) this->childNameNextButtonPressed(spriteImage);
            else if(spriteImage == buttonBackDob) this->moveToChildNameInput(spriteImage);
            else if(spriteImage == buttonNextDob) this->moveToOomeeSelection(spriteImage);
            else if(spriteImage == buttonBackOomee) this->moveToDOBInput(spriteImage);
            else if(spriteImage == buttonNextOomee) this->registerChildAccount();
            else this->selectOomee(spriteImage);
            
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

void ChildAccountScene::childNameNextButtonPressed(Node* button)
{
    setDOBLabel();
    
    std::string newChildsName = editBox_childName->getText();
    
    if(this->passedChildName == newChildsName && !this-isNewChildAccount)
    {
        moveToDOBInput(button);
    }
    else
    {
        TextInputChecker *textInputChecker = new TextInputChecker();
        if(textInputChecker->childNameExists(newChildsName))
        {
            ModalMessages::getInstance()->createMessageWithSingleButton("Display Name Exists", "Please insert a unique name.", "OK");
        }
        else moveToDOBInput(button);
        
        textInputChecker->release();
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
    if(this->isNewChildAccount)
    {
        std::string newChildsName = editBox_childName->getText();
        labelDob->setString(StringUtils::format("%s %s.", StringStorage::getInstance()->getStringForChildAccount("editChildBirthDay").c_str(), newChildsName.c_str()));
    }
    else
        labelDob->setString(StringStorage::getInstance()->getStringForChildAccount("editChildBirthDay").c_str());
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
    buttonNextOomee->setVisible(true);
    
    for(int i=0;i<NO_OF_OOMEES;i++)
    {
        cocos2d::Sprite* oomee = (cocos2d::Sprite *)childAccountContent->getChildByTag(i);
        oomee->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1)));
    }
    
    selectedOomee->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.2)));
    
    this->selectedOomeeNo = selectedOomee->getTag();
}

void ChildAccountScene::registerChildAccount()
{
    std::string profileName = editBox_childName->getText();
    
    int day = atoi(editBox_day->getText());
    int month = atoi(editBox_month->getText());
    int year = atoi(editBox_year->getText());
    
    std::string DOB = StringUtils::format("%04d-%02d-%02d",year,month,day);
    std::string gender = "MALE";
    
    if(this->isNewChildAccount)
    {
       auto backEndCaller = BackEndCaller::getInstance();
        backEndCaller->registerChild(profileName, gender, DOB, this->selectedOomeeNo);
    }
}

//Editbox Delegate Functions
void ChildAccountScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    TextInputChecker *textInputChecker = new TextInputChecker();
    
    if(editBox == editBox_childName)
    {
        bool canGoForward = false;
        if(text != "") canGoForward = true;
        buttonNextChild->setVisible(canGoForward);
    }
    else
    {
        int day = atoi(editBox_day->getText());
        int month = atoi(editBox_month->getText());
        int year = atoi(editBox_year->getText());
        
        bool canGoForward = textInputChecker->isDate(day, month, year);
        buttonNextDob->setVisible(canGoForward);
    }
}

void ChildAccountScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    //This function is mandatory for delegate.
    //This function is called if DONE pressed or if keyboard is closed. So cannot be used to detect DONE selection.
}
