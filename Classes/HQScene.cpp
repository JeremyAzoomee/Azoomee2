#include "HQScene.h"
#include "HQSceneElement.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HQScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HQScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HQScene::setCategoryFromName(std::string name)
{
    if(name == "sVideoHQ") category = 0;
    if(name == "sAudioHQ") category = 1;
    if(name == "sArtsHQ") category = 2;
    if(name == "sGameHQ") category = 3;
    
    CCLOG("Selected category: %d", category);
}

void HQScene::setBackground(std::string name)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bgImage = Sprite::create(StringUtils::format("res/previewbg/%s.jpg", name.c_str()));
    bgImage->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(bgImage, 0);
}

void HQScene::setName(std::string name)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto title = Label::createWithTTF(name, "fonts/arial.ttf", 40);
    title->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    title->setColor(Color3B(255,0,0));
    this->addChild(title, 20);
    
    setBackground(name);
    setCategoryFromName(name);
}

void HQScene::addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        CCLOG("touch captured");
        
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getBoundingBox().size;//getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            startLocation = touch->getLocation();
            directionDecided = false;
            CCLOG("touch on target, %f, %f", touch->getLocation().x, touch->getLocation().y);
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [=](Touch *touch, Event *event)
    {
        Vec2 currentLocation = touch->getLocation();
        float distance = currentLocation.getDistance(startLocation);
        
        if((distance > 5)&&(directionDecided == false))
        {
            directionDecided = true;
            
            float xdiff = fabsf(currentLocation.x - startLocation.x);
            float ydiff = fabsf(currentLocation.y - startLocation.y);
            
            if(xdiff > ydiff)
            {
                CCLOG("Horizontal scroll");
                vScrollView->setTouchEnabled(false);
            }
            else
            {
                CCLOG("Vertical scroll");
                vScrollView->setTouchEnabled(true);
                
                Vector<Node *> allHorizontals = vScrollView->getChildren();
                for(int i = 0; i < allHorizontals.size(); i++)
                {
                    auto currentSv = (cocos2d::ui::ScrollView *)allHorizontals.at(i);
                    currentSv->setTouchEnabled(false);
                }
            }
        }
        
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event)
    {
        CCLOG("touch ended captured");
        vScrollView->setTouchEnabled(true);
        vScrollView->setSwallowTouches(false);
        
        Vector<Node *> allHorizontals = vScrollView->getChildren();
        for(int i = 0; i < allHorizontals.size(); i++)
        {
            auto currentSv = (cocos2d::ui::ScrollView *)allHorizontals.at(i);
            currentSv->setTouchEnabled(true);
            currentSv->setSwallowTouches(false);
        }
        
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), vScrollView);
}

void HQScene::createScrollView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Size vScrollFrameSize = Size(visibleSize.width, visibleSize.height * 0.82);
    
    auto vScrollView = cocos2d::ui::ScrollView::create();
    vScrollView->setContentSize(vScrollFrameSize);
    vScrollView->setPosition(origin);
    vScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    vScrollView->setTouchEnabled(true);
    vScrollView->setBounceEnabled(true);
    vScrollView->setInnerContainerSize(Size(visibleSize.width, visibleSize.height * 2));
    vScrollView->setScrollBarEnabled(false);
    vScrollView->setSwallowTouches(false);
    this->addChild(vScrollView, 1);
    
    this->addListenerToScrollView(vScrollView);
    
    for(int j = 0; j < 4; j++)
    {
        Size hScrollFrameSize = Size(visibleSize.width, visibleSize.height / 3);
        
        auto scrollView = cocos2d::ui::ScrollView::create();
        
        scrollView->setContentSize(hScrollFrameSize);
        scrollView->setPosition(Size(origin.x,origin.y + visibleSize.height * 2 - visibleSize.height / 2 * (j + 1)));
        scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
        scrollView->setBounceEnabled(true);
        scrollView->setTouchEnabled(true);
        scrollView->setInnerContainerSize(Size(visibleSize.width * 2, visibleSize.height / 3));
        scrollView->setSwallowTouches(false);
        scrollView->setScrollBarEnabled(false);
        scrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        scrollView->setBackGroundColor(Color3B(20,0,0));
        
        vScrollView->addChild(scrollView);
        
        std::vector<std::string> filenames;
        filenames.push_back("res/previewimg/1a.png");
        filenames.push_back("res/previewimg/1b.png");
        filenames.push_back("res/previewimg/1c.png"); //TO BE REMOVED, JUST FOR PLACEHOLDER PURPOSES
        
        for(int i = 0; i < 8; i++)
        {
            auto hqSceneElement = HQSceneElement::create();
            hqSceneElement->addHQSceneElement(category, 0, "res/previewimg/1a.png", "Angry Birds");
            //hqSceneElement->setPosition(100 + i * 450, 50);
            hqSceneElement->setPosition(50 + i * 350, 50);
            scrollView->addChild(hqSceneElement);
        }
    }

}

// on "init" you need to initialize your instance
bool HQScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //Creating a scrollview structure. vScrollView is the main, vertical scrollview, having several children of scrollViews, that can scroll horizontally.
    //We capture the the touches "under" the scrollView-s, and locking all horizontal movements on vertical touchMoves, and all vertical movements on horizontal touchMove.
    //The listener works the same way, as with all other nodes.
    
    
    
    return true;
}
