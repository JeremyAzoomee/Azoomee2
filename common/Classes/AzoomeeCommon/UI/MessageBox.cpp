#include "MessageBox.h"
#include "../Strings.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"

using namespace cocos2d;

#define MESSAGE_BOX_PADDING 100


namespace Azoomee
{

MessageBox* MessageBox::createWith(std::string Title, std::string Body, std::vector<std::string> buttonTitleList, MessageBoxDelegate* _delegate)
{
    auto layer = MessageBox::create();
    
    layer->_buttonsTitleList = buttonTitleList;
    layer->initMessageBoxLayer(Title, Body, _delegate);
    
    return layer;
}

MessageBox* MessageBox::createWith(std::string Title, std::string Body, std::string Button, MessageBoxDelegate* _delegate)
{
    auto layer = MessageBox::create();
    
    layer->_buttonsTitleList.push_back(Button);
    layer->initMessageBoxLayer(Title, Body, _delegate);
    
    return layer;
}

MessageBox* MessageBox::createWith(long errorCode, MessageBoxDelegate* _delegate)
{
    return createWith(errorCode, nullptr , _delegate);
}

MessageBox* MessageBox::createWith(long errorCode, TextInputLayer* textInputToHide, MessageBoxDelegate* _delegate)
{
    std::map<std::string, std::string> errorStringMap = StringMgr::getInstance()->getErrorMessageWithCode(errorCode);
    
    auto layer = MessageBox::create();
    
    layer->hideTextInput(textInputToHide);
    layer->_buttonsTitleList.push_back(errorStringMap[ERROR_BUTTON]);
    layer->initMessageBoxLayer(errorStringMap[ERROR_TITLE], errorStringMap[ERROR_BODY], _delegate);
    
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
    
void MessageBox::setBodyHAlignment(TextHAlignment align)
{
    messageBodyLabel->setHorizontalAlignment(align);
}

//---------------------- Create Layer -----------------------------

void MessageBox::createBackgroundLayer()
{
    backgroundLayer = LayerColor::create(Color4B(15,14,7,255),visibleSize.width, visibleSize.height);
    backgroundLayer->setPosition(origin);
    
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
        log("MESSAGEBOX TOUCHT %f x %f",touch->getPreviousLocationInView().x,touch->getPreviousLocationInView().y);
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

//---------------------- Message Box Functions------------------------

void MessageBox::addButtonWithTitle(const std::string& buttonTitle)
{
    _buttonsTitleList.push_back(buttonTitle);
}
  
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
    
    const float maxHeight = visibleSize.height * 0.5f;
    Size size = messageBodyLabel->getContentSize();
    size.width = textMaxWidth;
    
    bool needScroll = false;
    if(size.height > maxHeight)
    {
        size.height = maxHeight;
        needScroll = true;
    }
    
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize(size);
    scrollView->setAnchorPoint(Vec2(0.5f, 0.5f));
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    scrollView->setBounceEnabled(needScroll);
    scrollView->setTouchEnabled(needScroll);
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(needScroll);
    scrollView->setInnerContainerSize(messageBodyLabel->getContentSize());
    
    scrollView->addChild(messageBodyLabel);
    
    messageBodyLabel->setAnchorPoint(Vec2(0, 0));
    messageBodyLabel->setPosition(0, 0);
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
    float windowHeight = messageTitleLabel->getContentSize().height + scrollView->getContentSize().height + buttonsList.at(0)->getContentSize().height + cancelButton->getContentSize().height*4;
    
    windowLayer = createWindowLayer(windowHeight);
    windowLayer->setPosition(visibleSize.width/2- windowLayer->getContentSize().width/2,origin.y + (visibleSize.height - windowLayer->getContentSize().height) * .66);
    this->addChild(windowLayer);
}

void MessageBox::addObjectsToWindow()
{
    float nextItemHeight = windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75;
    
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, nextItemHeight));
    windowLayer->addChild(cancelButton);
    
    log("TEST");
    log("MESSAGEBOX CANCELBUTTON POSITION %f  x %f",cancelButton->getCenterPosition().x,cancelButton->getCenterPosition().y);
    
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height*.25 - messageTitleLabel->getContentSize().height/2;
    
    messageTitleLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    windowLayer->addChild(messageTitleLabel);
    
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height - messageTitleLabel->getContentSize().height/2 - scrollView->getContentSize().height/2;
    
    scrollView->setPosition(Vec2(windowLayer->getContentSize().width/2, nextItemHeight));
    windowLayer->addChild(scrollView);
    
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height - scrollView->getContentSize().height/2 - buttonsList.at(0)->getContentSize().height/2;
    
    positionButtonsBasedOnWidth(nextItemHeight);
}

void MessageBox::positionButtonsBasedOnWidth(float nextItemHeight)
{
    for(int i=0;i < _buttonsTitleList.size(); i++)
    {
        buttonsList.at(i)->setCenterPosition(Vec2(buttonSpaceWidth/2 + buttonSpaceWidth*i, nextItemHeight));
        windowLayer->addChild(buttonsList.at(i));
        
        log("MESSAGEBOX BUTTON POSITION %f  x %f",buttonsList.at(i)->getCenterPosition().x,buttonsList.at(i)->getCenterPosition().y);
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
        onCancelPressed();
    }
    else
    {
        for(int i=0;i < buttonsList.size(); i++)
        {
            if(buttonsList.at(i) == button)
            {
                onButtonPressed(i);
                // We can stop searching now
                break;
            }
        }
    }
}
  
void MessageBox::onCancelPressed()
{
    this->scheduleOnce(schedule_selector(MessageBox::removeSelf), 0.1);
    UnHideTextInput();
    if(_delegate)
        this->getDelegate()->MessageBoxButtonPressed(_messageBoxTitle, "Cancel");
}

void MessageBox::onButtonPressed(int buttonSelect)
{
    //To enable call to delegate and avoid crash, schedule remove for after delegate call.
    this->scheduleOnce(schedule_selector(MessageBox::removeSelf), 0.1);
    UnHideTextInput();
    if(_delegate)
        this->getDelegate()->MessageBoxButtonPressed(_messageBoxTitle, _buttonsTitleList.at(buttonSelect));
}
  
}
