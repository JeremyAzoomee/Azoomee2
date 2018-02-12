//
//  StartScreen.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 12/02/2018.
//

#include "StartScreen.h"
#include "BackEndCaller.h"
#include "SceneManagerScene.h"
#include "DynamicNodeHandler.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Scene* StartScreen::createScene()
{
    Scene* scene = Scene::createWithSize(Director::getInstance()->getVisibleSize());
    StartScreen* startScreen = StartScreen::create();
    startScreen->setContentSize(scene->getContentSize());
    scene->addChild(startScreen);
    
    return scene;
}

void StartScreen::onEnter()
{
    addBGElements();
    addImages();
    addText();
    addButtons();
}

bool StartScreen::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    DynamicNodeHandler::getInstance()->getCTAFiles();
    
    return true;
}

void StartScreen::addBGElements()
{
    const Size& contentSize = this->getContentSize();
    
    LayerColor* bgColour = LayerColor::create(Color4B::BLACK);
    this->addChild(bgColour);
    
    Sprite* bigBGCircles = Sprite::create("res/startScreen/outer_circles.png");
    bigBGCircles->setPosition(this->getPosition() + contentSize / 2.0f);
    bigBGCircles->runAction(RepeatForever::create(Sequence::create(FadeIn::create(2.0f), FadeOut::create(2.0f), NULL)));
    this->addChild(bigBGCircles);
    
    Sprite* innerCircle1 = Sprite::create("res/startScreen/inner_circle_centre_1.png");
    innerCircle1->setPosition(this->getPosition() + contentSize / 2.0f);
    this->addChild(innerCircle1);
    
    Sprite* innerCircle2 = Sprite::create("res/startScreen/inner_circle_centre_2.png");
    innerCircle2->setPosition(this->getPosition() + contentSize / 2.0f);
    innerCircle2->runAction(RepeatForever::create(RotateBy::create(15.0f, 360)));
    this->addChild(innerCircle2);
    
    Sprite* wiresLeft = Sprite::create("res/startScreen/wire_left.png");
    wiresLeft->setPosition(this->getPosition() + Vec2(0, contentSize.height / 2.0f));
    wiresLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    this->addChild(wiresLeft);
    
    Sprite* wiresRight = Sprite::create("res/startScreen/wire_right.png");
    wiresRight->setPosition(this->getPosition() + Vec2(contentSize.width, contentSize.height / 2.0f));
    wiresRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    this->addChild(wiresRight);
    
}

void StartScreen::addImages()
{
    const Size& contentSize = this->getContentSize();
    
    Sprite* topLeftThumb = createSpriteWithBorderRing("res/startScreen/game_webthumb_1_copy.png", "res/startScreen/inner_circle_1.png");
    topLeftThumb->setPosition( Vec2(contentSize.width * 0.087, contentSize.height * 0.615));
    this->addChild(topLeftThumb);
    
    Sprite* midLeftThumb = createSpriteWithBorderRing("res/startScreen/game_webthumb_6_copy_3.png", "res/startScreen/inner_circle_2.png");
    midLeftThumb->setPosition( Vec2(contentSize.width * 0.205, contentSize.height * 0.406));
    this->addChild(midLeftThumb);
    
    Sprite* bottomLeftThumb = createSpriteWithBorderRing("res/startScreen/game_webthumb_3.png", "res/startScreen/inner_circle_3.png");
    bottomLeftThumb->setPosition( Vec2(contentSize.width * 0.087, contentSize.height * 0.208));
    this->addChild(bottomLeftThumb);
    
    Sprite* topRightThumb = createSpriteWithBorderRing("res/startScreen/game_webthumb_4.png", "res/startScreen/inner_circle_1.png", -1);
    topRightThumb->setPosition( Vec2(contentSize.width * 0.913, contentSize.height * 0.615));
    this->addChild(topRightThumb);
    
    Sprite* midRightThumb = createSpriteWithBorderRing("res/startScreen/game_webthumb_5.png", "res/startScreen/inner_circle_2.png", -1);
    midRightThumb->setPosition( Vec2(contentSize.width * 0.795, contentSize.height * 0.406));
    this->addChild(midRightThumb);
    
    Sprite* bottomRightThumb = createSpriteWithBorderRing("res/startScreen/game_webthumb_6.png", "res/startScreen/inner_circle_3.png", -1);
    bottomRightThumb->setPosition( Vec2(contentSize.width * 0.913, contentSize.height * 0.208));
    this->addChild(bottomRightThumb);
    
    Sprite* logo = Sprite::create("res/startScreen/azoomee_white.png");
    logo->setPosition(Vec2(contentSize.width * 0.5f, contentSize.height * 0.814));
    this->addChild(logo);
}

