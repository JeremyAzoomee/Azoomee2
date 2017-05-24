#include "ChildSelectorScene.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
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

#define OOMEE_LAYER_WIDTH 300
#define OOMEE_LAYER_HEIGHT 400
#define OOMEE_LAYER_GAP 40

USING_NS_CC;
using namespace Azoomee;

Scene* ChildSelectorScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ChildSelectorScene::create();
    scene->addChild(layer);
    
    layer->_errorCode = LoginLogicHandler::getInstance()->getErrorMessageCodeToDisplay();

    return scene;
}

bool ChildSelectorScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    AnalyticsSingleton::getInstance()->logoutChildEvent();
    
    AudioMixer::getInstance()->stopBackgroundMusic();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addVisualsToScene();
    createSettingsButton();
    addScrollViewForProfiles();
    addProfilesToScrollView();
    

    return true;
}

void ChildSelectorScene::onEnterTransitionDidFinish()
{
    OfflineChecker::getInstance()->setDelegate(this);
    
    if(_errorCode !=0)
    {
        MessageBox::createWith(_errorCode, nullptr);
    }
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------

void ChildSelectorScene::addVisualsToScene()
{
    addGlowToScreen(this, 1);
    addSideWiresToScreen(this, 0, 2);
    
    auto selectTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILD_SELECTSCENE_TITLE_LABEL));
    selectTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.9);
    this->addChild(selectTitle);
}

void ChildSelectorScene::createSettingsButton()
{
    auto settingsButton = SettingsButton::createSettingsButton(0.0f);
    settingsButton->setCenterPosition(Vec2(origin.x + visibleSize.width - settingsButton->getContentSize().width, origin.y + visibleSize.height - settingsButton->getContentSize().height));
    this->addChild(settingsButton);
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
    //This has to be changed - not a good idea to include json handler in every classes that need it. There must be one storing class that can either give data back by request, or converts data into std::vector.
    
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
    
    addNewChildButtonToScrollView();
}

Layer *ChildSelectorScene::createChildProfileButton(std::string profileName, int oomeeNumber)
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
    CCLOG("Found delay time is: %f", delayTime);
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
            
            if(target->getName() == "addChildButton")
                addChildButtonPressed(target);
            else //Oomee child pressed
            {
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
    
    if(scrollViewChildren.size() == 0)
    {
        return Point(OOMEE_LAYER_GAP / 2, scrollView->getInnerContainerSize().height - OOMEE_LAYER_GAP / 2 - layerToBeAdded->getContentSize().height);
    }
    
    Node *lastChild = scrollViewChildren.at(scrollViewChildren.size() - 1);
    Point lastPos = lastChild->getPosition();
    
    Point newPos = Point(lastPos.x + lastChild->getContentSize().width + OOMEE_LAYER_GAP, lastPos.y);
    
    if(newPos.x + layerToBeAdded->getContentSize().width > scrollView->getInnerContainerSize().width)
    {
        newPos = Point(OOMEE_LAYER_GAP / 2, newPos.y - OOMEE_LAYER_GAP - layerToBeAdded->getContentSize().height);
    }
    
    return newPos;
}

void ChildSelectorScene::addNewChildButtonToScrollView()
{
    auto addButtonLayer = Layer::create();
    //Keep Content Size the same as Oomee
    addButtonLayer->setContentSize(Size(OOMEE_LAYER_WIDTH,OOMEE_LAYER_HEIGHT));
    
    auto selectionSprite = Sprite::create("res/childSelection/selection.png");
    selectionSprite->setPosition(addButtonLayer->getContentSize().width / 2, addButtonLayer->getContentSize().height / 2);
    selectionSprite->setOpacity(0);
    addButtonLayer->addChild(selectionSprite);
    
    auto addButtonSprite = Sprite::create("res/childSelection/button_add_child.png");
    addButtonSprite->setPosition(addButtonLayer->getContentSize().width / 2, addButtonLayer->getContentSize().height /2);
    addButtonSprite->setOpacity(0);
    addButtonSprite->setName("addChildButton");
    addListenerToProfileLayer(addButtonSprite);
    addButtonLayer->addChild(addButtonSprite);
    
    float delayTime = CCRANDOM_0_1();
    addButtonSprite->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));

    addButtonLayer->setPosition(positionElementOnScrollView(addButtonLayer));
    scrollView->addChild(addButtonLayer);
}

//---------------------- Check Email Verified Before Adding Child ---------------

void ChildSelectorScene::addChildButtonPressed(Node* target)
{
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
    ModalMessages::getInstance()->startLoading();
    //Delay so loading screen has time to appear, due to long loading of Spines
    this->scheduleOnce(schedule_selector(ChildSelectorScene::callDelegateFunction), .5);
}

void ChildSelectorScene::connectivityStateChanged(bool online)
{
    if(!online)
    {
        OfflineChecker::getInstance()->setDelegate(nullptr);
        Director::getInstance()->replaceScene(OfflineHubScene::createScene());
    }
}

void ChildSelectorScene::callDelegateFunction(float dt)
{
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
