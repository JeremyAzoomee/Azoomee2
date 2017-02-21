#include "OfflineHubSceneVisuals.h"

USING_NS_CC;

Scene* OfflineHubSceneVisuals::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineHubSceneVisuals::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineHubSceneVisuals::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void OfflineHubSceneVisuals::onEnter()
{
    addOomeesToScreen();
    
    Node::onEnter();
}

//All methods below this line are private methods------------------------------------------------------------------------------------------------------------------------------------------

void OfflineHubSceneVisuals::addOomeesToScreen()
{
    Point origin = Director::getInstance()->getVisibleOrigin();
    Size size = Director::getInstance()->getVisibleSize();
    
    std::vector<Point> startPositions = {Point(0, origin.y + size.height), Point(origin.x + size.width, 200)};
    std::vector<Point> endPositions = {Point(0, origin.y + size.height), Point(origin.x + size.width, 200)};
    
    for(int i = 0; i < 2; i++)
    {
        auto helperOomee = Sprite::create("res/offline/helper" + StringUtils::format("%d", i) + ".png");
        helperOomee->setScale(0.6);
        helperOomee->setPosition(startPositions[i]);
        this->addChild(helperOomee);
        
        helperOomee->runAction(Sequence::create(DelayTime::create(0.5 * CCRANDOM_0_1()), EaseElasticOut::create(MoveTo::create(0.5 + 0.5 * CCRANDOM_0_1(), endPositions[i])), NULL));
    }
    
}