void StartScreen::addText()
{
    const Size& contentSize = this->getContentSize();
    
    Label* topLabel = Label::createWithTTF("Already got an account?", Style::Font::Regular , 115);
    topLabel->setTextColor(Color4B(Style::Color::brightAqua));
    topLabel->setPosition(Vec2(contentSize.width * 0.5f, contentSize.height * 0.647f ));
    topLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    
    this->addChild(topLabel);
    
    Label* bottomLabel = Label::createWithTTF("or", Style::Font::Regular , 115);
    bottomLabel->setTextColor(Color4B(Style::Color::brightAqua));
    bottomLabel->setPosition(Vec2(contentSize.width * 0.5f, contentSize.height * 0.369f ));
    bottomLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    
    this->addChild(bottomLabel);
}

void StartScreen::addButtons()
{
    ui::Button* closeButton = ui::Button::create("res/startScreen/close.png");
    closeButton->setNormalizedPosition(Vec2(0.978f, 0.978f));
    closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    closeButton->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType eEventType)
    {
        if(eEventType == ui::Widget::TouchEventType::ENDED)
        {
            BackEndCaller::getInstance()->anonymousDeviceLogin();
        }
    });
    
    this->addChild(closeButton);
    
    ui::Button* loginButton = ui::Button::create("res/startScreen/rectangle_copy_3.png");
    loginButton->setNormalizedPosition(Vec2(0.5f, 0.5f));
    loginButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    loginButton->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType eEventType)
    {
        if(eEventType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(Login));
        }
    });
    
    Label* loginText = Label::createWithTTF("Log in", Style::Font::Regular, 83);
    loginText->setTextColor(Color4B::BLACK);
    loginText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    loginText->setNormalizedPosition(Vec2(0.5f, 0.5f));
    loginButton->addChild(loginText);
    
    this->addChild(loginButton);
    
    ui::Button* startTrialButton = ui::Button::create("res/startScreen/rectangle_copy_3.png");
    startTrialButton->setNormalizedPosition(Vec2(0.5f, 0.225f));
    startTrialButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    startTrialButton->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType eEventType)
    {
        if(eEventType == ui::Widget::TouchEventType::ENDED)
        {
            DynamicNodeHandler::getInstance()->startIAPFlow();
        }
    });
    
    Label* trialText = Label::createWithTTF("Start a trial", Style::Font::Regular, 83);
    trialText->setTextColor(Color4B::BLACK);
    trialText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    trialText->setNormalizedPosition(Vec2(0.5f, 0.5f));
    startTrialButton->addChild(trialText);
    
    this->addChild(startTrialButton);
}

Sprite* StartScreen::createSpriteWithBorderRing(const std::string &imageFilename, const std::string& borderFilename, int rotationDir)
{
    Sprite* image = Sprite::create(imageFilename);
    Sprite* frame = Sprite::create(borderFilename);
    frame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    image->addChild(frame);
    
    auto rotateAction = RotateBy::create(15.0f, rotationDir * 360.0f);
    frame->runAction(RepeatForever::create(rotateAction));
    
    return image;
}

NS_AZOOMEE_END
