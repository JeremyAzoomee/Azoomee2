#include "ModalMessages.h"

USING_NS_CC;

#define MESSAGE_BOX_WIDTH 1366
#define MESSAGE_BOX_HEIGHT 1024
#define MESSAGE_BOX_PADDING 100

Scene* ModalMessages::createScene()
{
    auto scene = Scene::create();
    auto layer = ModalMessages::create();
    scene->addChild(layer);

    return scene;
}

bool ModalMessages::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    return true;
}

void ModalMessages::createLayer()
{
    loadingLayer = LayerColor::create(Color4B(0,0,0,150), visibleSize.width, visibleSize.height);
    loadingLayer->setPosition(origin.x, origin.y);
    loadingLayer->setOpacity(0);
    this->addChild(loadingLayer);
    
    addListenerToBackgroundLayer();
}

void ModalMessages::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), loadingLayer);
}

void ModalMessages::startLoading()
{
    createLayer();
    loadingLayer->runAction(FadeTo::create(0.5, 200));
    
    //ADDING SOME TEMPORARY CIRCLES AS LOADING, NOT FINAL YET
    
    for(int i = 0; i < 3; i++)
    {
        auto loadingCircle = Sprite::create("res/modal/loading.png");
        loadingCircle->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        loadingCircle->setOpacity(0);
        loadingCircle->setRotation(RandomHelper::random_int(0, 360));
        loadingCircle->setScale(0.6 + i * 0.2);
        
        loadingLayer->addChild(loadingCircle);
        
        int direction = 1;
        if(CCRANDOM_0_1() < 0.5) direction = -1;
        
        loadingCircle->runAction(RepeatForever::create(RotateBy::create(CCRANDOM_0_1() + 1, 360 * direction)));
        loadingCircle->runAction(FadeTo::create(0.5, 255));
    }
}

void ModalMessages::stopLoading()
{
    if(loadingLayer) //This might be called when loading is not active, so better to check first
    {
        this->removeChild(loadingLayer);
    }
}



void ModalMessages::createMessageWithSingleButton(std::string messageTitle, std::string messageBody, std::string buttonText)
{
    createLayer();
    
    auto messageBox = LayerColor::create((Color4B::WHITE), MESSAGE_BOX_WIDTH, MESSAGE_BOX_HEIGHT);
    messageBox->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    loadingLayer->addChild(messageBox);
    
    auto titleTextField = Label::createWithTTF(messageTitle, "fonts/azoomee.ttf", 90);
    titleTextField->setWidth(MESSAGE_BOX_WIDTH - MESSAGE_BOX_PADDING * 2);
    titleTextField->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    titleTextField->setColor(Color3B(28, 244, 244));
    messageBox->addChild(titleTextField);
    
    loadingLayer->runAction(FadeTo::create(0.5, 200));
}

void ModalMessages::createMessageBox()
{
    auto messageBox = LayerColor::create((Color4B::WHITE), MESSAGE_BOX_WIDTH, MESSAGE_BOX_HEIGHT);
    messageBox->setPosition(origin.x, origin.y);
    messageBox->setOpacity(0);
    loadingLayer->addChild(messageBox);
}

void ModalMessages::createMessageTitle(std::string messageTitle)
{
    auto titleTextField = Label::createWithTTF(messageTitle, "fonts/azoomee.ttf", 90);
    titleTextField->setWidth(MESSAGE_BOX_WIDTH - MESSAGE_BOX_PADDING * 2);
    titleTextField->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    titleTextField->setColor(Color3B(28, 244, 244));
    loadingLayer->addChild(titleTextField);
}

void ModalMessages::createMessageBody(std::string messageBody)
{
    
}

void ModalMessages::createButton(std::string buttonText)
{
    
}
