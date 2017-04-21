#include "MessageBox.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "LoginScene.h"
#include "OnboardingScene.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>

using namespace Azoomee;

#define MESSAGE_BOX_PADDING 100

Layer* MessageBox::createWith(std::string Title, std::string Body, std::vector<std::string> buttonTitleList, MessageBoxDelegate* _delegate)
{
    auto layer = MessageBox::create();
    
    layer->_buttonsTitleList = buttonTitleList;
    layer->initMessageBoxLayer(Title, Body, _delegate);
    
    return layer;
}

Layer* MessageBox::createWith(std::string Title, std::string Body, std::string Button, MessageBoxDelegate* _delegate)
{
    auto layer = MessageBox::create();
    
    layer->_buttonsTitleList.push_back(Button);
    layer->initMessageBoxLayer(Title, Body, _delegate);
    
    return layer;
}

Layer* MessageBox::createWith(long errorCode, MessageBoxDelegate* _delegate)
{
    return createWith(errorCode, nullptr , _delegate);
}

Layer* MessageBox::createWith(long errorCode, TextInputLayer* textInputToHide, MessageBoxDelegate* _delegate)
{
    std::map<std::string, std::string> errorStringMap = StringMgr::getInstance()->getErrorMessageWithCode(errorCode);
    
    auto layer = MessageBox::create();
    
    layer->hideTextInput(textInputToHide);
    layer->_buttonsTitleList.push_back(errorStringMap[ERROR_BUTTON]);
    layer->initMessageBoxLayer(errorStringMap[ERROR_TITLE], errorStringMap[ERROR_BODY], _delegate);
    
    return layer;
}

Layer* MessageBox::createPreviewLoginSignupMessageBox()
{
    auto layer = MessageBox::create();
    
    layer->_buttonsTitleList.push_back(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_IN));
    layer->_buttonsTitleList.push_back(StringMgr::getInstance()->getStringForKey(BUTTON_SIGN_UP));
    //layer->_buttonsTitleList.push_back(StringMgr::getInstance()->getStringForKey(BUTTON_CANCEL));
    layer->initMessageBoxLayer(StringMgr::getInstance()->getStringForKey(PREVIEW_MESSAGEBOX_TITLE_LABEL),StringMgr::getInstance()->getStringForKey(PREVIEW_MESSAGEBOX_BODY_LABEL),nullptr);
    
    return layer;
}

void MessageBox::initMessageBoxLayer(std::string Title, std::string Body, MessageBoxDelegate* _delegate)
{
    AnalyticsSingleton::getInstance()->messageBoxShowEvent(Title);
    
    if(_delegate)
        setDelegate(_delegate);

    _messageBoxTitle = Title;
    
    textMaxWidth = visibleSize.width*.66 - MESSAGE_BOX_PADDING*2;
    
    createBackgroundLayer();
    addSideWiresToScreen(this, 0, 2);
    createTitle();
    createBody(Body);
    createButtons();
    createCancelButton();
    createMessageWindow();
    addObjectsToWindow();
}

bool MessageBox::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void MessageBox::createBackgroundLayer()
{
    backgroundLayer = LayerColor::create(Color4B(15,14,7,255),origin.x+ visibleSize.width, origin.y + visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void MessageBox::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) 
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

//---------------------- Message Box Functions------------------------

void MessageBox::createTitle()
{
    messageTitleLabel = createLabelMessageBoxTitle(_messageBoxTitle);
    messageTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    messageTitleLabel->setWidth(textMaxWidth);
}

void MessageBox::createBody(std::string messageBody)
{
    messageBodyLabel = createLabelMessageBoxBody(messageBody);
    messageBodyLabel->setWidth(textMaxWidth);
}

void MessageBox::createButtons()
{
    buttonSpaceWidth = (visibleSize.width*.66 / _buttonsTitleList.size());
    
    if(_buttonsTitleList.size() == 1)
    {
        auto _button = ElectricDreamsButton::createButtonWithText(_buttonsTitleList.at(0));
        _button->setDelegate(this);
        buttonsList.push_back(_button);
    }
    else
    {
        for(int i=0;i < _buttonsTitleList.size(); i++)
        {
            auto _button = ElectricDreamsButton::createButtonWithWidth(_buttonsTitleList.at(i), buttonSpaceWidth * .75);
            _button->setDelegate(this);
            buttonsList.push_back(_button);
        }
    }
}

void MessageBox::createCancelButton()
{
    cancelButton = ElectricDreamsButton::createWindowCloselButton();
    cancelButton->setDelegate(this);
}

void MessageBox::createMessageWindow()
{
    float windowHeight = messageTitleLabel->getContentSize().height + messageBodyLabel->getContentSize().height + buttonsList.at(0)->getContentSize().height + cancelButton->getContentSize().height*4;
    
    windowLayer = createWindowLayer(windowHeight);
    windowLayer->setPosition(visibleSize.width/2- windowLayer->getContentSize().width/2,origin.y + (visibleSize.height - windowLayer->getContentSize().height) * .66);
    this->addChild(windowLayer);
}

void MessageBox::addObjectsToWindow()
{
    float nextItemHeight = windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75;
    
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, nextItemHeight));
    windowLayer->addChild(cancelButton);
    
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height*.25 - messageTitleLabel->getContentSize().height/2;
    
    messageTitleLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    windowLayer->addChild(messageTitleLabel);
    
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height - messageTitleLabel->getContentSize().height/2 - messageBodyLabel->getContentSize().height/2;
    
    messageBodyLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    windowLayer->addChild(messageBodyLabel);
    
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height - messageBodyLabel->getContentSize().height/2 - buttonsList.at(0)->getContentSize().height/2;
    
    positionButtonsBasedOnWidth(nextItemHeight);
}

