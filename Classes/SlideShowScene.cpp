#include "SlideShowScene.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "BaseScene.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "HQHistoryManager.h"

using namespace Azoomee;


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
    
    createPageView();
    
    return true;
}

// --------------------- Setup -----------------------

void SlideShowScene::fadeInObject(Node* objectToFade)
{
    objectToFade->setOpacity(0);
    
    float delayTime = CCRANDOM_0_1() * 0.5;
    objectToFade->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
}

//---------------------- Create Slides -----------------

Layout* SlideShowScene::addSlide(int SlideNumber)
{
    Layout* slideLayer = Layout::create();
    slideLayer->setContentSize(visibleSize);
    
    auto slideImage = Sprite::create(StringUtils::format("res/slideshow/slide_%d.jpg",SlideNumber));
    slideImage->setPosition(slideLayer->getContentSize().width /2, slideLayer->getContentSize().height /2);
    slideLayer->addChild(slideImage);

    return slideLayer;
}

void SlideShowScene::SheduleSlideSpriteCreation(float dt)
{
    auto slideImage2 = Sprite::create("res/slideshow/slide_2.jpg");
    slideImage2->setPosition(layout2->getContentSize().width /2, layout2->getContentSize().height /2);
    layout2->addChild(slideImage2);
    
    auto slideImag3 = Sprite::create("res/slideshow/slide_3.jpg");
    slideImag3->setPosition(layout3->getContentSize().width /2, layout3->getContentSize().height /2);
    layout3->addChild(slideImag3);
    
    auto slideImag4 = Sprite::create("res/slideshow/slide_4.jpg");
    slideImag4->setPosition(layout4->getContentSize().width /2, layout4->getContentSize().height /2);
    layout4->addChild(slideImag4);
    
    auto slideImag5 = Sprite::create("res/slideshow/slide_5.jpg");
    slideImag5->setPosition(layout5->getContentSize().width /2, layout5->getContentSize().height /2);
    layout5->addChild(slideImag5);
    
    auto slideImag6 = Sprite::create("res/slideshow/slide_6.jpg");
    slideImag6->setPosition(layout6->getContentSize().width /2, layout6->getContentSize().height /2);
    layout6->addChild(slideImag6);
    
    startExporingButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_START_EXPLORING));
    startExporingButton->setCenterPosition(Vec2(layout6->getContentSize().width/2, layout6->getContentSize().height/2));
    startExporingButton->setDelegate(this);
    startExporingButton->setMixPanelButtonName("SlideshowStartExploring");
    layout6->addChild(startExporingButton);
}

void SlideShowScene::createPageView()
{
    currentSlideIndex = 0;
    slideShowStarted = false;
    
    _pageView = PageView::create();
    _pageView->setDirection(PageView::Direction::HORIZONTAL);
    _pageView->setContentSize(visibleSize);
    _pageView->cocos2d::Node::setPosition(origin.x, origin.y);
    _pageView->removeAllItems();
    _pageView->setIndicatorEnabled(true);
    _pageView->setIndicatorSelectedIndexColor(Color3B(28, 244, 244));
    
    //Create Pointers to Pages, to add sprites later.
    //Stopping blank screen for 8 seconds on Pixie
    //Jan 2017
    layout2 = Layout::create();
    layout2->setContentSize(visibleSize);
    layout3 = Layout::create();
    layout3->setContentSize(visibleSize);
    layout4 = Layout::create();
    layout4->setContentSize(visibleSize);
    layout5 = Layout::create();
    layout5->setContentSize(visibleSize);
    layout6 = Layout::create();
    layout6->setContentSize(visibleSize);
    
    //Add first slide and dummy slides, to add sprites later.
    _pageView->insertCustomItem(addSlide(1),0);
    _pageView->insertCustomItem(layout2,1);
    _pageView->insertCustomItem(layout3,2);
    _pageView->insertCustomItem(layout4,3);
    _pageView->insertCustomItem(layout5,4);
    _pageView->insertCustomItem(layout6,5);
    
    _pageView->scrollToItem(0);
    
    this->scheduleOnce(schedule_selector(SlideShowScene::SheduleSlideSpriteCreation),0.3);
    
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
            
            //Slideshow started - stops sound for first slide, as it is always delayed
            if(currentSlideIndex > _pageView->getCurrentPageIndex() && slideShowStarted)
                AudioMixer::getInstance()->playEffect(SWIPE_LEFT_AUDIO_EFFECT);
            else if(slideShowStarted)
                AudioMixer::getInstance()->playEffect(SWIPE_RIGHT_AUDIO_EFFECT);
            
            slideShowStarted = true;
            currentSlideIndex = _pageView->getCurrentPageIndex();
            
            auto nextSlideCallback = CallFunc::create(CC_CALLBACK_0(SlideShowScene::pageViewScrollToNextPage, this));
            
            _pageView->runAction(Sequence::create(DelayTime::create(10.0), nextSlideCallback, NULL));

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
    _pageView->stopAllActions();
    
    ConfigStorage::getInstance()->setFirstSlideShowSeen();
    
    HQHistoryManager::getInstance()->emptyHistory();
    auto baseScene = BaseScene::createScene();
    Director::getInstance()->replaceScene(baseScene);
    
}

void SlideShowScene::buttonPressed(ElectricDreamsButton* button)
{
    skipSlideShow();
}
