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
    layer->initMessageBoxLayer(Title, Body, _delegate, 0);
    
    return layer;
}

MessageBox* MessageBox::createWith(std::string Title, std::string Body, std::string Button, MessageBoxDelegate* _delegate)
{
    auto layer = MessageBox::create();
    
    layer->_buttonsTitleList.push_back(Button);
    layer->initMessageBoxLayer(Title, Body, _delegate, 0);
    
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
    layer->_buttonsTitleList = splitStringToVector(errorStringMap[ERROR_BUTTON], "|");
    
    if(errorStringMap.at(ERROR_BUTTON_REFERENCE) != "")
        layer->_buttonsReferenceList = splitStringToVector(errorStringMap[ERROR_BUTTON_REFERENCE], "|");
    
    layer->initMessageBoxLayer(errorStringMap[ERROR_TITLE], errorStringMap[ERROR_BODY], _delegate, errorCode);
    
    return layer;
}

void MessageBox::initMessageBoxLayer(std::string Title, std::string Body, MessageBoxDelegate* _delegate, long errorCode)
{
    AnalyticsSingleton::getInstance()->messageBoxShowEvent(Title,errorCode);
    
    if(_delegate)
        setDelegate(_delegate);

    _messageBoxTitle = Title;
    _messageBoxBody = Body;
    
    createBackgroundLayer();
    onSizeChanged();
}

bool MessageBox::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setName("MessageBoxLayer");
    return true;
}
    
void MessageBox::setBodyHAlignment(TextHAlignment align)
{
    messageBodyLabel->setHorizontalAlignment(align);
}

//---------------------- Create Layer -----------------------------

void MessageBox::createBackgroundLayer()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    backgroundLayer = LayerColor::create(Color4B(15,14,7,255),currentRunningScene->getContentSize().width, currentRunningScene->getContentSize().height);
    this->addChild(backgroundLayer);
    
    Director::getInstance()->getRunningScene()->addChild(this);
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

void MessageBox::createBody()
{
    messageBodyLabel = createLabelMessageBoxBody(_messageBoxBody);
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
    buttonSpaceWidth = (visibleSize.width*percentageOfScreenForBox / _buttonsTitleList.size());
    
    if(_buttonsTitleList.size() == 1)
    {
        auto _button = ElectricDreamsButton::createButtonWithText(_buttonsTitleList.at(0));
        _button->setMixPanelButtonName(StringUtils::format("messageBoxButton-%s", _buttonsTitleList.at(0).c_str()));
        _button->setDelegate(this);
        buttonsList.push_back(_button);
    }
    else
    {
        for(int i=0;i < _buttonsTitleList.size(); i++)
        {
            auto _button = ElectricDreamsButton::createButtonWithWidth(_buttonsTitleList.at(i), buttonSpaceWidth * .75);
            _button->setMixPanelButtonName(StringUtils::format("messageBoxButton-%s", _buttonsTitleList.at(i).c_str()));
            _button->setDelegate(this);
            buttonsList.push_back(_button);
        }
    }
}

void MessageBox::createCancelButton()
{
    cancelButton = ElectricDreamsButton::createWindowCloselButton();
    cancelButton->setMixPanelButtonName("messageBoxCancelButton");
    cancelButton->setDelegate(this);
}

void MessageBox::createMessageWindow()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    
    float windowHeight = messageTitleLabel->getContentSize().height + scrollView->getContentSize().height + buttonsList.at(0)->getContentSize().height + cancelButton->getContentSize().height*4;
    
    windowLayer = createWindowLayer(visibleSize.width * percentageOfScreenForBox, windowHeight);
    windowLayer->setPosition(currentRunningScene->getContentSize().width/2- windowLayer->getContentSize().width/2,(currentRunningScene->getContentSize().height - windowLayer->getContentSize().height) * 0.66);
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
    
void MessageBox::onSizeChanged()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    backgroundLayer->setContentSize(currentRunningScene->getContentSize());
    backgroundLayer->removeAllChildren();
    
    if(windowLayer)
        windowLayer->removeAllChildren();
    
    buttonsList.clear();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    if(currentRunningScene->getContentSize().width < currentRunningScene->getContentSize().height)
    {
        percentageOfScreenForBox = 0.85;
        isLandscape = false;
    }
    else
    {
        percentageOfScreenForBox = 0.66;
        isLandscape = true;
    }
    
    textMaxWidth = visibleSize.width*percentageOfScreenForBox - MESSAGE_BOX_PADDING*2;

    if(isLandscape)
        addSideWiresToScreen(backgroundLayer, 0, 2);
    createTitle();
    createBody();
    createButtons();
    createCancelButton();
    createMessageWindow();
    addObjectsToWindow();
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
    {
        if(_buttonsReferenceList.size() >0)
            this->getDelegate()->MessageBoxButtonPressed(_messageBoxTitle, _buttonsReferenceList.at(buttonSelect));
        else
            this->getDelegate()->MessageBoxButtonPressed(_messageBoxTitle, _buttonsTitleList.at(buttonSelect));
    }
}
  
}
