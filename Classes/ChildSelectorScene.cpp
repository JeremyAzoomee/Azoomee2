#include "ChildSelectorScene.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Cookie/CookieDataParser.h>
#include <math.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include "SettingsButton.h"
#include "OfflineHubScene.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "LoginLogicHandler.h"
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include "SceneManagerScene.h"
#include "FlowDataSingleton.h"
#include "ForceUpdateSingleton.h"
#include "ChatNotificationsSingleton.h"
#include <AzoomeeCommon/UI/PrivacyLayer.h>

#define OOMEE_LAYER_WIDTH 300
#define OOMEE_LAYER_HEIGHT 450
#define OOMEE_LAYER_GAP 210

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Scene* ChildSelectorScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ChildSelectorScene::create();
    scene->addChild(layer);

    return scene;
}

bool ChildSelectorScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    AnalyticsSingleton::getInstance()->logoutChildEvent();
    AnalyticsSingleton::getInstance()->setIsUserAnonymous(false);
    
    AudioMixer::getInstance()->stopBackgroundMusic();
    ChatNotificationsSingleton::getInstance()->stopNotificationsUpdate();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addVisualsToScene();
    createSettingsButton();
    addScrollViewForProfiles();
    addProfilesToScrollView();
    addPrivacyButton();
    
    auto newProfileButton = createNewProfileButton();
    newProfileButton->setPosition(origin.x + newProfileButton->getContentSize().width / 2, origin.y + visibleSize.height - newProfileButton->getContentSize().height * 0.8);
    this->addChild(newProfileButton);

    return true;
}

void ChildSelectorScene::onEnterTransitionDidFinish()
{
    ForceUpdateSingleton::getInstance()->doForceUpdateLogic();
    OfflineChecker::getInstance()->setDelegate(this);
    
    if(FlowDataSingleton::getInstance()->hasError())
    {
        MessageBox::createWith(FlowDataSingleton::getInstance()->getErrorCode(), nullptr);
    }
    
    FlowDataSingleton::getInstance()->clearData();
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------

void ChildSelectorScene::addVisualsToScene()
{
    addBackgroundToScreen();
    
    auto selectTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILD_SELECTSCENE_TITLE_LABEL));
    selectTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.9);
    this->addChild(selectTitle);
}

void ChildSelectorScene::addBackgroundToScreen()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto wireLeft = Sprite::create("res/childSelection/wireLeft.png");
    wireLeft->setPosition(wireLeft->getContentSize().width / 2, visibleSize.height / 2 + origin.y);
    this->addChild(wireLeft);
    
    auto wireRight = Sprite::create("res/childSelection/wireRight.png");
    wireRight->setPosition(visibleSize.width - wireRight->getContentSize().width / 2, visibleSize.height / 2 + origin.y);
    this->addChild(wireRight);
}

void ChildSelectorScene::createSettingsButton()
{
    auto settingsButton = SettingsButton::createSettingsButton(0.0f);
    settingsButton->setCenterPosition(Vec2(origin.x + visibleSize.width - settingsButton->getContentSize().width, origin.y + visibleSize.height - settingsButton->getContentSize().height));
    this->addChild(settingsButton);
}

void ChildSelectorScene::addPrivacyButton()
{
    PrivacyLayer* privacyLayer = PrivacyLayer::createWithColor();
    privacyLayer->setCenterPosition(Vec2(origin.x + privacyLayer->getContentSize().height/2 +privacyLayer->getContentSize().width/2,origin.y + privacyLayer->getContentSize().height));
    this->addChild(privacyLayer);
}

void ChildSelectorScene::addScrollViewForProfiles()
{
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width * 0.75, visibleSize.height * 0.8));
    scrollView->setPosition(Point(origin.x + visibleSize.width * 0.125, origin.y + visibleSize.height * 0.05));
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
    AnalyticsSingleton::getInstance()->registerNoOfChildren(ParentDataProvider::getInstance()->getAmountOfAvailableChildren());
    
    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        std::string oomeeUrl = ParentDataProvider::getInstance()->getAvatarForAnAvailableChildren(i);
        int oomeeNr = ConfigStorage::getInstance()->getOomeeNumberForUrl(oomeeUrl);
        
        auto profileLayer = createChildProfileButton(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(i), oomeeNr);
        profileLayer->setTag(i);
        profileLayer->setPosition(positionElementOnScrollView(profileLayer));
        addListenerToProfileLayer(profileLayer);
        scrollView->addChild(profileLayer);
    }
    
    auto parentButton = createParentProfileButton();
    parentButton->setPosition(positionElementOnScrollView(parentButton));
    scrollView->addChild(parentButton);
}

