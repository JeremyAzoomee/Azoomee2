#include "ChildSelectorScene.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <math.h>
#include "LoginScene.h"
#include <AzoomeeChat/UI/FriendListScene.h>

#define OOMEE_LAYER_WIDTH 300
#define OOMEE_LAYER_HEIGHT 400
#define OOMEE_LAYER_GAP 40

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

bool ChildSelectorScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    AnalyticsSingleton::getInstance()->logoutChildEvent();
    AudioMixer::getInstance()->stopBackgroundMusic();
    
    wiresLayer = Layer::create();
    wiresLayer->setPosition(Director::getInstance()->getVisibleOrigin() * -1.0f);
    addChild(wiresLayer);
    
    addVisualsToScene();
    
    titleLabel = createLabelHeader(_("Select your profile"));
    titleLabel->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.9);
    this->addChild(titleLabel);
    
    addScrollViewForProfiles();
    addButtonsScene();
    
    return true;
}

void ChildSelectorScene::onEnter()
{
    Super::onEnter();
    
    onSizeChanged();
    
    // Register for API events
    AuthAPI::getInstance()->registerObserver(this);
    
    // Request profiles
    AuthAPI::getInstance()->logoutChild();
    AuthAPI::getInstance()->getAvailableChildren();
    ModalMessages::getInstance()->startLoading();
}

void ChildSelectorScene::onExit()
{
    Super::onExit();
    
    // Unregister on chat API events
    AuthAPI::getInstance()->removeObserver(this);
}

#pragma mark - Size Changes

void ChildSelectorScene::onSizeChanged()
{
    Super::onSizeChanged();
    
    visibleSize = getContentSize();
    
    // If initialised
    if(wiresLayer)
    {
        wiresLayer->removeAllChildren();
        wiresLayer->setPosition(Director::getInstance()->getVisibleOrigin() * -1.0f);
        addVisualsToScene();
        
        titleLabel->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.9);
        backButton->setCenterPosition(Vec2(backButton->getContentSize().width*.7, visibleSize.height - backButton->getContentSize().height*.7));
        scrollView->setContentSize(Size(visibleSize.width * 0.6, visibleSize.height * 0.8));
        scrollView->setPosition(Point(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.05));
        
        // Update scrollable size
        scrollView->setInnerContainerSize(getScrollviewInnerSize(scrollView->getContentSize().width));
        
        // Re-position scrollview elements
        for(const auto& child : scrollView->getChildren())
        {
            child->setPosition(positionElementOnScrollView((Layer*)child));
        }
        
        scrollView->jumpToTop();
    }
}

#pragma mark - UI

void ChildSelectorScene::addVisualsToScene()
{
    addGlowToScreen(wiresLayer, 1);
    addSideWiresToScreen(wiresLayer, 0, 2);
}

void ChildSelectorScene::addButtonsScene()
{
    backButton = ElectricDreamsButton::createBackButton();
    backButton->setCenterPosition(Vec2(origin.x +backButton->getContentSize().width*.7, origin.y + visibleSize.height - backButton->getContentSize().height*.7));
    backButton->setDelegate(this);
    this->addChild(backButton);
}

void ChildSelectorScene::addScrollViewForProfiles()
{
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width * 0.6, visibleSize.height * 0.8));
    scrollView->setPosition(Point(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.05));
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    scrollView->setInnerContainerSize(getScrollviewInnerSize(scrollView->getContentSize().width));
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(true);
    
    this->addChild(scrollView);
}

Size ChildSelectorScene::getScrollviewInnerSize(float scrollviewWidth)
{
    double oomeeLayersPerWidth;
    modf(scrollviewWidth/(OOMEE_LAYER_WIDTH + OOMEE_LAYER_GAP), &oomeeLayersPerWidth);
    
    double oomeeLayesNeededForHeight;
    modf((ParentDataProvider::getInstance()->getAmountOfAvailableChildren() / oomeeLayersPerWidth) + 1, &oomeeLayesNeededForHeight);
    
    return Size(scrollviewWidth, (OOMEE_LAYER_GAP+OOMEE_LAYER_HEIGHT)*oomeeLayesNeededForHeight);
}

void ChildSelectorScene::addProfilesToScrollView()
{
    // Remove all old elements
    scrollView->removeAllChildren();
    
    // Update scrollable size
    scrollView->setInnerContainerSize(getScrollviewInnerSize(scrollView->getContentSize().width));
    
    AnalyticsSingleton::getInstance()->registerNoOfChildren(ParentDataProvider::getInstance()->getAmountOfAvailableChildren());
    
    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        const std::string& oomeeUrl = ParentDataProvider::getInstance()->getAvatarForAnAvailableChild(i);
        int oomeeNr = ConfigStorage::getInstance()->getOomeeNumberForUrl(oomeeUrl);
        
        auto profileLayer = createChildProfileButton(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(i), oomeeNr);
        profileLayer->setTag(i);
        profileLayer->setPosition(positionElementOnScrollView(profileLayer));
        addListenerToProfileLayer(profileLayer);
        scrollView->addChild(profileLayer);
    }
}

