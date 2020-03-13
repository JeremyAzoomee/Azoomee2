#include "ElectricDreamsButton.h"
#include "../Audio/AudioMixer.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"
#include "Style.h"

using namespace cocos2d;
using namespace cocos2d::ui;


namespace Azoomee
{

bool ElectricDreamsButton::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    buttonEnabled = true;
    
    return true;
}

//---------------------- Create Button Types -----------------------------

ElectricDreamsButton* ElectricDreamsButton::createTextAsButton(std::string buttonText)
{
    auto layer = ElectricDreamsButton::create();
    
    Label* textButton = createLabelHeader(buttonText);
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

ElectricDreamsButton* ElectricDreamsButton::createTextAsButton(std::string buttonText, float fontSize, bool underlined)
{    
    auto layer = ElectricDreamsButton::create();
    
    Label* textButton = Label::createWithTTF(buttonText, Style::Font::Regular(), fontSize);
    textButton->setPosition(Vec2(textButton->getContentSize().width/2, textButton->getContentSize().height/2));
    textButton->setHorizontalAlignment(TextHAlignment::CENTER);
    layer->setContentSize(textButton->getContentSize());
    
    layer->addChild(textButton);
    
    layer->buttonAudioFile = CANCEL_BUTTON_AUDIO_EFFECT;
    layer->addListener();
    
    if(underlined)
    {
        DrawNode* newDrawNode = DrawNode::create();
        newDrawNode->drawRect(Vec2(0, -7), Vec2(textButton->getContentSize().width, -6), Color4F::WHITE);
        layer->addChild(newDrawNode);
    }
    
    return layer;
}
    
ElectricDreamsButton* ElectricDreamsButton::createTextAsButtonAqua(std::string buttonText, float fontSize, bool underlined)
{
    auto layer = ElectricDreamsButton::create();
    
    Label* textButton = Label::createWithTTF(buttonText, Style::Font::Regular(), fontSize);
    textButton->setPosition(Vec2(textButton->getContentSize().width/2, textButton->getContentSize().height/2));
    textButton->setHorizontalAlignment(TextHAlignment::CENTER);
    textButton->setColor(Style::Color::brightAqua);
    layer->setContentSize(textButton->getContentSize());
    
    layer->addChild(textButton);
    
    layer->buttonAudioFile = CANCEL_BUTTON_AUDIO_EFFECT;
    layer->addListener();
    
    if(underlined)
    {
        DrawNode* newDrawNode = DrawNode::create();
        newDrawNode->setLineWidth(2);
        newDrawNode->drawLine(Vec2(0, 0), Vec2(textButton->getContentSize().width,0), Style::Color_4F::brightAqua);
        layer->addChild(newDrawNode);
    }
    
    return layer;
}
    
ElectricDreamsButton* ElectricDreamsButton::createTextAsButtonWithColor(std::string buttonText, float fontSize, bool underlined, Color3B color)
{
    auto layer = ElectricDreamsButton::create();
    
    Label* textButton = Label::createWithTTF(buttonText, Style::Font::Regular(), fontSize);
    textButton->setPosition(Vec2(textButton->getContentSize().width/2, textButton->getContentSize().height/2));
    textButton->setHorizontalAlignment(TextHAlignment::CENTER);
    textButton->setColor(color);
    layer->setContentSize(textButton->getContentSize());
    
    layer->addChild(textButton);
    
    layer->buttonAudioFile = CANCEL_BUTTON_AUDIO_EFFECT;
    layer->addListener();
    
    if(underlined)
    {
        DrawNode* newDrawNode = DrawNode::create();
        newDrawNode->setLineWidth(2);
        newDrawNode->drawLine(Vec2(0, 0), Vec2(textButton->getContentSize().width,0), Color4F(color.r/255.0f,color.g/255.0f, color.b/255.0f,255/255.0f));
        layer->addChild(newDrawNode);
    }
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createButtonWithText(std::string buttonText, float textPadding)
{
    return createPrimaryButton(buttonText, textPadding, 0);
}

ElectricDreamsButton* ElectricDreamsButton::createButtonWithText(std::string buttonText)
{
    return createPrimaryButton(buttonText, 0, 0);
}

ElectricDreamsButton* ElectricDreamsButton::createButtonWithWidth(std::string buttonText, float buttonWidth)
{
    return createPrimaryButton(buttonText, 0, buttonWidth);
}
    

ElectricDreamsButton* ElectricDreamsButton::createPrimaryButton(std::string buttonText, float textPadding, float minWidth)
{
    Label* buttonLabel = createLabelButtonAdultPrimary(buttonText);
    
    Rect spriteRect = Rect(0, 0, 196, 197);
    Rect capInsents = Rect(98, 98, 1, 1);
    
    float buttonHeight = 197;
    
    if(buttonLabel->getContentSize().height >179)
        buttonHeight = buttonLabel->getContentSize().height + 100;
    
    float buttonWidth = buttonLabel->getContentSize().width+150+textPadding;
                             
    if(buttonWidth < minWidth)
        buttonWidth = minWidth;
    
    ui::Scale9Sprite* newButton = ui::Scale9Sprite::create("res/modal/generic_button_slice_ready.png", spriteRect, capInsents);
    newButton->setContentSize(Size(buttonWidth, buttonHeight));
    newButton->setPosition(Vec2(newButton->getContentSize().width/2, newButton->getContentSize().height/2));
    
    buttonLabel->setPosition(newButton->getContentSize().width/2, newButton->getContentSize().height/2-5);
    
    newButton->addChild(buttonLabel);
    newButton->setCascadeOpacityEnabled(true);
    
    auto layer = ElectricDreamsButton::create();
    layer->setCascadeOpacityEnabled(true);
    layer->setContentSize(newButton->getContentSize());
    layer->setOpacity(0);
    layer->addChild(newButton);
    
    layer->runAction(FadeTo::create(0.5, 255));
    
    layer->buttonAudioFile = OK_BUTTON_AUDIO_EFFECT;
    
    layer->addListener();
    
    return layer;
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

ElectricDreamsButton* ElectricDreamsButton::createWindowCloseButton()
{
    auto layer = ElectricDreamsButton::create();
    Sprite* cancelButton = layer->createSpriteButton("res/buttons/windowCloseButton.png", CANCEL_BUTTON_AUDIO_EFFECT );
    layer->addChild(cancelButton);
    layer->addListener();
    
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
        
        if(rect.containsPoint(locationInNode) && this->isVisible() && buttonEnabled)
        {
            this->scheduleOnce(CC_SCHEDULE_SELECTOR(ElectricDreamsButton::callDelegateFunction), 0.1);
            return true;
        }
        
        return false;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void ElectricDreamsButton::callDelegateFunction(float dt)
{
    AudioMixer::getInstance()->playEffect(buttonAudioFile);
    sendMixPanelEvent();
    onButtonPressed();
}
  
void ElectricDreamsButton::onButtonPressed()
{
    auto del = this->getDelegate();
    if(del != nullptr)
    {
        del->buttonPressed(this);
    }
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
    
void ElectricDreamsButton::setEnabled(bool isEnabled)
{
    buttonEnabled = isEnabled;
    
    if(isEnabled)
        spriteButton->setTexture(buttonEnabledImage);
    else
        spriteButton->setTexture(buttonDisablednabledImage);
}

//------------------------- private functions ----------------------

void ElectricDreamsButton::sendMixPanelEvent()
{
    if(mixPanelButtonName != "")
        AnalyticsSingleton::getInstance()->genericButtonPressEvent(mixPanelButtonName);
}

Sprite* ElectricDreamsButton::createSpriteButton(std::string buttonImage, std::string buttonAudio)
{
    spriteButton = Sprite::create(buttonImage);
    spriteButton->setPosition(spriteButton->getContentSize().width/2, spriteButton->getContentSize().height/2);
    
    setContentSize(spriteButton->getContentSize());
    buttonAudioFile = buttonAudio;
    
    return spriteButton;
}
  
}