Layer *ChildSelectorScene::createChildProfileButton(std::string profileName, int oomeeNumber)
{
    auto profileLayer = Layer::create();
    profileLayer->setContentSize(Size(OOMEE_LAYER_WIDTH,OOMEE_LAYER_HEIGHT));
    
    auto glow = Sprite::create("res/childSelection/glow.png");
    glow->setPosition(profileLayer->getContentSize().width / 2, profileLayer->getContentSize().height /2);
    glow->setOpacity(0);
    profileLayer->addChild(glow);
    
    float delayTime = CCRANDOM_0_1() * 0.5;
    glow->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    auto oomee = Sprite::create(StringUtils::format("res/childSelection/%s.png", ConfigStorage::getInstance()->getNameForOomee(oomeeNumber).c_str()));
    oomee->setPosition(profileLayer->getContentSize().width / 2, profileLayer->getContentSize().height /2);
    oomee->setOpacity(0);
    oomee->setScale(1.25);
    profileLayer->addChild(oomee);
    
    oomee->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    auto profileLabel = createLabelChildName(profileName);
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
    listener->onTouchBegan = [=](Touch *touch, Event *event)
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
            
            if(target->getName() == "addChildButton")
                addChildButtonPressed(target);
            else //Oomee child pressed
            {
                parentIconSelected = false;
                OfflineChecker::getInstance()->setDelegate(nullptr);
                target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0)));
                int childNumber = target->getTag();
                AnalyticsSingleton::getInstance()->registerChildGenderAndAge(childNumber);
                BackEndCaller::getInstance()->childLogin(childNumber);
            }
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
    
    int numberOfChildrenWithParent = ParentDataProvider::getInstance()->getAmountOfAvailableChildren() + 1;
    if(numberOfChildrenWithParent > 4) numberOfChildrenWithParent = 4;
    
    float layerMultiplier = numberOfChildrenWithParent * 0.5;
    float gapMultiplier = (numberOfChildrenWithParent - 1) * 0.5;
    
    float startPosX = scrollView->getInnerContainerSize().width / 2 - (gapMultiplier * OOMEE_LAYER_GAP) - (layerMultiplier * OOMEE_LAYER_WIDTH);
    
    if(scrollViewChildren.size() == 0)
    {
        return Point(startPosX, scrollView->getInnerContainerSize().height - OOMEE_LAYER_GAP / 2 - layerToBeAdded->getContentSize().height);
    }
    
    Node *lastChild = scrollViewChildren.at(scrollViewChildren.size() - 1);
    Point lastPos = lastChild->getPosition();
    
    Point newPos = Point(lastPos.x + lastChild->getContentSize().width + OOMEE_LAYER_GAP, lastPos.y);
    
    if(newPos.x + layerToBeAdded->getContentSize().width > scrollView->getInnerContainerSize().width)
    {
        newPos = Point(startPosX, newPos.y - OOMEE_LAYER_GAP / 2 - layerToBeAdded->getContentSize().height);
    }
    
    return newPos;
}

Layer* ChildSelectorScene::createNewProfileButton()
{
    auto addButtonLayer = Layer::create();
    addButtonLayer->setContentSize(Size(OOMEE_LAYER_WIDTH,OOMEE_LAYER_HEIGHT));
    
    
    auto addButtonSprite = Sprite::create("res/childSelection/button_addChild_frame.png");
    addButtonSprite->setPosition(addButtonLayer->getContentSize().width / 2, addButtonLayer->getContentSize().height /2);
    addButtonSprite->setOpacity(0);
    addButtonSprite->setName("addChildButton");
    addListenerToProfileLayer(addButtonSprite);
    addButtonLayer->addChild(addButtonSprite);
    
    float delayTime = CCRANDOM_0_1();
    addButtonSprite->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    auto addButtonIcon = Sprite::create("res/childSelection/button_addChild_icon.png");
    addButtonIcon->setPosition(addButtonIcon->getContentSize().width * 0.9, addButtonSprite->getContentSize().height * 0.5);
    addButtonIcon->setOpacity(0);
    addButtonSprite->addChild(addButtonIcon);
    
    addButtonIcon->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    auto addButtonLabel = createLabelWith("Add a profile", Style::Font::Regular, Style::Color::brightAqua, 40);
    addButtonLabel->setPosition(addButtonSprite->getContentSize().width * 0.57, addButtonSprite->getContentSize().height * 0.47);
    addButtonLabel->setOpacity(0);
    addButtonSprite->addChild(addButtonLabel);
    
    addButtonLabel->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    return addButtonLayer;
}