Layer* ChildSelectorScene::createChildProfileButton(std::string profileName, int oomeeNumber)
{
    auto profileLayer = Layer::create();
    profileLayer->setContentSize(Size(OOMEE_LAYER_WIDTH,OOMEE_LAYER_HEIGHT));
    
    auto selectionSprite = Sprite::create("res/childSelection/selection.png");
    selectionSprite->setPosition(profileLayer->getContentSize().width / 2, profileLayer->getContentSize().height / 2);
    selectionSprite->setOpacity(0);
    profileLayer->addChild(selectionSprite);
    
    auto oomee = Sprite::create(StringUtils::format("res/childSelection/%s.png", ConfigStorage::getInstance()->getNameForOomee(oomeeNumber).c_str()));
    oomee->setPosition(profileLayer->getContentSize().width / 2, profileLayer->getContentSize().height /2);
    oomee->setOpacity(0);
    profileLayer->addChild(oomee);
    
    float delayTime = CCRANDOM_0_1() * 0.5;
    oomee->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    auto profileLabel = createLabelBody(profileName);
    profileLabel->setPosition(profileLayer->getContentSize().width / 2, profileLabel->getContentSize().height / 2);
    profileLabel->setOpacity(0);
    reduceLabelTextToFitWidth(profileLabel,OOMEE_LAYER_WIDTH);
    profileLayer->addChild(profileLabel);
    
    profileLabel->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    return profileLayer;
}

void ChildSelectorScene::addListenerToProfileLayer(Node *profileLayer)
{
    auto listener = EventListenerTouchOneByOne::create();
    //listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        touchMovedAway = false;
        auto target = static_cast<Node*>(event->getCurrentTarget());
        startTouchPosition = touch->getLocation();
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.1)));
            
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [=](Touch *touch, Event *event)
    {
        if((!touchMovedAway)&&(touch->getLocation().distance(startTouchPosition) > 10))
        {
            touchMovedAway = true;
            auto target = static_cast<Node*>(event->getCurrentTarget());
            target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0)));
        }
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event)
    {
        if(!touchMovedAway)
        {
            auto target = static_cast<Node*>(event->getCurrentTarget());
         
            AudioMixer::getInstance()->playEffect(SELECT_OOMEE_AUDIO_EFFECT);
            
            //Oomee child pressed
            target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0)));
            int childNumber = target->getTag();
            AnalyticsSingleton::getInstance()->registerChildGenderAndAge(childNumber);
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("childSelectedForLogin");
            
            const std::string& profileName = ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(childNumber);
            AuthAPI::getInstance()->loginChild(profileName);
            ModalMessages::getInstance()->startLoading();
            return true;
        }
        
        return false;
    };
    
    listener->onTouchCancelled = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0)));
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), profileLayer);
}

Point ChildSelectorScene::positionElementOnScrollView(Layer *layerToBeAdded)
{
    Vector<Node*> scrollViewChildren = scrollView->getChildren();
    
    ssize_t index = scrollViewChildren.getIndex(layerToBeAdded);
    if(index == -1)
    {
        index = scrollViewChildren.size();
    }
    
    if(index == 0)
    {
        return Point(OOMEE_LAYER_GAP / 2, scrollView->getContentSize().height - OOMEE_LAYER_GAP / 2 - layerToBeAdded->getContentSize().height);
    }
    
    Node *lastChild = scrollViewChildren.at(index - 1);
    Point lastPos = lastChild->getPosition();
    
    Point newPos = Point(lastPos.x + lastChild->getContentSize().width + OOMEE_LAYER_GAP, lastPos.y);
    
    if(newPos.x + layerToBeAdded->getContentSize().width > scrollView->getContentSize().width)
    {
        newPos = Point(OOMEE_LAYER_GAP / 2, newPos.y - OOMEE_LAYER_GAP - layerToBeAdded->getContentSize().height);
    }
    
    return newPos;
}

#pragma mark - ElectricDreamsButtonDelegate

void ChildSelectorScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == backButton)
    {
        AuthAPI::getInstance()->logoutUser();
        auto loginScene = LoginScene::create();
        Director::getInstance()->replaceScene(loginScene);
    }
}

#pragma mark - MessageBoxDelegate

void ChildSelectorScene::MessageBoxButtonPressed(std::string messageBoxTitle, std::string buttonTitle)
{
    // Messagebox only used for errors so far, so just go back to login
    AuthAPI::getInstance()->logoutUser();
    auto loginScene = LoginScene::create();
    Director::getInstance()->replaceScene(loginScene);
}

#pragma mark - AuthAPIObserver

//#define AZOOMEE_CHAT_AUTO_CHILD_LOGIN

void ChildSelectorScene::onAuthAPIGetAvailableChildren()
{
    // Got children
    addProfilesToScrollView();
    ModalMessages::getInstance()->stopLoading();
    
#ifdef AZOOMEE_CHAT_AUTO_CHILD_LOGIN
    // Auto select first child
    const std::string& profileName = ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(3);
    AuthAPI::getInstance()->loginChild(profileName);
    ModalMessages::getInstance()->startLoading();
#endif
}

void ChildSelectorScene::onAuthAPIChildLogin()
{
    auto chatScene = FriendListScene::create();
    Director::getInstance()->replaceScene(chatScene);
}

void ChildSelectorScene::onAuthAPIRequestFailed(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(errorCode, this);
}

NS_AZOOMEE_CHAT_END
