#include "SlideShowScene.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <cstdlib>
#include "SceneManagerScene.h"
#include "IAPUpsaleLayer.h"
#include "BackEndCaller.h"

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
    addLoginButton();
    
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
            
            ElectricDreamsButton *startTrialButton = ElectricDreamsButton::createGreenButton("Start your free 7-day trial", 1000);
            startTrialButton->setCenterPosition(Vec2(layoutVector.at(SlideNumber)->getContentSize().width/2, visibleSize.height/10));
            startTrialButton->setDelegate(this);
            startTrialButton->setMixPanelButtonName(StringUtils::format("SlideShow-StartTrial-%d",SlideNumber));
            startTrialButton->setName("startTrialButton");
            layoutVector.at(SlideNumber)->addChild(startTrialButton);
            
            if(SlideNumber == 4)
            {
                skipButton = ElectricDreamsButton::createTextAsButtonAqua(StringMgr::getInstance()->getStringForKey(BUTTON_SKIP), 60, true);
                
                float buttonBoarder = skipButton->getContentSize().height/2;
                
                skipButton->setPosition(buttonBoarder, buttonBoarder);
                skipButton->setDelegate(this);
                skipButton->setMixPanelButtonName("SlideShow-Skip");
                layoutVector.at(SlideNumber)->addChild(skipButton);
            }
        }
    }
    this->release();
}

void SlideShowScene::createPageView()
{
    //-----------Setup Slideshow------------
    
    currentSlideIndex = 0;
    slideShowStarted = false;
    
    _pageView = PageView::create();
    _pageView->setDirection(PageView::Direction::HORIZONTAL);
    _pageView->setContentSize(visibleSize);
    _pageView->cocos2d::Node::setPosition(origin.x, origin.y);
    _pageView->removeAllItems();
    _pageView->setIndicatorEnabled(true);
    _pageView->setIndicatorSelectedIndexColor(Color3B(28, 244, 244));
    _pageView->setIndicatorPosition(Vec2(visibleSize.width/2,visibleSize.height/50));
    
    //-------------Load Slideshow Images in Background--------------------
    
    for(int i=0;i<5;i++)
    {
        Layout* newLayout = Layout::create();
        newLayout->setContentSize(visibleSize);
        _pageView->insertCustomItem(newLayout,i);
        this->retain();
        Director::getInstance()->getTextureCache()->addImageAsync(StringUtils::format("res/slideshow/slide_%d.jpg",i+1), CC_CALLBACK_1(SlideShowScene::imageAddedToCache, this));

        layoutVector.push_back(newLayout);
    }
    
    //--------------Initialise Slideshow--------------
    
    _pageView->scrollToItem(0);
    _pageView->addEventListener((PageView::ccPageViewCallback)CC_CALLBACK_2(SlideShowScene::pageViewEvent, this));
    this->addChild(_pageView);
}

void SlideShowScene::addLoginButton()
{
    loginButton = ElectricDreamsButton::createTextAsButtonAqua(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_IN), 60, true);
    
    float buttonBoarder = loginButton->getContentSize().height/2;
    
    loginButton->setPosition(origin.x + visibleSize.width - loginButton->getContentSize().width-buttonBoarder, origin.y+visibleSize.height-loginButton->getContentSize().height - buttonBoarder);
    loginButton->setDelegate(this);
    loginButton->setMixPanelButtonName("SlideShow-Login");
    this->addChild(loginButton);
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
    
    if(vectorOfSlides.size() != _pageView->getCurrentPageIndex()+1)
        _pageView->scrollToItem(_pageView->getCurrentPageIndex()+1);
}

void SlideShowScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == loginButton)
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Login));
    else if (button->getName() == "startTrialButton")
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Onboarding));
    else if (button == skipButton)
        BackEndCaller::getInstance()->anonymousDeviceLogin();
}
