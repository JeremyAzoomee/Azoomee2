#include "MessageBox.h"
#include "StringMgr.h"
#include "MixPanelSingleton.h"
#include "LoginScene.h"
#include "OnboardingScene.h"
#include "StringMgr.h"

#define MESSAGE_BOX_PADDING 100
#define MESSAGE_BOX_MINIMUM_WIDTH 1366
#define MESSAGE_BOX_MAXIMUM_WIDTH 2049

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
    layer->_buttonsTitleList.push_back(StringMgr::getInstance()->getStringForKey(BUTTON_CANCEL));
    layer->initMessageBoxLayer(StringMgr::getInstance()->getStringForKey(PREVIEW_MESSAGEBOX_TITLE_LABEL),StringMgr::getInstance()->getStringForKey(PREVIEW_MESSAGEBOX_BODY_LABEL),nullptr);
    
    return layer;
}

void MessageBox::initMessageBoxLayer(std::string Title, std::string Body, MessageBoxDelegate* _delegate)
{
    MixPanelSingleton::getInstance()->mixPanel_messageBoxShow(Title);
    
    if(_delegate)
        setDelegate(_delegate);

    _messageBoxTitle = Title;
    
    createAndFadeInLayer();
    createTitle();
    createBody(Body);
    createButtons();
    createMessageBackground();
    
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

void MessageBox::createAndFadeInLayer()
{
    backgroundLayer = LayerColor::create(Color4B(0,0,0,255),origin.x+ visibleSize.width, origin.y + visibleSize.height);
    
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
    messageTitleLabel = Label::createWithTTF(_messageBoxTitle, "fonts/azoomee.ttf", 120);
    
    if(messageTitleLabel->getContentSize().width < (MESSAGE_BOX_MINIMUM_WIDTH - MESSAGE_BOX_PADDING * 2))
    {
        messageBoxWidth = MESSAGE_BOX_MINIMUM_WIDTH;
        underlineTitle();
    }
    else if(messageTitleLabel->getContentSize().width < (MESSAGE_BOX_MAXIMUM_WIDTH - MESSAGE_BOX_PADDING * 2))
    {
        messageBoxWidth = messageTitleLabel->getContentSize().width + (MESSAGE_BOX_PADDING * 2);
        underlineTitle();
    }
    else
    {
        //Due to Title being more than 2 lines, currently not set to be underlined.
        messageBoxWidth = MESSAGE_BOX_MAXIMUM_WIDTH;
        messageTitleLabel->setWidth(MESSAGE_BOX_MAXIMUM_WIDTH - MESSAGE_BOX_PADDING * 2);
    }
    
    messageTitleLabel->setPosition(backgroundLayer->getContentSize().width * 0.5, backgroundLayer->getContentSize().height * 0.75);
    messageTitleLabel->setColor(Color3B(28, 244, 244));
    messageTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    backgroundLayer->addChild(messageTitleLabel,2);
}

void MessageBox::underlineTitle()
{
    DrawNode* newDrawNode = DrawNode::create();
    newDrawNode->drawRect(Vec2(0, 10), Vec2(messageTitleLabel->getContentSize().width, 14), Color4F((28 * 255), (244 * 255), (244 * 255), 1));
    messageTitleLabel->addChild(newDrawNode);
}

void MessageBox::createBody(std::string messageBody)
{
    messageBodyLabel = Label::createWithTTF(messageBody, "fonts/azoomee.ttf", 90);
    messageBodyLabel->setWidth(messageBoxWidth - MESSAGE_BOX_PADDING * 2);
    messageBodyLabel->setPosition(backgroundLayer->getContentSize().width * 0.5, messageTitleLabel->getPositionY() - (messageTitleLabel->getContentSize().height/2) - MESSAGE_BOX_PADDING - (messageBodyLabel->getContentSize().height/2));
    messageBodyLabel->setColor(Color3B::WHITE);
    messageBodyLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    backgroundLayer->addChild(messageBodyLabel,2);
}

void MessageBox::createButtons()
{
    float buttonsTotalWidth = 0;
    
    for(int i=0;i < _buttonsTitleList.size(); i++)
    {
        auto _button = ElectricDreamsButton::createButtonWithText(_buttonsTitleList.at(i));
        _button->setDelegate(this);
        backgroundLayer->addChild(_button, 2);
        
        buttonsList.push_back(_button);
        
        buttonsTotalWidth = buttonsTotalWidth + _button->getContentSize().width;
    }
    positionButtonsBasedOnWidth(buttonsTotalWidth);
}

void MessageBox::positionButtonsBasedOnWidth(float totalButtonsWidth)
{
    float totalWidth =totalButtonsWidth + ((_buttonsTitleList.size()+1) * MESSAGE_BOX_PADDING);
    
    if(totalWidth > MESSAGE_BOX_MAXIMUM_WIDTH)
        messageBoxWidth = MESSAGE_BOX_MAXIMUM_WIDTH;
    else if(totalWidth > messageBoxWidth)
        messageBoxWidth = totalWidth;
    
    float MessageBoxButtonSpace = messageBoxWidth/_buttonsTitleList.size();
    
    for(int i=0;i < _buttonsTitleList.size(); i++)
    {
        float buttonXValue = (backgroundLayer->getContentSize().width * 0.5) - (messageBoxWidth/2) + (MessageBoxButtonSpace * i) + (MessageBoxButtonSpace/2);
        
        buttonsList.at(i)->setCenterPosition(Vec2(buttonXValue, messageBodyLabel->getPositionY() - (messageBodyLabel->getContentSize().height/2) - MESSAGE_BOX_PADDING - (buttonsList.at(i)->getContentSize().height/2)));
    }
}

void MessageBox::createMessageBackground()
{
    float messageBoxHeight = messageTitleLabel->getContentSize().height + messageBodyLabel->getContentSize().height+buttonsList.at(0)->getContentSize().height + (4 * MESSAGE_BOX_PADDING);
    
    float messageBoxY = buttonsList.at(0)->getCenterPosition().y - (buttonsList.at(0)->getContentSize().height/2) - MESSAGE_BOX_PADDING;
    
    auto messageBoxLayer = LayerColor::create(Color4B::BLACK, messageBoxWidth, messageBoxHeight);
    messageBoxLayer->setPosition((visibleSize.width - messageBoxLayer->getContentSize().width)/2, messageBoxY);
    backgroundLayer->addChild(messageBoxLayer,1);
    
    DrawNode* newDrawNode = DrawNode::create();
    messageBoxLayer->addChild(newDrawNode);
    newDrawNode->drawRect(Vec2(0, 0), Vec2(messageBoxLayer->getContentSize().width, messageBoxLayer->getContentSize().height), Color4F((28 * 255), (244 * 255), (244 * 255), 1));
    
    newDrawNode->setLineWidth(4);
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
    else if(_buttonsTitleList.at(buttonSelect) == StringMgr::getInstance()->getStringForKey(BUTTON_CANCEL))
    {
        this->scheduleOnce(schedule_selector(MessageBox::removeSelf), 0.1);
        MixPanelSingleton::getInstance()->mixPanel_previewPopupCancelledEvent();
    }
}
