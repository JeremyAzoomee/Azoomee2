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
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Utils/ActionBuilder.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string StartScreen::kAssetLoc = "res/startScreen/";

Scene* StartScreen::createScene()
{
    Scene* scene = Scene::createWithSize(Director::getInstance()->getVisibleSize());
    StartScreen* startScreen = StartScreen::create();
    startScreen->setContentSize(scene->getContentSize());
    startScreen->setPosition(Director::getInstance()->getVisibleOrigin());
    scene->addChild(startScreen);
    
    return scene;
}

void StartScreen::onEnter()
{
    addBGElements();
    addImages();
    addButtons();
    
    Super::onEnter();
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
    
    Sprite* bigBGCircles = Sprite::create(kAssetLoc + "outer_circles.png");
    bigBGCircles->setPosition(contentSize / 2.0f);
    bigBGCircles->runAction(RepeatForever::create( createFadeInAndOutEffect(3.0f, 75, 255)));
    this->addChild(bigBGCircles);
    Sprite* innerCircle1 = Sprite::create("res/mainhub/circle_0.png");
    innerCircle1->setPosition(contentSize / 2.0f);
    this->addChild(innerCircle1);
    
    Sprite* innerCircle2 = Sprite::create("res/mainhub/circle_1.png");
    innerCircle2->setPosition(contentSize / 2.0f);
    this->addChild(innerCircle2);
    innerCircle2->runAction(RepeatForever::create(RotateBy::create(15.0f, 360)));
    
    Sprite* wiresLeft = Sprite::create("res/decoration/wireLeft.png");
    wiresLeft->setPosition(Vec2(0, contentSize.height / 2.0f));
    wiresLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    wiresLeft->runAction(RepeatForever::create( Sequence::create( DelayTime::create(0.5f), createFlickerEffect(1, 6.5f), nullptr)));
    this->addChild(wiresLeft);
    
    Sprite* wiresRight = Sprite::create("res/decoration/wireRight.png");
    wiresRight->setPosition(Vec2(contentSize.width, contentSize.height / 2.0f));
    wiresRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    wiresRight->runAction(RepeatForever::create( createFlickerEffect(1, 7.0f)));
    this->addChild(wiresRight);
    
}

void StartScreen::addImages()
{
    const Size& contentSize = this->getContentSize();
    
    Sprite* topLeftThumb = createSpriteWithBorderRing(kAssetLoc + "game_webthumb_1_copy.png", kAssetLoc + "inner_circle_1.png");
    topLeftThumb->setPosition( Vec2(contentSize.width * 0.087, contentSize.height * 0.615));
    this->addChild(topLeftThumb);
    
    Sprite* midLeftThumb = createSpriteWithBorderRing(kAssetLoc + "game_webthumb_6_copy_3.png", kAssetLoc + "inner_circle_2.png");
    midLeftThumb->setPosition( Vec2(contentSize.width * 0.205, contentSize.height * 0.406));
    this->addChild(midLeftThumb);
    
    Sprite* bottomLeftThumb = createSpriteWithBorderRing(kAssetLoc + "game_webthumb_3.png", kAssetLoc + "inner_circle_3.png");
    bottomLeftThumb->setPosition( Vec2(contentSize.width * 0.087, contentSize.height * 0.208));
    this->addChild(bottomLeftThumb);
    
    Sprite* topRightThumb = createSpriteWithBorderRing(kAssetLoc + "game_webthumb_4.png", kAssetLoc + "inner_circle_1.png", -1);
    topRightThumb->setPosition( Vec2(contentSize.width * 0.913, contentSize.height * 0.615));
    this->addChild(topRightThumb);
    
    Sprite* midRightThumb = createSpriteWithBorderRing(kAssetLoc + "game_webthumb_5.png", kAssetLoc + "inner_circle_2.png", -1);
    midRightThumb->setPosition( Vec2(contentSize.width * 0.795, contentSize.height * 0.406));
    this->addChild(midRightThumb);
    
    Sprite* bottomRightThumb = createSpriteWithBorderRing(kAssetLoc + "game_webthumb_6.png", kAssetLoc + "inner_circle_3.png", -1);
    bottomRightThumb->setPosition( Vec2(contentSize.width * 0.913, contentSize.height * 0.208));
    this->addChild(bottomRightThumb);
    
    Sprite* logo = Sprite::create(kAssetLoc + "azoomee_white.png");
    logo->setPosition(Vec2(contentSize.width * 0.5f, contentSize.height * 0.814));
    this->addChild(logo);
}

void StartScreen::addButtons()
{
    const float posMod = ConfigStorage::getInstance()->isDeviceIphoneX() ? 1.1f : 1.5f;
    
    ui::Button* closeButton = ui::Button::create("res/buttons/windowCloseButtonGreen.png");
    closeButton->setScale(1.5f);
    closeButton->setNormalizedPosition(Vec2(0.978f, 0.978f));
    closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    closeButton->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType eEventType)
    {
        if(eEventType == ui::Widget::TouchEventType::ENDED)
        {
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("startScreen|close");
            BackEndCaller::getInstance()->anonymousDeviceLogin();
        }
    });
    
    this->addChild(closeButton);
    
    ui::Button* loginButton = ui::Button::create(kAssetLoc + "rectangle_copy_3.png");
    loginButton->setNormalizedPosition(Vec2(0.5f, 0.5f));
    loginButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    loginButton->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType eEventType)
    {
        if(eEventType == ui::Widget::TouchEventType::ENDED)
        {
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("startScreen|login");
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(Login));
        }
    });
    
    Label* loginText = Label::createWithTTF("Log in", Style::Font::Regular, 83);
    loginText->setTextColor(Color4B::BLACK);
    loginText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    loginText->setNormalizedPosition(Vec2(0.5f, 0.5f));
    loginButton->addChild(loginText);
    
    Label* topLabel = Label::createWithTTF("Already got an account?", Style::Font::Regular , 115);
    topLabel->setTextColor(Color4B(Style::Color::brightAqua));
    topLabel->setPosition(Vec2(loginButton->getContentSize().width * 0.5f, loginButton->getContentSize().height * posMod ));
    topLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    
    loginButton->addChild(topLabel);
    
    this->addChild(loginButton);
    
    ui::Button* startTrialButton = ui::Button::create(kAssetLoc + "rectangle_copy_3.png");
    startTrialButton->setNormalizedPosition(Vec2(0.5f, 0.225f));
    startTrialButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    startTrialButton->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType eEventType)
    {
        if(eEventType == ui::Widget::TouchEventType::ENDED)
        {
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("startScreen|startTrial");
            DynamicNodeHandler::getInstance()->startIAPFlow();
        }
    });
    
    Label* trialText = Label::createWithTTF("Start a trial", Style::Font::Regular, 83);
    trialText->setTextColor(Color4B::BLACK);
    trialText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    trialText->setNormalizedPosition(Vec2(0.5f, 0.5f));
    startTrialButton->addChild(trialText);
    
    Label* bottomLabel = Label::createWithTTF("or", Style::Font::Regular , 115);
    bottomLabel->setTextColor(Color4B(Style::Color::brightAqua));
    bottomLabel->setPosition(Vec2(startTrialButton->getContentSize().width * 0.5f, startTrialButton->getContentSize().height * posMod ));
    bottomLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    
    startTrialButton->addChild(bottomLabel);
    
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
