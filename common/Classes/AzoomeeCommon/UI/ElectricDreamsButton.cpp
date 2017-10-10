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
    
    Label* textButton = Label::createWithTTF(buttonText, Style::Font::Regular, fontSize);
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
    
    Label* textButton = Label::createWithTTF(buttonText, Style::Font::Regular, fontSize);
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
    
    Label* textButton = Label::createWithTTF(buttonText, Style::Font::Regular, fontSize);
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
    
ElectricDreamsButton* ElectricDreamsButton::createGreenButtonWithWidth(std::string buttonText, float buttonWidth)
{
    return createPrimaryGreenButton(buttonText, 0, buttonWidth);
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

ElectricDreamsButton* ElectricDreamsButton::createPrimaryGreenButton(std::string buttonText, float textPadding, float minWidth)
{
    Label* buttonLabel = createLabelButtonAdultPrimary(buttonText);
    
    Rect spriteRect = Rect(0, 0, 572, 213);
    Rect capInsents = Rect(100, 106, 300, 1);
    
    float buttonHeight = 213;
    
    if(buttonLabel->getContentSize().height >179)
        buttonHeight = buttonLabel->getContentSize().height + 100;
    
    float buttonWidth = buttonLabel->getContentSize().width+150+textPadding;
    
    if(buttonWidth < minWidth)
        buttonWidth = minWidth;
    
    ui::Scale9Sprite* newButton = ui::Scale9Sprite::create("res/buttons/greenMainButton.png", spriteRect, capInsents);
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

ElectricDreamsButton* ElectricDreamsButton::createSecondaryButtonWithWidth(std::string buttonText, float buttonWidth)
{
    return createSecondaryButton(buttonText, 0, buttonWidth);
}
    
ElectricDreamsButton* ElectricDreamsButton::createSecondaryGreenButtonWithWidth(std::string buttonText, float buttonWidth)
{
    return createSecondaryGreenButton(buttonText, 0, buttonWidth);
}

ElectricDreamsButton* ElectricDreamsButton::createSecondaryButton(std::string buttonText, float textPadding, float minWidth)
{
    Label* buttonLabel = createLabelButtonAdultSecondary(buttonText);
    
    Rect spriteRect = Rect(0, 0, 206, 134);
    Rect capInsents = Rect(103, 67, 1, 1);
    
    float buttonHeight = 134;
    
    if(buttonLabel->getContentSize().height >110)
        buttonHeight = buttonLabel->getContentSize().height + 70;
    
    float buttonWidth = buttonLabel->getContentSize().width+150+textPadding;
    
    if(buttonWidth < minWidth)
        buttonWidth = minWidth;
    
    ui::Scale9Sprite* newButton = ui::Scale9Sprite::create("res/buttons/secondaryButton.png", spriteRect, capInsents);
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
    
    layer->buttonAudioFile = CANCEL_BUTTON_AUDIO_EFFECT;
    
    layer->addListener();
    
    return layer;
}
    
ElectricDreamsButton* ElectricDreamsButton::createSecondaryGreenButton(std::string buttonText, float textPadding, float minWidth)
{
    Label* buttonLabel = createLabelButtonAdultSecondary(buttonText);
    
    Rect spriteRect = Rect(0, 0, 572, 213);
    Rect capInsents = Rect(100, 106, 300, 1);
    
    float buttonHeight = 213;
    
    if(buttonLabel->getContentSize().height >179)
        buttonHeight = buttonLabel->getContentSize().height + 70;
    
    float buttonWidth = buttonLabel->getContentSize().width+150+textPadding;
    
    if(buttonWidth < minWidth)
        buttonWidth = minWidth;
    
    ui::Scale9Sprite* newButton = ui::Scale9Sprite::create("res/buttons/greenSecondatyButton.png", spriteRect, capInsents);
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
    
    layer->buttonAudioFile = CANCEL_BUTTON_AUDIO_EFFECT;
    
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createOutlineButtonWithText(std::string buttonText)
{
    auto layer = ElectricDreamsButton::create();
    
    ui::Scale9Sprite* newButton = layer->createButtonOutline(buttonText);
    
    layer->setCascadeOpacityEnabled(true);
    layer->setContentSize(newButton->getContentSize());
    newButton->setCascadeOpacityEnabled(true);
    layer->addChild(newButton);
    
    layer->buttonAudioFile = CANCEL_BUTTON_AUDIO_EFFECT;
    
    layer->addListener();
    
    return layer;
}

ui::Scale9Sprite* ElectricDreamsButton::createButtonOutline(std::string buttonText)
{
    Label* buttonLabel = createLabelBody(buttonText);
    
    Rect spriteRect = Rect(0, 0, 736, 131);
    Rect capInsents = Rect(60, 65, 616, 1);
    
    float buttonHeight = 131;
    
    if(buttonLabel->getContentSize().height >110)
        buttonHeight = buttonLabel->getContentSize().height + 80;
    
    ui::Scale9Sprite* newButton = ui::Scale9Sprite::create("res/buttons/buttonArea.png", spriteRect, capInsents);
    newButton->setContentSize(Size(buttonLabel->getContentSize().width+100, buttonHeight));
    newButton->setPosition(Vec2(newButton->getContentSize().width/2, newButton->getContentSize().height/2));
    
    buttonLabel->setPosition(newButton->getContentSize().width/2, newButton->getContentSize().height/2-5);
    
    newButton->addChild(buttonLabel);
    
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
    Sprite* acceptButton =layer->createSpriteButton("res/modal/acceptGreen.png", ACCEPT_BUTTON_AUDIO_EFFECT );
    acceptButton->setOpacity(0);
    layer->addChild(acceptButton);
    layer->setCascadeOpacityEnabled(true);
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

ElectricDreamsButton* ElectricDreamsButton::createWindowCloseButton()
{
    auto layer = ElectricDreamsButton::create();
    Sprite* cancelButton = layer->createSpriteButton("res/buttons/windowCloseButton.png", CANCEL_BUTTON_AUDIO_EFFECT );
    layer->addChild(cancelButton);
    layer->addListener();
    
    return layer;
}
    
ElectricDreamsButton* ElectricDreamsButton::createWindowCloseButtonGreen()
{
    auto layer = ElectricDreamsButton::create();
    Sprite* cancelButton = layer->createSpriteButton("res/buttons/windowCloseButtonGreen.png", CANCEL_BUTTON_AUDIO_EFFECT );
    layer->addChild(cancelButton);
    layer->addListener();
    
    return layer;
}
    
ElectricDreamsButton* ElectricDreamsButton::createWhiteWindowCloselButton()
{
    auto layer = ElectricDreamsButton::create();
    Sprite* cancelButton = layer->createSpriteButton("res/buttons/whiteWindowCloseButton.png", CANCEL_BUTTON_AUDIO_EFFECT );
    layer->addChild(cancelButton);
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createAddButton()
{
    auto layer = ElectricDreamsButton::create();
    layer->addChild(layer->createSpriteButton("res/childSelection/button_add_child.png", NEXT_BUTTON_AUDIO_EFFECT ));
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createPlaceHolderButton()
{
    auto layer = ElectricDreamsButton::create();
    layer->addChild(layer->createSpriteButton("res/modal/blankGreen.png", NEXT_BUTTON_AUDIO_EFFECT ));
    layer->setCascadeOpacityEnabled(true);
    layer->setOpacity(80);
    
    return layer;
}
    
ElectricDreamsButton* ElectricDreamsButton::createPlaceHolderButton(float withWidth)
{
    Rect spriteRect = Rect(0, 0, 196, 197);
    Rect capInsents = Rect(98, 98, 1, 1);
    
    float buttonHeight = 197;
    
    ui::Scale9Sprite* newButton = ui::Scale9Sprite::create("res/modal/generic_button_slice_ready.png", spriteRect, capInsents);
    newButton->setContentSize(Size(withWidth, buttonHeight));
    newButton->setPosition(Vec2(newButton->getContentSize().width/2, newButton->getContentSize().height/2));
    newButton->setCascadeOpacityEnabled(true);
    
    auto layer = ElectricDreamsButton::create();
    layer->setCascadeOpacityEnabled(true);
    layer->setContentSize(newButton->getContentSize());
    layer->setOpacity(80);
    layer->addChild(newButton);
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createChildSelectorButton()
{
    auto layer = ElectricDreamsButton::create();
    
    Sprite* spriteOuterButton = Sprite::create("res/navigation/outer_circle.png");
    spriteOuterButton->setPosition(spriteOuterButton->getContentSize().width/2, spriteOuterButton->getContentSize().height/2);
    spriteOuterButton->setColor(Color3B::WHITE);
    layer->addChild(spriteOuterButton);
    
    Sprite* spriteInnerButton = Sprite::create("res/navigation/menu6.png");
    spriteInnerButton->setPosition(spriteOuterButton->getContentSize().width/2, spriteOuterButton->getContentSize().height/2);
    spriteOuterButton->addChild(spriteInnerButton);
    
    layer->setContentSize(spriteOuterButton->getContentSize());
    layer->buttonAudioFile = BACK_BUTTON_AUDIO_EFFECT;
    layer->addListener();

    return layer;

}
    
ElectricDreamsButton* ElectricDreamsButton::createTabButton(std::string buttonText)
{
    auto layer = ElectricDreamsButton::create();
    layer->addChild(layer->createSpriteButton("res/settings/tab.png", OK_BUTTON_AUDIO_EFFECT ));
    
    Label* buttonLabel = createLabelButtonAdultSecondary(buttonText);
    buttonLabel->setColor(Style::Color::black);
    buttonLabel->setPosition(layer->getContentSize().width/2, layer->getContentSize().height/2);
    layer->addChild(buttonLabel);
    
    layer->addListener();

    return layer;
}
    
ElectricDreamsButton* ElectricDreamsButton::createChevronLeftButton()
{
    auto layer = ElectricDreamsButton::create();
    layer->addChild(layer->createSpriteButton("res/buttons/chevronLeft.png", BACK_BUTTON_AUDIO_EFFECT ));
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createChevronRightButton()
{
    auto layer = ElectricDreamsButton::create();
    layer->addChild(layer->createSpriteButton("res/buttons/chevronRight.png", NEXT_BUTTON_AUDIO_EFFECT));
    layer->addListener();
    
    return layer;
}

//----------- CHAT INVITE BUTTONS -------------------

ElectricDreamsButton* ElectricDreamsButton::createKidCodeShareButton(std::string kidCode, float buttonWidth)
{
    auto layer = ElectricDreamsButton::createSmallSprite9Button("", buttonWidth,"res/buttons/greenButton.png", Color3B::BLACK);
    
    Label* kidCodeLabel = createLabelWith(kidCode, Style::Font::kidCodeRegular, Color3B::BLACK, 64);
    
    float labelGap = 30;
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        Sprite* shareButton = Sprite::create("res/buttons/shareIconIOs.png");
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        Sprite* shareButton = Sprite::create("res/buttons/shareIconAndroid.png");
    #endif
    
    float widthOfItems = kidCodeLabel->getContentSize().width + shareButton->getContentSize().width + labelGap;
    
    kidCodeLabel->setPosition(layer->getContentSize().width/2 + kidCodeLabel->getContentSize().width/2 - (widthOfItems/2),layer->getContentSize().height/2 );
    shareButton->setPosition(kidCodeLabel->getPositionX()+kidCodeLabel->getContentSize().width/2+labelGap+shareButton->getContentSize().width/2,kidCodeLabel->getPositionY());
    
    layer->addChild(kidCodeLabel);
    layer->addChild(shareButton);
    
    
    layer->buttonAudioFile = OK_BUTTON_AUDIO_EFFECT;
    return layer;
    
}
    
ElectricDreamsButton* ElectricDreamsButton::createGreenButton(std::string buttonText, float buttonWidth)
{
    auto layer = ElectricDreamsButton::createSmallSprite9Button(buttonText, buttonWidth,"res/buttons/greenButton.png", Color3B::BLACK);
    layer->buttonAudioFile = OK_BUTTON_AUDIO_EFFECT;
    return layer;

}
ElectricDreamsButton* ElectricDreamsButton::createRedOutlineButton(std::string buttonText, float buttonWidth)
{
    auto layer = ElectricDreamsButton::createSmallSprite9Button(buttonText, buttonWidth,"res/buttons/inviteRejectionSecondaryButton.png", Color3B::BLACK);
    layer->buttonAudioFile = CANCEL_BUTTON_AUDIO_EFFECT;
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createRedFilledButton(std::string buttonText, float buttonWidth)
{
    auto layer = ElectricDreamsButton::createSmallSprite9Button(buttonText, buttonWidth,"res/buttons/inviteRejectionMainButton.png", Color3B::BLACK);
    layer->buttonAudioFile = ACCEPT_BUTTON_AUDIO_EFFECT;
    return layer;
}
    
ElectricDreamsButton* ElectricDreamsButton::createSmallSprite9Button(std::string buttonText, float buttonWidth,std::string sprite9, Color3B textColor)
{
    Label* buttonLabel = createLabelSettingsChat(buttonText,textColor);
    
    Rect spriteRect = Rect(0, 0, 184, 107);
    Rect capInsents = Rect(50, 53, 1, 1);
    
    
    ui::Scale9Sprite* newButton = ui::Scale9Sprite::create(sprite9, spriteRect, capInsents);
    newButton->setContentSize(Size(buttonWidth, 107));
    newButton->setPosition(Vec2(newButton->getContentSize().width/2, newButton->getContentSize().height/2));
    
    buttonLabel->setPosition(newButton->getContentSize().width/2, newButton->getContentSize().height/2-5);
    
    newButton->addChild(buttonLabel);
    newButton->setCascadeOpacityEnabled(true);
    
    auto layer = ElectricDreamsButton::create();
    layer->setCascadeOpacityEnabled(true);
    layer->setContentSize(newButton->getContentSize());
    layer->addChild(newButton);
    
    layer->buttonAudioFile = OK_BUTTON_AUDIO_EFFECT;
    
    layer->addListener();
    
    return layer;
}
    
ElectricDreamsButton* ElectricDreamsButton::createTextInputAsButton(std::string buttonText, float buttonWidth)
{
    Label* buttonLabel = createLabelSettingsChat(buttonText,Style::Color::black);
    
    Rect spriteRect = Rect(0, 0, 268, 107);
    Rect capInsents = Rect(100, 53, 1, 1);
    
    
    ui::Scale9Sprite* newButton = ui::Scale9Sprite::create("res/settings/textField.png", spriteRect, capInsents);
    newButton->setContentSize(Size(buttonWidth, 107));
    newButton->setPosition(Vec2(newButton->getContentSize().width/2, newButton->getContentSize().height/2));
    
    buttonLabel->setPosition(newButton->getContentSize().width/2, newButton->getContentSize().height/2-5);
    
    newButton->addChild(buttonLabel);
    newButton->setCascadeOpacityEnabled(true);
    
    auto layer = ElectricDreamsButton::create();
    layer->setCascadeOpacityEnabled(true);
    layer->setContentSize(newButton->getContentSize());
    layer->addChild(newButton);
    
    layer->buttonAudioFile = OK_BUTTON_AUDIO_EFFECT;
    
    layer->addListener();
    
    return layer;
    
}
    
ElectricDreamsButton* ElectricDreamsButton::createSendButton()
{
    auto layer = ElectricDreamsButton::create();
    layer->buttonEnabledImage = "res/settings/sendBtn.png";
    layer->buttonDisablednabledImage = "res/settings/sendBtnDisabled.png";
    layer->buttonEnabled = false;
    layer->addChild(layer->createSpriteButton(layer->buttonDisablednabledImage, NEXT_BUTTON_AUDIO_EFFECT ));
    layer->addListener();
    
    return layer;
}

//-------------OOMEE BUTTONS AND FUNCTIONS---------------------

ElectricDreamsButton* ElectricDreamsButton::createOomeeButtonWithOutline(int oomeeNumber, std::string oomeeName)
{
    auto layer = ElectricDreamsButton::create();
    layer->addChild(layer->createSpriteButton("res/buttons/rectangle2.png", "" ));
    layer->addListener();
    
    Sprite* glow = createGlow();
    glow->setPosition(layer->getContentSize().width/2, layer->getContentSize().height*.6);
    glow->setScale(.3);
    layer->addChild(glow);
    
    layer->oomeeLayer = OomeeButtonLayer::createOomeeLayer(oomeeNumber);
    layer->oomeeLayer->setPosition(layer->getContentSize().width/2, layer->getContentSize().height *.30);
    layer->addChild(layer->oomeeLayer);
    
    auto oomeeNameLabel = createLabelBody(oomeeName);
    oomeeNameLabel->setPosition(layer->getContentSize().width/2, layer->getContentSize().height *.15);
    layer->addChild(oomeeNameLabel);
    
    layer->setMixPanelButtonName(StringUtils::format("oomeeButton|%d",oomeeNumber));
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createOomeeAsButton(int oomeeNumber)
{
    auto layer = ElectricDreamsButton::create();
    layer->setContentSize(Size(300,300));
    layer->oomeeLayer = OomeeButtonLayer::createOomeeLayer(oomeeNumber);
    layer->oomeeLayer->setPosition(layer->getContentSize().width/2, 0);
    layer->addChild(layer->oomeeLayer);
    
    layer->setTag(oomeeNumber);
    
    layer->addListener();
    
    layer->setMixPanelButtonName(StringUtils::format("oomeeButton|%d",oomeeNumber));
    
    return layer;
}

void ElectricDreamsButton::playOomeeAnimation(std::string OomeeAnimation, bool loop)
{
    oomeeLayer->playAnimation(OomeeAnimation, loop);
}

void ElectricDreamsButton::playOomeeAnimationNoSound(std::string OomeeAnimation)
{
    oomeeLayer->playAnimationNoSound(OomeeAnimation);
}

void ElectricDreamsButton::hideOomee()
{
    oomeeLayer->hideOomee();
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
