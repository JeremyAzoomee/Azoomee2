#include "ChildSelectorScene.h"
#include "BackEndCaller.h"
#include "DataStorage.h"


USING_NS_CC;

Scene* ChildSelectorScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ChildSelectorScene::create();
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool ChildSelectorScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    
    addVisualsToScene();
    addScrollViewForProfiles();
    addProfilesToScrollView();

    return true;
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

void ChildSelectorScene::addScrollViewForProfiles()
{
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width * 0.6, visibleSize.height * 0.6));
    scrollView->setPosition(Point(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.15));
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    scrollView->setInnerContainerSize(scrollView->getContentSize());
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(true);
    //scrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    //scrollView->setBackGroundColor(Color3B::BLUE);
    
    this->addChild(scrollView);
}

void ChildSelectorScene::addProfilesToScrollView()
{
    //This has to be changed - not a good idea to include json handler in every classes that need it. There must be one storing class that can either give data back by request, or converts data into std::vector.
    
    for(int i = 0; i < DataStorage::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        auto profileLayer = createChildProfileButton(DataStorage::getInstance()->getValueFromOneAvailableChild(i, "profileName"), RandomHelper::random_int(0, 4));
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
    profileLayer->setContentSize(Size(300,400));
    
    auto selectionSprite = Sprite::create("res/childSelection/selection.png");
    selectionSprite->setPosition(profileLayer->getContentSize().width / 2, profileLayer->getContentSize().height / 2);
    selectionSprite->setOpacity(0);
    profileLayer->addChild(selectionSprite);
    
    auto oomee = Sprite::create(StringUtils::format("res/childSelection/oomee_%d.png", oomeeNumber));
    oomee->setPosition(profileLayer->getContentSize().width / 2, profileLayer->getContentSize().height /2);
    oomee->setOpacity(0);
    profileLayer->addChild(oomee);
    
    float delayTime = CCRANDOM_0_1();
    oomee->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    auto profileLabel = Label::createWithTTF(profileName, "fonts/azoomee.ttf", 50);
    profileLabel->setColor(Color3B::WHITE);
    profileLabel->setPosition(profileLayer->getContentSize().width / 2, profileLabel->getContentSize().height / 2);
    profileLabel->setOpacity(0);
    profileLayer->addChild(profileLabel);
    
    profileLabel->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    return profileLayer;
}

void ChildSelectorScene::addListenerToProfileLayer(Layer *profileLayer)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
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
    
    listener->onTouchMoved = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        
        
        if((!touchMovedAway)&&(touch->getLocation().distance(startTouchPosition) > 10))
        {
            touchMovedAway = true;
            auto target = static_cast<Node*>(event->getCurrentTarget());
            target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0)));
        }
        
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        if(!touchMovedAway)
        {
            auto target = static_cast<Node*>(event->getCurrentTarget());
            target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0)));
            int childNumber = target->getTag();
            BackEndCaller::getInstance()->childLogin(childNumber);
            return true;
        }
        
        return false;
    };
    
    listener->onTouchCancelled = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        target->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.0)));
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), profileLayer);
}

Point ChildSelectorScene::positionElementOnScrollView(Layer *layerToBeAdded)
{
    float gapSize = 40;
    
    Vector<Node*> scrollViewChildren = scrollView->getChildren();
    
    if(scrollViewChildren.size() == 0)
    {
        return Point(gapSize / 2, scrollView->getInnerContainerSize().height - gapSize / 2 - layerToBeAdded->getContentSize().height);
    }
    
    Node *lastChild = scrollViewChildren.at(scrollViewChildren.size() - 1);
    Point lastPos = lastChild->getPosition();
    
    Point newPos = Point(lastPos.x + lastChild->getContentSize().width + gapSize, lastPos.y);
    
    if(newPos.y + layerToBeAdded->getContentSize().width > scrollView->getInnerContainerSize().width)
    {
        newPos = Point(gapSize / 2, newPos.y - gapSize - layerToBeAdded->getContentSize().height);
    }
    
    return newPos;
}

void ChildSelectorScene::addNewChildButtonToScrollView()
{
    auto addButtonLayer = Layer::create();
    //Keep Content Size the same as Oomee
    addButtonLayer->setContentSize(Size(300,400));
    
    auto selectionSprite = Sprite::create("res/childSelection/selection.png");
    selectionSprite->setPosition(addButtonLayer->getContentSize().width / 2, addButtonLayer->getContentSize().height / 2);
    selectionSprite->setOpacity(0);
    addButtonLayer->addChild(selectionSprite);
    
    auto addButtonSprite = Sprite::create("res/childSelection/button_add_child.png");
    addButtonSprite->setPosition(addButtonLayer->getContentSize().width / 2, addButtonLayer->getContentSize().height /2);
    addButtonSprite->setOpacity(0);
    addButtonLayer->addChild(addButtonSprite);
    
    float delayTime = CCRANDOM_0_1();
    addButtonSprite->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));

    addButtonLayer->setPosition(positionElementOnScrollView(addButtonLayer));
    scrollView->addChild(addButtonLayer);
}
