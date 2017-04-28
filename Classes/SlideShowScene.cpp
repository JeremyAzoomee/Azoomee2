#include "SlideShowScene.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "BaseScene.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "HQHistoryManager.h"
#include<cstdlib>

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

//---------------------- Create Slides -----------------

void SlideShowScene::imageAddedToCache(Texture2D* resulting_texture)
{
    std::vector<std::string> pathSplit = splitStringToVector(resulting_texture->getPath(), "slide_");
    
    auto slideImage = Sprite::createWithTexture( resulting_texture );
    slideImage->setPosition(visibleSize.width /2, visibleSize.height /2);
    
    if(pathSplit.size() == 2)
    {
        std::vector<std::string> dotSplit = splitStringToVector(pathSplit.at(1), ".");
        
        if(dotSplit.size() == 2)
        {
            int SlideNumber = std::atoi(dotSplit.at(0).c_str()) - 1;
            
            layoutVector.at(SlideNumber)->addChild(slideImage);
            
            if(SlideNumber == 5)
            {
                startExporingButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_START_EXPLORING));
                startExporingButton->setCenterPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
                startExporingButton->setDelegate(this);
                startExporingButton->setMixPanelButtonName("SlideshowStartExploring");
                layoutVector.at(SlideNumber)->addChild(startExporingButton);
            }
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
    
    for(int i=0;i<6;i++)
    {
        Layout* newLayout = Layout::create();
        newLayout->setContentSize(visibleSize);
        _pageView->insertCustomItem(newLayout,i);

        Director::getInstance()->getTextureCache()->addImageAsync(StringUtils::format("res/slideshow/slide_%d.jpg",i+1), CC_CALLBACK_1(SlideShowScene::imageAddedToCache, this));
        
        layoutVector.push_back(newLayout);
    }
    
    _pageView->scrollToItem(0);
    
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