void MessageBox::positionButtonsBasedOnWidth(float nextItemHeight)
{
    for(int i=0;i < _buttonsTitleList.size(); i++)
    {
        buttonsList.at(i)->setCenterPosition(Vec2(buttonSpaceWidth/2 + buttonSpaceWidth*i, nextItemHeight));
        windowLayer->addChild(buttonsList.at(i));
    }
}

//---------------------- Actions -----------------

void MessageBox::removeSelf(float dt)
{
    if(this)
    {
        this->removeChild(backgroundLayer);
        this->removeFromParent();
    }
}

//-------------------- Object To Hide Functions ----------------------

void MessageBox::hideTextInput(TextInputLayer* textInputToHide)
{
    if(textInputToHide)
    {
        textInputToHide->setEditboxVisibility(false);
        savedTextInputToHide = textInputToHide;
    }
    else
        savedTextInputToHide = nullptr;
}

void MessageBox::UnHideTextInput()
{
    if(savedTextInputToHide)
        savedTextInputToHide->setEditboxVisibility(true);
}

//----------------------- Delegate Functions ----------------------------

void MessageBox::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
    {
        if(_messageBoxTitle == StringMgr::getInstance()->getStringForKey(PREVIEW_MESSAGEBOX_TITLE_LABEL))
            AnalyticsSingleton::getInstance()->previewPopupCancelledEvent();
        
        this->scheduleOnce(schedule_selector(MessageBox::removeSelf), 0.1);
        UnHideTextInput();
        if(_delegate)
            this->getDelegate()->MessageBoxButtonPressed(_messageBoxTitle, "Cancel");
    }
    else
    {
        for(int i=0;i < buttonsList.size(); i++)
        {
            if(buttonsList.at(i) == button)
            {
                if(_messageBoxTitle == StringMgr::getInstance()->getStringForKey(PREVIEW_MESSAGEBOX_TITLE_LABEL))
                {
                    handlePreviewLoginSignupMessageBoxSelection(i);
                }
                else
                {
                    //To enable call to delegate and avoid crash, schedule remove for after delegate call.
                    this->scheduleOnce(schedule_selector(MessageBox::removeSelf), 0.1);
                    UnHideTextInput();
                    if(_delegate)
                        this->getDelegate()->MessageBoxButtonPressed(_messageBoxTitle, _buttonsTitleList.at(i));
                }
            }
        }
    }
}

void MessageBox::handlePreviewLoginSignupMessageBoxSelection(int buttonSelect)
{
    if(_buttonsTitleList.at(buttonSelect) == StringMgr::getInstance()->getStringForKey(BUTTON_LOG_IN))
    {
        Scene *loginScene = LoginScene::createScene(0);
        Director::getInstance()->replaceScene(loginScene);
    }
    else if(_buttonsTitleList.at(buttonSelect) == StringMgr::getInstance()->getStringForKey(BUTTON_SIGN_UP))
    {
        Scene *onboardingScene = OnboardingScene::createScene(0);
        Director::getInstance()->replaceScene(onboardingScene);
    }
}
