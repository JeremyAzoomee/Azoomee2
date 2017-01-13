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
    animateNextSlide(0);
    
    return true;
}

// --------------------- Setup -----------------------

void SlideShowScene::createStaticImages()
{
    auto bg_glow = Sprite::create("res/mainhub/bg_glow.png");
    bg_glow->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(bg_glow);
    
    auto leftImage = Sprite::create("res/childSelection/oomee_0.png");
    leftImage->setPosition(origin.x + leftImage->getContentSize().width , origin.y + leftImage->getContentSize().height);
    this->addChild(leftImage);
    
    auto rightImage = Sprite::create("res/childSelection/oomee_0.png");
    rightImage->setPosition(origin.x + visibleSize.width - leftImage->getContentSize().width , origin.y + leftImage->getContentSize().height);
    this->addChild(rightImage);
    
    for(int i =0; i<5;i++)
    {
        auto circle0 = Sprite::create(StringUtils::format("res/mainhub/circle_%d.png",i));
        circle0->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
        this->addChild(circle0);
    }
}

//---------------------- Create Slides -----------------

Layer* SlideShowScene::createSlide0()
{
    Layer* slideLayer = cocos2d::Layer::create();
    slideLayer->setContentSize(visibleSize);
    
    auto pinkOomee = Sprite::create("res/childSelection/oomee_3.png");
    pinkOomee->setPosition(slideLayer->getContentSize().width /2, slideLayer->getContentSize().height /2);
    pinkOomee->setScale(2);
    slideLayer->addChild(pinkOomee);
    
    auto slideMessage = Label::createWithTTF("The best kid's TV shows, games and audiobooks in one safe app.", "fonts/azoomee.ttf", 90);
    slideMessage->setPosition(slideLayer->getContentSize().width /2, slideLayer->getContentSize().height *025);
    slideMessage->setColor(Color3B::WHITE);
    slideLayer->addChild(slideMessage);
    
    slideLayer->setPosition(Vec2(origin.x + visibleSize.width, origin.y));
    this->addChild(slideLayer);
    
    return slideLayer;
}

//----------------------- Actions ---------------------

void SlideShowScene::animateNextSlide(int newSlideIndex)
{
    if(currentSlideLayer != nullptr)
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
    
    this->runAction(Sequence::create(DelayTime::create(delayTime), EaseInOut::create(MoveTo::create(0.5, targetPosition), 2), nextSlideCallback, NULL));
    
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
