//
//  ChildOnboardingScene.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 25/11/2019.
//

#include "ChildOnboardingScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "SceneManagerScene.h"
#include "BackEndCaller.h"
#include "LoginController.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/ErrorCodes.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "PopupMessageBox.h"
#include "FlowDataSingleton.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChildOnboardingScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    const Color3B& bgColour = Style::Color::darkIndigo;
    
    _bgColour = ui::Layout::create();
    _bgColour->setBackGroundColorType(ui::HBox::BackGroundColorType::SOLID);
    _bgColour->setBackGroundColor(bgColour);
    _bgColour->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bgColour->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgColour->setSizeType(ui::Layout::SizeType::PERCENT);
    _bgColour->setSizePercent(Vec2(1.0,1.0));
    addChild(_bgColour);
    
    _bgPattern = RoundedRectSprite::create();
    _bgPattern->setTexture("res/decoration/pattern_general_tile.png");
    _bgPattern->setTileScaleFactor(2.0f);
    _bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setRoundedCorners(false, false, false, false);
    _bgPattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    _bgPattern->setOpacity(153);
    addChild(_bgPattern);
    
    _patternHider = DrawNode::create();
    addChild(_patternHider);
    
    _gradient = LayerGradient::create( Color4B(bgColour.r, bgColour.g, bgColour.b, 255), Color4B(bgColour.r, bgColour.g, bgColour.b, 0), Vec2(0.0f, 1.0f));
    _gradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _gradient->setIgnoreAnchorPointForPosition(false);
    addChild(_gradient);
    
    _childCreator = ChildCreator::create();
    _childCreator->setHttpRespnseDelegate(this);
    
    _nameEntry = ChildNameEntry::create();
    _nameEntry->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
    _nameEntry->setVisible(false);
    _nameEntry->setContinueCallback([this](const std::string& inputString){
        _childCreator->setChildName(inputString);
        this->transitionState(State::ENTER_AGE);
    });
    _nameEntry->setBackCallback([this](const std::string& inputString){
        if(UserAccountManager::getInstance()->isLoggedInParentAnonymous())
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::WelcomeScene));
        }
        else
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::SettingsFromChildSelect));
        }
    });
    addChild(_nameEntry);
    
    _ageEntry = ChildAgeEntry::create();
    _ageEntry->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
    _ageEntry->setVisible(false);
    _ageEntry->setContinueCallback([this](int age){
        _childCreator->setAge(age);
        ModalMessages::getInstance()->startLoading();
        if(UserAccountManager::getInstance()->isLoggedInParentAnonymous())
        {
            _childCreator->updateChild(UserAccountManager::getInstance()->getChild(0));
        }
        else
        {
            _childCreator->addChild();
        }
    });
    _ageEntry->setBackCallback([this](int age){
        _childCreator->setAge(age);
        this->transitionState(State::ENTER_NAME);
    });
    addChild(_ageEntry);
    
    _oomee = ui::ImageView::create("res/childOnboarding/oomee_torso.png");
    _oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _oomee->setNormalizedPosition(Vec2(0.25f, 0.0f));
    addChild(_oomee);
    
    _oomeeFace = ui::ImageView::create("res/childOnboarding/oomee_face_smile.png");
    _oomeeFace->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _oomeeFace->setNormalizedPosition(Vec2(0.5f, 0.425f));
    _oomee->addChild(_oomeeFace);
    
    _oomeeArms = ui::ImageView::create("res/childOnboarding/oomee_hands.png");
    _oomeeArms->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _oomeeArms->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _oomee->addChild(_oomeeArms);
    
    _speechPoint = ui::ImageView::create("res/childOnboarding/speech_point.png");
    _speechPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _speechPoint->setNormalizedPosition(Vec2(0.53f, 0.2f));
    addChild(_speechPoint);
    
    return true;
}

void ChildOnboardingScene::onEnter()
{
    transitionState(_state);
    Super::onEnter();
}

void ChildOnboardingScene::onExit()
{
    Super::onExit();
}

