#include "SlideShowScene.h"
#include "ConfigStorage.h"
#include "BaseScene.h"
#include "StringMgr.h"
#include "AudioMixer.h"
#include "HQHistoryManager.h"
#include "StringFunctions.h"

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

//---------------------- Create Slides -----------------

void SlideShowScene::imageAddedToCache(Texture2D* resulting_texture)
{
    std::vector<std::string> pathSplit = splitStringToVector(resulting_texture->getPath(), "slide_");
    
    auto slideImage = Sprite::createWithTexture( resulting_texture );
    slideImage->setPosition(layout2->getContentSize().width /2, layout2->getContentSize().height /2);
    
    if(pathSplit.size() == 2)
    {
        if(pathSplit.at(1) == "1.jpg")
            layout1->addChild(slideImage);
        else if(pathSplit.at(1) == "2.jpg")
            layout2->addChild(slideImage);
        else if(pathSplit.at(1) == "3.jpg")
            layout3->addChild(slideImage);
        else if(pathSplit.at(1) == "4.jpg")
            layout4->addChild(slideImage);
        else if(pathSplit.at(1) == "5.jpg")
            layout5->addChild(slideImage);
        else if(pathSplit.at(1) == "6.jpg")
        {
            layout6->addChild(slideImage);
            
            startExporingButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_START_EXPLORING));
            startExporingButton->setCenterPosition(Vec2(layout6->getContentSize().width/2, layout6->getContentSize().height/2));
            startExporingButton->setDelegate(this);
            startExporingButton->setMixPanelButtonName("SlideshowStartExploring");
            layout6->addChild(startExporingButton);
            
        }
    }
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
    
    //FUTURE OPTION - Create a Layout object with the slideshow.jpg file
    //the Layout calls the Async on itself, so the sprite and image is just added when ready.
    
    //Create Pointers to Pages, to add sprites later.
    //Stopping blank screen for 8 seconds on Pixie
    //Jan 2017
    layout1 = Layout::create();
    layout1->setContentSize(visibleSize);
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
    _pageView->insertCustomItem(layout1,0);
    _pageView->insertCustomItem(layout2,1);
    _pageView->insertCustomItem(layout3,2);
    _pageView->insertCustomItem(layout4,3);
    _pageView->insertCustomItem(layout5,4);
    _pageView->insertCustomItem(layout6,5);
    
    _pageView->scrollToItem(0);
    
    Director::getInstance()->getTextureCache()->addImageAsync("res/slideshow/slide_1.jpg", CC_CALLBACK_1(SlideShowScene::imageAddedToCache, this));
    Director::getInstance()->getTextureCache()->addImageAsync("res/slideshow/slide_2.jpg", CC_CALLBACK_1(SlideShowScene::imageAddedToCache, this));
    Director::getInstance()->getTextureCache()->addImageAsync("res/slideshow/slide_3.jpg", CC_CALLBACK_1(SlideShowScene::imageAddedToCache, this));
    Director::getInstance()->getTextureCache()->addImageAsync("res/slideshow/slide_4.jpg", CC_CALLBACK_1(SlideShowScene::imageAddedToCache, this));
    Director::getInstance()->getTextureCache()->addImageAsync("res/slideshow/slide_5.jpg", CC_CALLBACK_1(SlideShowScene::imageAddedToCache, this));
    Director::getInstance()->getTextureCache()->addImageAsync("res/slideshow/slide_6.jpg", CC_CALLBACK_1(SlideShowScene::imageAddedToCache, this));
    
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
