#include "ElectricDreamsButton.h"
#include "ExitOrLogoutLayer.h"
#include "AudioMixer.h"
#include "MixPanelCalls.h"

bool ElectricDreamsButton::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//---------------------- Create Button Types -----------------------------

ElectricDreamsButton* ElectricDreamsButton::createTextAsButton(std::string buttonText)
{
    auto layer = ElectricDreamsButton::create();
    
    Label* textButton = Label::createWithTTF(buttonText, "fonts/azoomee.ttf", 90);
    textButton->setColor(Color3B(28, 244, 244));
    textButton->setPosition(Vec2(textButton->getContentSize().width/2, textButton->getContentSize().height/2));
    textButton->setOpacity(0);
    textButton->setHorizontalAlignment(TextHAlignment::CENTER);
    layer->setContentSize(textButton->getContentSize());
    
    layer->addChild(textButton);
    
    layer->buttonAudioFile = TEXT_BUTTON_AUDIO_EFFECT;
    
    textButton->runAction(FadeTo::create(0.5, 255));
    
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createButtonWithText(std::string buttonText)
{
    auto layer = ElectricDreamsButton::create();
    
    ui::Scale9Sprite* newButton = layer->createButtonBackground(buttonText);

    layer->setContentSize(newButton->getContentSize());
    newButton->setOpacity(0);
    layer->addChild(newButton);
    
    newButton->runAction(FadeTo::create(0.5, 255));
    
    layer->buttonAudioFile = OK_BUTTON_AUDIO_EFFECT;
    
    layer->addListener();
    
    return layer;
}

ui::Scale9Sprite* ElectricDreamsButton::createButtonBackground(std::string buttonText)
{
    Label* buttonLabel = Label::createWithTTF(buttonText, "fonts/azoomee.ttf", 90);
    buttonLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    buttonLabel->setColor(Color3B::BLACK);
    
    Rect spriteRect = Rect(0, 0, 196, 197);
    Rect capInsents = Rect(98, 98, 1, 1);
    
    float buttonHeight = 197;
    
    if(buttonLabel->getContentSize().height >179)
        buttonHeight = buttonLabel->getContentSize().height + 100;
    
    ui::Scale9Sprite* newButton = ui::Scale9Sprite::create("res/modal/generic_button_slice_ready.png", spriteRect, capInsents);
    newButton->setContentSize(Size(buttonLabel->getContentSize().width+100, buttonHeight));
    newButton->setPosition(Vec2(newButton->getContentSize().width/2, newButton->getContentSize().height/2));
    
    buttonLabel->setPosition(newButton->getContentSize().width/2, newButton->getContentSize().height/2-5);
    
    buttonLabel->setOpacity(0);
    newButton->addChild(buttonLabel);
    
    buttonLabel->runAction(FadeTo::create(0.5, 255));
    
    return newButton;
}

ElectricDreamsButton* ElectricDreamsButton::createNextButton()
{
    auto layer = ElectricDreamsButton::create();
    
    Sprite* nextButton = Sprite::create("res/login/next_btn.png");
    nextButton->setPosition(nextButton->getContentSize().width/2, nextButton->getContentSize().height/2);
    
    layer->setContentSize(nextButton->getContentSize());
    
    layer->addChild(nextButton);
    
    layer->buttonAudioFile = NEXT_BUTTON_AUDIO_EFFECT;
    
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createBackButton()
{
    auto layer = ElectricDreamsButton::create();
    
    Sprite* backButton = Sprite::create("res/login/back_btn.png");
    backButton->setPosition(backButton->getContentSize().width/2, backButton->getContentSize().height/2);
    
    layer->setContentSize(backButton->getContentSize());
    
    layer->addChild(backButton);
    
    layer->buttonAudioFile = BACK_BUTTON_AUDIO_EFFECT;
    
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createAcceptButton()
{
    auto layer = ElectricDreamsButton::create();
    
    Sprite* acceptButton = Sprite::create("res/modal/accept.png");
    acceptButton->setPosition(acceptButton->getContentSize().width/2, acceptButton->getContentSize().height/2);
    acceptButton->setOpacity(0);
    layer->setContentSize(acceptButton->getContentSize());
    
    layer->addChild(acceptButton);
    
    acceptButton->runAction(FadeTo::create(0.5, 255));
    
    layer->buttonAudioFile = ACCEPT_BUTTON_AUDIO_EFFECT;
    
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createCancelButton()
{
    auto layer = ElectricDreamsButton::create();
    
    Sprite* cancelButton = Sprite::create("res/modal/cancel.png");
    cancelButton->setPosition(cancelButton->getContentSize().width/2, cancelButton->getContentSize().height/2);
    cancelButton->setOpacity(0);
    layer->setContentSize(cancelButton->getContentSize());
    
    layer->addChild(cancelButton);
    
    cancelButton->runAction(FadeTo::create(0.5, 255));
    
    layer->buttonAudioFile = CANCEL_BUTTON_AUDIO_EFFECT;
    
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createSettingsButton(float creationDelay)
{
    auto layer = ElectricDreamsButton::create();
    
    Sprite* settingsButton = Sprite::create("res/navigation/settings.png");
    settingsButton->setPosition(settingsButton->getContentSize().width/2, settingsButton->getContentSize().height/2);
    settingsButton->setOpacity(0);
    layer->setContentSize(settingsButton->getContentSize());
    
    layer->addChild(settingsButton);
    
    float randomDelay = RandomHelper::random_real(0.2, 0.7);
    settingsButton->runAction(Sequence::create(DelayTime::create(creationDelay + randomDelay), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    layer->buttonAudioFile = SETTINGS_BUTTON_AUDIO_EFFECT;
    layer->mixPanelButtonName = "Settings";
    
    layer->addListener();
    
    layer->isSettingsButton = true;
    
    return layer;
}

//---------------------- Listener Function -----------------------------

void ElectricDreamsButton::addListener()
{

    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Rect rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
        
        if(rect.containsPoint(locationInNode) && this->isVisible())
        {
            AudioMixer::getInstance()->playEffect(buttonAudioFile);
            sendMixPanelEvent();
            
            if(isSettingsButton)
                ExitOrLogoutLayer::create();
            else
                this->scheduleOnce(schedule_selector(ElectricDreamsButton::callDelegateFunction), 0.1);
            
            return true;
        }
        
        return false;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void ElectricDreamsButton::callDelegateFunction(float dt)
{
    this->getDelegate()->buttonPressed(this);
}

//---------------------- public Functions After Setup -----------------------------

void ElectricDreamsButton::setCenterPosition(Vec2 position)
{
    this->setPosition(Vec2(position.x - this->getContentSize().width/2, position.y - this->getContentSize().height/2));
}

Vec2 ElectricDreamsButton::getCenterPosition()
{
    return Vec2(this->getPositionX() + this->getContentSize().width/2, this->getPositionY() + this->getContentSize().height/2);
}

void ElectricDreamsButton::setMixPanelButtonName(std::string buttonName)
{
    mixPanelButtonName = buttonName;
}

//------------------------- private functions ----------------------

void ElectricDreamsButton::sendMixPanelEvent()
{
    if(mixPanelButtonName != "")
        mixPanel_genericButtonPress(mixPanelButtonName);
}