void ChildOnboardingScene::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = this->getContentSize();
    bool isPortrait = contentSize.height > contentSize.width;
    
    _patternHider->clear();
    if(isPortrait)
    {
        Vec2 points[4] = {Vec2(0,-1), Vec2(contentSize.width, -1), Vec2(contentSize.width, contentSize.height * 0.66f), Vec2(0,contentSize.height * 0.66f)};
        _patternHider->drawSolidPoly(points, 4, Color4F(Style::Color::darkIndigo));
    }
    else
    {
        Vec2 points[3] = {Vec2(0, 0), Vec2(contentSize.width, contentSize.height), Vec2(contentSize.width,0)};
        _patternHider->drawSolidPoly(points, 3, Color4F(Style::Color::darkIndigo));
    }
    
    _gradient->setStartOpacity(isPortrait ? 166 : 255);
    _gradient->setNormalizedPosition(isPortrait ? Vec2(0.5f,0.66f) : Vec2::ANCHOR_MIDDLE);
    _gradient->setContentSize(isPortrait ? Size(contentSize.width, contentSize.height * 0.34f) : Size(Vec2(contentSize).distance(Vec2(0,0)),Vec2(contentSize).distance(Vec2(0,0)) / 2.0f));
    _gradient->setRotation(CC_RADIANS_TO_DEGREES(isPortrait ? 0 : -Vec2(contentSize).getAngle()));
    
    _bgPattern->setNormalizedPosition(isPortrait ? Vec2(0.5f,0.66f) : Vec2::ANCHOR_MIDDLE_LEFT);
    _bgPattern->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
    _bgPattern->setContentSize(isPortrait ? Size(contentSize.width, contentSize.height * 0.34f) : Size(contentSize.width * 0.55f, contentSize.height));
    
    _oomee->setNormalizedPosition(isPortrait ? Vec2(0.5f, 0.69f) : Vec2(0.25f, 0.0f));
    _oomeeArms->loadTexture(isPortrait ? "res/childOnboarding/oomee_hands.png" : "res/childOnboarding/oomee_hands_crossed.png");
    _oomeeArms->setAnchorPoint(isPortrait ? Vec2(0.5f, 0.66f) : Vec2::ANCHOR_MIDDLE_BOTTOM);
    _oomee->setScale(isPortrait ? 0.75f : 1.0f);
    _speechPoint->setVisible(!isPortrait);
    
    _nameEntry->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
    _nameEntry->setPosition(isPortrait ? Vec2(contentSize.width * 0.5,0.0) : Vec2(contentSize.width * 0.5f, contentSize.height * 0.5f));
    _nameEntry->setSizePercent(isPortrait ? Vec2(1.0f,0.72f) : Vec2(0.5f, 0.95f));
    
    _ageEntry->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
    _ageEntry->setPosition(isPortrait ? Vec2(contentSize.width * 0.5,0.0) : Vec2(contentSize.width * 0.5f, contentSize.height * 0.5f));
    _ageEntry->setSizePercent(isPortrait ? Vec2(1.0f,0.72f) : Vec2(0.5f, 0.95f));
    
}

void ChildOnboardingScene::transitionState(const State &newState)
{
    _state = newState;
    
    _nameEntry->setVisible(_state == State::ENTER_NAME);
    _ageEntry->setVisible(_state == State::ENTER_AGE);
    
    _nameEntry->setChildName(_childCreator->getName());
    _ageEntry->setChildName(_childCreator->getName());
    _ageEntry->setSelectedAge(_childCreator->getAge());
    
    _oomeeFace->loadTexture(_state == State::ENTER_AGE ? "res/childOnboarding/oomee_face_excited.png" : "res/childOnboarding/oomee_face_smile.png");
}

void ChildOnboardingScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{

    if(requestTag == API::TagUpdateChild)
    {
        AnalyticsSingleton::getInstance()->childProfileCreatedSuccessEvent();
        UserDefault::getInstance()->setBoolForKey("anonOnboardingComplete", true);
        UserDefault::getInstance()->flush();
        UserAccountManager::getInstance()->getChildrenForLoggedInParent([](bool success, long errorcode){
            if(success)
            {
                LoginController::getInstance()->handleGetChildrenSuccess();
            }
            else
            {
                LoginController::getInstance()->forceNewLogin();
            }
        });
    }
    else if(requestTag == API::TagRegisterChild)
    {
        AnalyticsSingleton::getInstance()->childProfileCreatedSuccessEvent();
        UserAccountManager::getInstance()->getChildrenForLoggedInParent([](bool success, long errorcode){
            if(success)
            {
                LoginController::getInstance()->handleGetChildrenSuccess();
            }
            else
            {
                LoginController::getInstance()->forceNewLogin();
            }
        });
    }
    ModalMessages::getInstance()->stopLoading();
}

void ChildOnboardingScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    if(errorCode == ERROR_CODE_ALREADY_REGISTERED)
    {
        errorCode = ERROR_CODE_NAME_EXISTS;
    }
    
    const auto& errorMessageText = StringMgr::getInstance()->getErrorMessageWithCode(errorCode);
        
    PopupMessageBox* messageBox = PopupMessageBox::create();
    messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
    messageBox->setBody(errorMessageText.at(ERROR_BODY));
    messageBox->setButtonText(_("Back"));
    messageBox->setButtonColour(Style::Color::darkIndigo);
    messageBox->setPatternColour(Style::Color::azure);
    messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
        pSender->removeFromParent();
        this->transitionState(State::ENTER_NAME);
    });
    this->addChild(messageBox, 1);
    
    AnalyticsSingleton::getInstance()->childProfileCreatedErrorEvent(errorCode);
    ModalMessages::getInstance()->stopLoading();
}

NS_AZOOMEE_END

