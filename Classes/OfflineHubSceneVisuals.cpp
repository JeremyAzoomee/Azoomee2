#include "OfflineHubSceneVisuals.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    
    std::vector<Point> startPositions = {Point(-220, origin.y + size.height - 650), Point(origin.x + size.width + 160, origin.y + size.height - 650)};
    std::vector<Point> endPositions = {Point(157, origin.y + size.height - 450), Point(origin.x + size.width - 144, origin.y + size.height - 450)};
    
    for(int i = 0; i < 2; i++)
    {
        auto helperOomee = Sprite::create("res/offline/helper" + StringUtils::format("%d", i) + ".png");
        helperOomee->setPosition(startPositions[i]);
        this->addChild(helperOomee);
        
        helperOomee->runAction(Sequence::create(DelayTime::create(0.5 * CCRANDOM_0_1()), EaseElasticOut::create(MoveTo::create(0.5 + 0.5 * CCRANDOM_0_1(), endPositions[i])), NULL));
    }
    
}

NS_AZOOMEE_END
