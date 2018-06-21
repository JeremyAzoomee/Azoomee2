#include "MessageBoxTextLayer.h"
#include "../../Strings.h"
#include "../../Analytics/AnalyticsSingleton.h"
#include "../ElectricDreamsTextStyles.h"
#include "../ElectricDreamsDecoration.h"

using namespace cocos2d;

#define MESSAGE_BOX_PADDING 100

namespace Azoomee
{
    
Layer* MessageBoxTextLayer::createMessageBoxTextLayer(std::string Title, std::string Body, std::vector<std::string> buttonTitleList,Layer* parentLayer)
{
    auto layer = MessageBoxTextLayer::create();
    
    layer->_buttonsTitleList = buttonTitleList;
    layer->_parentLayer = parentLayer;
    layer->initMessageBoxTextLayer(Title, Body, 0);
    
    return layer;
}

Layer* MessageBoxTextLayer::createMessageBoxTextLayer(long errorCode,Layer* parentLayer)
{
    std::map<std::string, std::string> errorStringMap = StringMgr::getInstance()->getErrorMessageWithCode(errorCode);
    
    auto layer = MessageBoxTextLayer::create();
    
    layer->_parentLayer = parentLayer;
    
    layer->_buttonsTitleList = splitStringToVector(errorStringMap[ERROR_BUTTON], "|");
    
    if(errorStringMap.at(ERROR_BUTTON_REFERENCE) != "")
        layer->_buttonsReferenceList = splitStringToVector(errorStringMap[ERROR_BUTTON_REFERENCE], "|");
    
    layer->initMessageBoxTextLayer(errorStringMap[ERROR_TITLE], errorStringMap[ERROR_BODY], errorCode);
    
    return layer;
}

void MessageBoxTextLayer::initMessageBoxTextLayer(std::string Title, std::string Body, long errorCode)
{
    AnalyticsSingleton::getInstance()->messageBoxShowEvent(Title,errorCode);
    
    _messageBoxTitle = Title;
    _messageBoxBody = Body;
    
    onSizeChanged();
}

bool MessageBoxTextLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    setCurrentRunningSceneSize();
    setPercentageofScreenForBox();
    
    return true;
}
    
void MessageBoxTextLayer::setPercentageofScreenForBox()
{
    if(currentRunningSceneSize.width < currentRunningSceneSize.height)
        percentageOfScreenForBox = 0.85;
    else
        percentageOfScreenForBox = 0.66;
}

//---------------------- Message Box Functions------------------------

void MessageBoxTextLayer::addButtonWithTitle(const std::string& buttonTitle)
{
    _buttonsTitleList.push_back(buttonTitle);
}
  
void MessageBoxTextLayer::createTitle()
{
    messageTitleLabel = createLabelMessageBoxTitle(_messageBoxTitle);
    messageTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    messageTitleLabel->setWidth(textMaxWidth);
}

void MessageBoxTextLayer::createBody()
{
    messageBodyLabel = createLabelMessageBoxBody(_messageBoxBody);
    messageBodyLabel->setWidth(textMaxWidth);
    
    const float maxHeight = currentRunningSceneSize.height * 0.5f;
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

void MessageBoxTextLayer::createButtons()
{
    buttonSpaceWidth = (currentRunningSceneSize.width*percentageOfScreenForBox / _buttonsTitleList.size());
    
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

void MessageBoxTextLayer::createCancelButton()
{
    cancelButton = ElectricDreamsButton::createWindowCloseButton();
    cancelButton->setMixPanelButtonName("messageBoxCancelButton");
    cancelButton->setDelegate(this);
}

void MessageBoxTextLayer::createMessageWindow()
{
    float buttonHeight = buttonsList.at(0)->getContentSize().height;
    if(currentRunningSceneSize.width < currentRunningSceneSize.height)
    {
        buttonHeight = (buttonsList.at(0)->getContentSize().height * buttonsList.size()) + (cancelButton->getContentSize().height * buttonsList.size() - 1);
    }
    
    float windowHeight = messageTitleLabel->getContentSize().height + scrollView->getContentSize().height + buttonHeight + cancelButton->getContentSize().height*4;
    
    windowLayer = createWindowLayer(currentRunningSceneSize.width * percentageOfScreenForBox, windowHeight);
    windowLayer->setPosition(currentRunningSceneSize.width/2- windowLayer->getContentSize().width/2,(currentRunningSceneSize.height - windowLayer->getContentSize().height) * 0.66);
    this->addChild(windowLayer);
}

void MessageBoxTextLayer::addObjectsToWindow()
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

void MessageBoxTextLayer::positionButtonsBasedOnWidth(float nextItemHeight)
{
    if(currentRunningSceneSize.width < currentRunningSceneSize.height)
    {
        for(int i=0;i < _buttonsTitleList.size(); i++)
        {
            buttonsList.at(i)->setCenterPosition(Vec2(windowLayer->getContentSize().width/2, nextItemHeight));
            windowLayer->addChild(buttonsList.at(i));
            nextItemHeight -= (buttonsList.at(0)->getContentSize().height + cancelButton->getContentSize().height);
        }
    }
    else
    {
        for(int i=0;i < _buttonsTitleList.size(); i++)
        {
            buttonsList.at(i)->setCenterPosition(Vec2(buttonSpaceWidth/2 + buttonSpaceWidth*i, nextItemHeight));
            windowLayer->addChild(buttonsList.at(i));
        }
    }
}

//---------------------- Actions -----------------
void MessageBoxTextLayer::addOrRemoveWires()
{
    removeWiresFromScreen(_parentLayer);
    
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    if(currentRunningScene->getContentSize().width > currentRunningScene->getContentSize().height)
        addSideWiresToScreen(_parentLayer);
}
    
void MessageBoxTextLayer::onSizeChanged()
{
    setCurrentRunningSceneSize();
    setPercentageofScreenForBox();
    
    if(windowLayer)
        windowLayer->removeAllChildren();
    
    buttonsList.clear();
    
    textMaxWidth = currentRunningSceneSize.width*percentageOfScreenForBox - MESSAGE_BOX_PADDING*2;

    createTitle();
    createBody();
    createButtons();
    createCancelButton();
    createMessageWindow();
    addObjectsToWindow();
    
    addOrRemoveWires();
}

//----------------------- Delegate Functions ----------------------------

void MessageBoxTextLayer::buttonPressed(ElectricDreamsButton* button)
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
  
void MessageBoxTextLayer::onCancelPressed()
{
     dynamic_cast<MessageBox*>(_parentLayer)->sendDelegateMessageBoxButtonPressed(_messageBoxTitle, MessageBox::kCancel);
}

void MessageBoxTextLayer::onButtonPressed(int buttonSelect)
{
    if(_buttonsReferenceList.size() >0)
        dynamic_cast<MessageBox*>(_parentLayer)->sendDelegateMessageBoxButtonPressed(_messageBoxTitle, _buttonsReferenceList.at(buttonSelect));
    else
        dynamic_cast<MessageBox*>(_parentLayer)->sendDelegateMessageBoxButtonPressed(_messageBoxTitle,_buttonsTitleList.at(buttonSelect));
}
  
}