Layer* ChildSelectorScene::createParentProfileButton()
{
    auto profileLayer = Layer::create();
    profileLayer->setContentSize(Size(OOMEE_LAYER_WIDTH,OOMEE_LAYER_HEIGHT));
    profileLayer->setPosition(Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width - profileLayer->getContentSize().width, profileLayer->getContentSize().height * 0.75);
    
    auto oomee = Sprite::create("res/childSelection/om_GenericParent.png");
    oomee->setPosition(profileLayer->getContentSize().width / 2, profileLayer->getContentSize().height * 0.62);
    oomee->setScale(1.25);
    oomee->setOpacity(0);
    profileLayer->addChild(oomee);
    
    float delayTime = CCRANDOM_0_1() * 0.5;
    oomee->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    auto profileLabel = createLabelChildName("Parent");
    profileLabel->setPosition(profileLayer->getContentSize().width / 2, profileLabel->getContentSize().height / 2);
    profileLabel->setOpacity(0);
    reduceLabelTextToFitWidth(profileLabel,OOMEE_LAYER_WIDTH);
    profileLayer->addChild(profileLabel);
    
    profileLabel->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChildSelector - ParentOomee");
            parentIconSelected = true;
            target->stopAllActions();
            target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0)));
            AwaitingAdultPinLayer::create()->setDelegate(this);
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), profileLayer);
    
    return profileLayer;
}

//---------------------- Check Email Verified Before Adding Child ---------------

void ChildSelectorScene::addChildButtonPressed(Node* target)
{
    AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChildSelector - AddChild");
    
    target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0)));
    
    AwaitingAdultPinLayer::create()->setDelegate(this);
    AnalyticsSingleton::getInstance()->childProfileStartEvent();
}

//----------------------- Delegate Functions ----------------------------
void ChildSelectorScene::AdultPinCancelled(AwaitingAdultPinLayer* layer)
{
    //Do Nothing.
}

void ChildSelectorScene::AdultPinAccepted(AwaitingAdultPinLayer* layer)
{
    if(parentIconSelected)
    {
        const std::string& userId = ParentDataProvider::getInstance()->getLoggedInParentId();
        const std::string& sessionId = ParentDataProvider::getInstance()->getLoggedInParentCdnSessionId();
        
        HttpRequestCreator* request = API::GetGordenRequest(userId, sessionId, this);
        request->execute();
        
        return;
    }
    
    ModalMessages::getInstance()->startLoading();
    //Delay so loading screen has time to appear, due to long loading of Spines
    this->scheduleOnce(schedule_selector(ChildSelectorScene::callDelegateFunction), .5);
}

void ChildSelectorScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(CookieDataParser::getInstance()->parseDownloadCookies(headers))
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
}

void ChildSelectorScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    //do nothing
}

void ChildSelectorScene::connectivityStateChanged(bool online)
{
    if(!online)
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(OfflineHub));
}

void ChildSelectorScene::callDelegateFunction(float dt)
{
    FlowDataSingleton::getInstance()->setFlowToNewProfile();
    OfflineChecker::getInstance()->setDelegate(nullptr);
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildAccount));
}

void ChildSelectorScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(messageBoxTitle == StringMgr::getInstance()->getErrorMessageWithCode(ERROR_CODE_EMAIL_VARIFICATION_REQUIRED)[ERROR_TITLE] && buttonTitle == StringMgr::getInstance()->getErrorMessageWithCode(ERROR_CODE_EMAIL_VARIFICATION_REQUIRED)[ERROR_BUTTON])
    {
        AnalyticsSingleton::getInstance()->logoutParentEvent();
        ParentDataParser::getInstance()->logoutChild();
        
        LoginLogicHandler::getInstance()->forceNewLogin();
    }
}

void ChildSelectorScene::onExit()
{
    OfflineChecker::getInstance()->setDelegate(nullptr);
    Node::onExit();
}

NS_AZOOMEE_END
