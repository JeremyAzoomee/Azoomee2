#include "MessageBoxSignLoginLayer.h"
#include "../../Strings.h"
#include "../../Analytics/AnalyticsSingleton.h"
#include "../ElectricDreamsTextStyles.h"
#include "../ElectricDreamsDecoration.h"

using namespace cocos2d;

#define MESSAGE_BOX_PADDING 100

namespace Azoomee
{
    
Layer* MessageBoxSignLoginLayer::createSignLoginLayer(Layer* parentLayer)
{
    auto layer = MessageBoxSignLoginLayer::create();
    
    layer->_parentLayer = parentLayer;
    layer->_messageBoxTitle = StringMgr::getInstance()->getStringForKey(PREVIEW_MESSAGEBOX_TITLE_LABEL);
    layer->_messageBoxBody = StringMgr::getInstance()->getStringForKey(PREVIEW_MESSAGEBOX_BODY_LABEL);
    layer->addButtonWithTitle(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_IN));
    layer->addButtonWithTitle(StringMgr::getInstance()->getStringForKey(BUTTON_SIGN_UP));

    AnalyticsSingleton::getInstance()->messageBoxShowEvent(layer->_messageBoxTitle,0);
    
    return layer;
}

bool MessageBoxSignLoginLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    setCurrentRunningSceneSize();
    setPercentageofScreenForBox();
    
    return true;
}
    
void MessageBoxSignLoginLayer::setPercentageofScreenForBox()
{
    if(currentRunningSceneSize.width < currentRunningSceneSize.height)
        percentageOfScreenForBox = 0.85;
    else
        percentageOfScreenForBox = 0.66;
}

//---------------------- Message Box Functions------------------------

void MessageBoxSignLoginLayer::addButtonWithTitle(const std::string& buttonTitle)
{
    _buttonsTitleList.push_back(buttonTitle);
}
  
void MessageBoxSignLoginLayer::createTitle()
{
    messageTitleLabel = createLabelMessageBoxTitle(_messageBoxTitle);
    messageTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    messageTitleLabel->setWidth(textMaxWidth);
}

void MessageBoxSignLoginLayer::createBody()
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

void MessageBoxSignLoginLayer::createButtons()
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

void MessageBoxSignLoginLayer::createCancelButton()
{
    cancelButton = ElectricDreamsButton::createWindowCloselButton();
    cancelButton->setMixPanelButtonName("messageBoxCancelButton");
    cancelButton->setDelegate(this);
}

void MessageBoxSignLoginLayer::createMessageWindow()
{
    float windowHeight = messageTitleLabel->getContentSize().height + scrollView->getContentSize().height + buttonsList.at(0)->getContentSize().height + cancelButton->getContentSize().height*4;
    
    windowLayer = createWindowLayer(currentRunningSceneSize.width * percentageOfScreenForBox, windowHeight);
    windowLayer->setPosition(currentRunningSceneSize.width/2- windowLayer->getContentSize().width/2,(currentRunningSceneSize.height - windowLayer->getContentSize().height) * 0.66);
    this->addChild(windowLayer);
}

void MessageBoxSignLoginLayer::addObjectsToWindow()
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

void MessageBoxSignLoginLayer::positionButtonsBasedOnWidth(float nextItemHeight)
{
    for(int i=0;i < _buttonsTitleList.size(); i++)
    {
        buttonsList.at(i)->setCenterPosition(Vec2(buttonSpaceWidth/2 + buttonSpaceWidth*i, nextItemHeight));
        windowLayer->addChild(buttonsList.at(i));
    }
}

//---------------------- Actions -----------------
void MessageBoxSignLoginLayer::addOrRemoveWires()
{
    removeWiresFromScreen(_parentLayer);
    
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    if(currentRunningScene->getContentSize().width > currentRunningScene->getContentSize().height)
        addSideWiresToScreen(_parentLayer);
}
    
void MessageBoxSignLoginLayer::onSizeChanged()
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

void MessageBoxSignLoginLayer::buttonPressed(ElectricDreamsButton* button)
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
  
void MessageBoxSignLoginLayer::onCancelPressed()
{
    AnalyticsSingleton::getInstance()->previewPopupCancelledEvent();
     dynamic_cast<MessageBox*>(_parentLayer)->sendDelegateMessageBoxButtonPressed(_messageBoxTitle, "Cancel");
}

void MessageBoxSignLoginLayer::onButtonPressed(int buttonSelect)
{
    /*if( buttonSelect == 0 ) // log-in
        LoginLogicHandler::getInstance()->forceNewLogin();
    else if( buttonSelect == 1 ) // signup
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Onboarding));*/
}
  
}
