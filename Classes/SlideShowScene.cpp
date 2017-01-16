#include "SlideShowScene.h"

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
    //animateNextSlide(0);
    
    return true;
}

// --------------------- Setup -----------------------

void SlideShowScene::createStaticImages()
{
    auto bg_glow = Sprite::create("res/mainhub/bg_glow.png");
    bg_glow->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    bg_glow->setOpacity(0);
    this->addChild(bg_glow);
    
    float delayTime = CCRANDOM_0_1() * 0.5;
    bg_glow->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    auto leftImage = Sprite::create("res/childSelection/oomee_0.png");
    leftImage->setPosition(origin.x + leftImage->getContentSize().width , origin.y + leftImage->getContentSize().height);
    leftImage->setOpacity(0);
    this->addChild(leftImage);
    
    delayTime = CCRANDOM_0_1() * 0.5;
    leftImage->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    auto rightImage = Sprite::create("res/childSelection/oomee_0.png");
    rightImage->setPosition(origin.x + visibleSize.width - leftImage->getContentSize().width , origin.y + leftImage->getContentSize().height);
    rightImage->setOpacity(0);
    this->addChild(rightImage);
    
    delayTime = CCRANDOM_0_1() * 0.5;
    rightImage->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    for(int i =0; i<5;i++)
    {
        auto circle = Sprite::create(StringUtils::format("res/mainhub/circle_%d.png",i));
        circle->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
        circle->setOpacity(0);
        this->addChild(circle);
        
        delayTime = CCRANDOM_0_1() * 0.5;
        circle->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    }
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
    
    auto slideMessage = Label::createWithTTF("The best kid's TV shows, games and audiobooks in one safe app.", "fonts/azoomee.ttf", 90);
    slideMessage->setPosition(slideLayer->getContentSize().width /2, slideLayer->getContentSize().height *025);
    slideMessage->setColor(Color3B::WHITE);
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
    
   // _pageView->insertCustomItem(createSlide0(),0);
    //
    int pageCount = 4;
    for (int i = 0; i < pageCount; ++i)
    {
        Layout* layout = Layout::create();
        layout->setContentSize(visibleSize);
        
        Label* label = Label::Label::createWithTTF(StringUtils::format("page %d",(i+1)), "fonts/azoomee.ttf", 90);
        label->setColor(Color3B(192, 192, 192));
        label->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
        layout->addChild(label);
        
        auto pinkOomee = Sprite::create("res/childSelection/oomee_3.png");
        pinkOomee->setPosition(layout->getContentSize().width /2, layout->getContentSize().height /2);
        pinkOomee->setScale(2);
        layout->addChild(pinkOomee);
        
        _pageView->insertCustomItem(layout, i);
    }
    
    _pageView->scrollToItem(pageCount - 2);
    //This method is deprecated, we used here only testing purpose
    //pageView->addEventListenerPageView(this, pagevieweventselector(UIPageViewTest::pageViewEvent));
    
    
    _pageView->addEventListener((PageView::ccPageViewCallback)CC_CALLBACK_2(SlideShowScene::pageViewEvent, this));
    
    this->addChild(_pageView);
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
    _pageView->scrollToPage(_pageView->getCurrentPageIndex()+1);
}

void SlideShowScene::animateNextSlide(int newSlideIndex)
{

    
    
    /*if(currentSlideLayer != nullptr)
        removeCurrentSlide(currentSlideLayer);
    
    switch (newSlideIndex)
    
    {
        case 0: //TextInputIsEmail
        {
            currentSlideLayer = createSlide0();
            break;
        }
    }
    
    Point targetPosition = Point(origin.x, origin.y);
    float delayTime = 1;
    
    auto nextSlideCallback = CallFunc::create(CC_CALLBACK_0(SlideShowScene::animateNextSlide, this,0));
    
    this->runAction(Sequence::create(DelayTime::create(delayTime), EaseInOut::create(MoveTo::create(0.5, targetPosition), 2), nextSlideCallback, NULL));*/
    
}

void SlideShowScene::animageCurrentSlide(Layer* currentSlideLayer)
{
    Point targetPosition = Point(origin.x - visibleSize.width, origin.y);
    float delayTime = 1;
    
    auto removeLayerCallback = CallFunc::create(CC_CALLBACK_0(SlideShowScene::removeCurrentSlide, this,currentSlideLayer));
    
    this->runAction(Sequence::create(DelayTime::create(delayTime), EaseInOut::create(MoveTo::create(0.5, targetPosition), 2), removeLayerCallback, NULL));
}

void SlideShowScene::removeCurrentSlide(Layer* currentSlideLayer)
{

}
