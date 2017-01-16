#include "SlideShowScene.h"
#include "LoginScene.h"

Scene* SlideShowScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SlideShowScene::create();
    scene->addChild(layer);

    return scene;
}

bool SlideShowScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    currentSlideLayer = nullptr;
    
    createStaticImages();
    createPageView();
    createButtons();
    
    return true;
}

// --------------------- Setup -----------------------

void SlideShowScene::createButtons()
{
    skipButton = ElectricDreamsButton::createTextAsButton("Skip");
    skipButton->setCenterPosition(Vec2(origin.x+visibleSize.width - skipButton->getContentSize().width/2 - skipButton->getContentSize().height/2, origin.y + visibleSize.height- skipButton->getContentSize().height));
    skipButton->setDelegate(this);
    this->addChild(skipButton);
    
    fadeInObject(skipButton);
}

void SlideShowScene::createStaticImages()
{
    auto bg_glow = Sprite::create("res/mainhub/bg_glow.png");
    bg_glow->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(bg_glow);
    
    fadeInObject(bg_glow);
    
    auto leftImage = Sprite::create("res/childSelection/oomee_0.png");
    leftImage->setPosition(origin.x + leftImage->getContentSize().width , origin.y + leftImage->getContentSize().height);
    this->addChild(leftImage);
    
    fadeInObject(leftImage);
    
    auto rightImage = Sprite::create("res/childSelection/oomee_0.png");
    rightImage->setPosition(origin.x + visibleSize.width - leftImage->getContentSize().width , origin.y + leftImage->getContentSize().height);
    this->addChild(rightImage);
    
    fadeInObject(rightImage);
    
    for(int i =0; i<5;i++)
    {
        auto circle = Sprite::create(StringUtils::format("res/mainhub/circle_%d.png",i));
        circle->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
        this->addChild(circle);
        
        fadeInObject(circle);
    }
}

void SlideShowScene::fadeInObject(Node* objectToFade)
{
    objectToFade->setOpacity(0);
    
    float delayTime = CCRANDOM_0_1() * 0.5;
    objectToFade->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
}

//---------------------- Create Slides -----------------

Layout* SlideShowScene::createSlide0()
{
    Layout* slideLayer = Layout::create();
    slideLayer->setContentSize(visibleSize);
    
    auto pinkOomee = Sprite::create("res/childSelection/oomee_3.png");
    pinkOomee->setPosition(slideLayer->getContentSize().width /2, slideLayer->getContentSize().height /2);
    pinkOomee->setScale(2);
    slideLayer->addChild(pinkOomee);
    
    fadeInObject(pinkOomee);
    
    auto slideMessage = Label::createWithTTF("The best kid's TV shows, games and audiobooks in one safe app.", "fonts/azoomee.ttf", 90);
    slideMessage->setWidth(slideLayer->getContentSize().width * 0.66);
    slideMessage->setPosition(slideLayer->getContentSize().width /2, slideLayer->getContentSize().height *0.25);
    slideMessage->setColor(Color3B::WHITE);
    slideMessage->setHorizontalAlignment(TextHAlignment::CENTER);
    slideLayer->addChild(slideMessage);
    
    fadeInObject(slideMessage);
    
    return slideLayer;
}

Layout* SlideShowScene::createSlide1()
{
    Layout* slideLayer = Layout::create();
    slideLayer->setContentSize(visibleSize);
    
    auto pinkOomee = Sprite::create("res/childSelection/oomee_0.png");
    pinkOomee->setPosition(slideLayer->getContentSize().width /2, slideLayer->getContentSize().height /2);
    pinkOomee->setScale(2);
    slideLayer->addChild(pinkOomee);
    
    auto slideMessage = Label::createWithTTF("It's an awesome APP.", "fonts/azoomee.ttf", 90);
    slideMessage->setWidth(slideLayer->getContentSize().width * 0.66);
    slideMessage->setPosition(slideLayer->getContentSize().width /2, slideLayer->getContentSize().height *0.25);
    slideMessage->setColor(Color3B::WHITE);
    slideMessage->setHorizontalAlignment(TextHAlignment::CENTER);
    slideLayer->addChild(slideMessage);
    
    return slideLayer;
}

Layout* SlideShowScene::createSlide2()
{
    Layout* slideLayer = Layout::create();
    slideLayer->setContentSize(visibleSize);
    
    auto pinkOomee = Sprite::create("res/childSelection/oomee_1.png");
    pinkOomee->setPosition(slideLayer->getContentSize().width /2, slideLayer->getContentSize().height /2);
    pinkOomee->setScale(2);
    slideLayer->addChild(pinkOomee);
    
    auto slideMessage = Label::createWithTTF("Pat yourself on the back for selecting AZOOMEE!", "fonts/azoomee.ttf", 90);
    slideMessage->setWidth(slideLayer->getContentSize().width * 0.66);
    slideMessage->setPosition(slideLayer->getContentSize().width /2, slideLayer->getContentSize().height *0.25);
    slideMessage->setColor(Color3B::WHITE);
    slideMessage->setHorizontalAlignment(TextHAlignment::CENTER);
    slideLayer->addChild(slideMessage);
    
    return slideLayer;
}

void SlideShowScene::createPageView()
{
    _pageView = PageView::create();
    _pageView->setDirection(PageView::Direction::HORIZONTAL);
    _pageView->setContentSize(visibleSize);
    _pageView->cocos2d::Node::setPosition(origin.x, origin.y);
    _pageView->removeAllItems();
    _pageView->setIndicatorEnabled(true);
    _pageView->setIndicatorSelectedIndexColor(Color3B(28, 244, 244));
    
    _pageView->insertCustomItem(createSlide0(),0);
    _pageView->insertCustomItem(createSlide1(),1);
    _pageView->insertCustomItem(createSlide2(),2);
    
    _pageView->scrollToItem(0);
    
    _pageView->addEventListener((PageView::ccPageViewCallback)CC_CALLBACK_2(SlideShowScene::pageViewEvent, this));
    
    this->addChild(_pageView);
    
    fadeInObject(_pageView);
}

//----------------------- Actions ---------------------

void SlideShowScene::pageViewEvent(Ref *pSender, PageView::EventType type)
{
    switch (type)
    {
        case PageView::EventType::TURNING:
        {
            _pageView->stopAllActions();
            
            auto nextSlideCallback = CallFunc::create(CC_CALLBACK_0(SlideShowScene::pageViewScrollToNextPage, this));
            
            _pageView->runAction(Sequence::create(DelayTime::create(3.0), nextSlideCallback, NULL));

        }
            break;
            
        default:
            break;
    }
}

void SlideShowScene::pageViewScrollToNextPage()
{
    auto vectorOfSlides = _pageView->getItems();
    
    if(vectorOfSlides.size() == _pageView->getCurrentPageIndex()+1)
        skipSlideShow();
    else
        _pageView->scrollToItem(_pageView->getCurrentPageIndex()+1);
}

void SlideShowScene::skipSlideShow()
{
    //should go to preview Mode
    //Maybe slowly fade out object first.
    _pageView->stopAllActions();
    
    auto loginScene = LoginScene::createScene(0);
    Director::getInstance()->replaceScene(loginScene);
    
}

void SlideShowScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == skipButton) skipSlideShow();
}
