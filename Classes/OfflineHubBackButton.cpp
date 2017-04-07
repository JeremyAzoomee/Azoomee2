#include "OfflineHubBackButton.h"
#include "OfflineHubScene.h"
#include "AudioMixer.h"

USING_NS_CC;

Scene* OfflineHubBackButton::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineHubScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineHubBackButton::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto backButton = createBackButton();
    addListenerToBackButton(backButton);
    
    return true;
}

//----------------------------------------------ALL METHODS BEYOND THIS LINE ARE PRIVATE----------------------------------------------------

Sprite* OfflineHubBackButton::createBackButton()
{
    auto backButtonImage = Sprite::create("res/hqscene/back_btn.png");
    
    this->setContentSize(backButtonImage->getContentSize());
    backButtonImage->setPosition(this->getContentSize() / 2);
    
    backButtonImage->setOpacity(0);
    backButtonImage->setName("backButton");
    this->addChild(backButtonImage);
    
    backButtonImage->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    return backButtonImage;
}

void OfflineHubBackButton::addListenerToBackButton(Sprite* toBeAddedTo)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            AudioMixer::getInstance()->playEffect(BACK_BUTTON_AUDIO_EFFECT);
            Director::getInstance()->replaceScene(OfflineHubScene::createScene());
            
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), toBeAddedTo);
}
