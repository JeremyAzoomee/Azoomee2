#include "ChildSelectorScene.h"
#include "BackEndCaller.h"
#include "ParentDataProvider.h"
#include "ChildAccountScene.h"
#include <math.h>
#include "ModalMessages.h"
#include "ConfigStorage.h"
#include "StringStorage.h"

#define OOMEE_LAYER_WIDTH 300
#define OOMEE_LAYER_HEIGHT 400
#define OOMEE_LAYER_GAP 40

USING_NS_CC;

Scene* ChildSelectorScene::createScene(long errorCode)
{
    auto scene = Scene::create();
    auto layer = ChildSelectorScene::create();
    scene->addChild(layer);
    
    layer->_errorCode = errorCode;

    return scene;
}

bool ChildSelectorScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addVisualsToScene();
    createSettingsButton();
    addScrollViewForProfiles();
    addProfilesToScrollView();
    

    return true;
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------
void ChildSelectorScene::onEnterTransitionDidFinish()
{
    if(_errorCode !=0)
    {
        handleErrorCode(_errorCode);
    }
}

void ChildSelectorScene::handleErrorCode(long errorCode)
{
    //#TODO handle modal message strings.
    ModalMessages::getInstance()->createMessageWithSingleButton("ERROR", StringUtils::format("Error Code:%ld",errorCode), "OK");
}

void ChildSelectorScene::addVisualsToScene()
{
    auto bg = Sprite::create("res/mainhub/bg_glow.png");
    bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(bg);
    
    auto bg1 = Sprite::create("res/login/wire_left.png");
    bg1->setPosition(bg1->getContentSize().width / 2, bg1->getContentSize().height / 2);
    this->addChild(bg1);
    
    auto bg2 = Sprite::create("res/login/wire_right.png");
    bg2->setPosition(visibleSize.width - bg2->getContentSize().width / 2, bg2->getContentSize().height / 2);
    this->addChild(bg2);
    
    auto selectTitle = Label::createWithTTF("Select your kid's profile", "fonts/azoomee.ttf", 90);
    selectTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.9);
    selectTitle->setColor(Color3B(28, 244, 244));
    this->addChild(selectTitle);
}

void ChildSelectorScene::createSettingsButton()
{
    auto settingsButton = ElectricDreamsButton::createSettingsButton(0.0f);
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
    
    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        std::string oomeeUrl = ParentDataProvider::getInstance()->getValueFromOneAvailableChild(i, "avatar");
        int oomeeNr = ConfigStorage::getInstance()->getOomeeNumberForUrl(oomeeUrl);
        
        auto profileLayer = createChildProfileButton(ParentDataProvider::getInstance()->getValueFromOneAvailableChild(i, "profileName"), oomeeNr);
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
    
    auto profileLabel = Label::createWithTTF(profileName, "fonts/azoomee.ttf", 50);
    profileLabel->setColor(Color3B::WHITE);
    profileLabel->setPosition(profileLayer->getContentSize().width / 2, profileLabel->getContentSize().height / 2);
    profileLabel->setOpacity(0);
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
            
            if(target->getName() == "addChildButton")
                addChildButtonPressed(target);
            else //Oomee child pressed
            {
                target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0)));
                int childNumber = target->getTag();
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

void ChildSelectorScene::addChildButtonPressed(Node* target)
{
    //Check is email verified, if not refresh profile, then error
    if((ParentDataProvider::getInstance()->getParentLoginValue("actorStatus") == "VERIFIED")||(ParentDataProvider::getInstance()->getParentLoginValue("actorStatus") == "ACTIVE"))
    {
        target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0)));
        AwaitingAdultPinLayer::create()->setDelegate(this);
    }
    else
        ModalMessages::getInstance()->createMessageWithSingleButton(EMAIL_NOT_VERIFIED_ERROR_TITLE_TEXT, EMAIL_NOT_VERIFIED_ERROR_BODY_TEXT, EMAIL_NOT_VERIFIED_ERROR_BUTTON_TEXT);
}

//----------------------- Delegate Functions ----------------------------
void ChildSelectorScene::AdultPinCancelled(AwaitingAdultPinLayer* layer)
{
    //Do Nothing.
}

void ChildSelectorScene::AdultPinAccepted(AwaitingAdultPinLayer* layer)
{
    auto newChildScene = ChildAccountScene::createScene("", 0);
    Director::getInstance()->replaceScene(newChildScene);
}
