#include "ElectricDreamsButton.h"
#include "ExitOrLogoutLayer.h"
#include "AudioMixer.h"
#include "AnalyticsSingleton.h"

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
    layer->addChild(layer->createSpriteButton("res/login/next_btn.png", NEXT_BUTTON_AUDIO_EFFECT ));
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createBackButton()
{
    auto layer = ElectricDreamsButton::create();
    layer->addChild(layer->createSpriteButton("res/login/back_btn.png", BACK_BUTTON_AUDIO_EFFECT ));
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createAcceptButton()
{
    auto layer = ElectricDreamsButton::create();
    Sprite* acceptButton =layer->createSpriteButton("res/modal/accept.png", ACCEPT_BUTTON_AUDIO_EFFECT );
    acceptButton->setOpacity(0);
    layer->addChild(acceptButton);
    acceptButton->runAction(FadeTo::create(0.5, 255));
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createCancelButton()
{
    auto layer = ElectricDreamsButton::create();
    Sprite* cancelButton = layer->createSpriteButton("res/modal/cancel.png", CANCEL_BUTTON_AUDIO_EFFECT );
    cancelButton->setOpacity(0);
    layer->addChild(cancelButton);
    cancelButton->runAction(FadeTo::create(0.5, 255));
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createSettingsButton(float creationDelay)
{
    auto layer = ElectricDreamsButton::create();
    Sprite* settingsButton = layer->createSpriteButton("res/navigation/settings.png", SETTINGS_BUTTON_AUDIO_EFFECT );
    settingsButton->setOpacity(0);
    layer->addChild(settingsButton);
    
    float randomDelay = RandomHelper::random_real(0.2, 0.7);
    settingsButton->runAction(Sequence::create(DelayTime::create(creationDelay + randomDelay), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
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
        AnalyticsSingleton::getInstance()->mixPanel_genericButtonPress(mixPanelButtonName);
}

Sprite* ElectricDreamsButton::createSpriteButton(std::string buttonImage, std::string buttonAudio)
{
    Sprite* spriteButton = Sprite::create(buttonImage);
    spriteButton->setPosition(spriteButton->getContentSize().width/2, spriteButton->getContentSize().height/2);
    
    setContentSize(spriteButton->getContentSize());
    buttonAudioFile = buttonAudio;
    
    return spriteButton;
}
