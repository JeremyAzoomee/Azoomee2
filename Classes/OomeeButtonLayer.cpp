#include "OomeeButtonLayer.h"
#include "extensions/cocos-ext.h"
#include "spine/spine.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "ChildDataProvider.h"
#include <AzoomeeCommon/Data/ParentDataProvider.h>
#include "HQHistoryManager.h"
#include "AudioMixer.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

USING_NS_CC;
using namespace Azoomee;
using namespace spine;


OomeeButtonLayer* OomeeButtonLayer::createOomeeLayer(int oomeeNumber)
{
    auto layer = OomeeButtonLayer::create();
    
    layer->displayedOomeeNumber = oomeeNumber;
    layer->animationsTillWave = 3;
    layer->addOomeeToLayer();
    
    return layer;
}

bool OomeeButtonLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void OomeeButtonLayer::playAnimation(std::string OomeeAnimation, bool loop)
{
    AudioMixer::getInstance()->stopOomeeEffect();
    AudioMixer::getInstance()->playOomeeIdleSounds(true);
    oomee->setAnimation(0, OomeeAnimation, false);
    AudioMixer::getInstance()->playOomeeEffect(ConfigStorage::getInstance()->getNameForOomee(displayedOomeeNumber), OomeeAnimation, true);
    
    if(loop)
        loopAnimation = OomeeAnimation;
    else
        loopAnimation = "";
}

void OomeeButtonLayer::playAnimationNoSound(std::string OomeeAnimation)
{
    oomee->setAnimation(0, OomeeAnimation, false);
    loopAnimation = "";
}

void OomeeButtonLayer::hideOomee()
{
    oomee->setOpacity(0);
}

//------------ PRIVATE FUNCTIONS---------------

void OomeeButtonLayer::addOomeeToLayer()
{
    std::string oomeeName = ConfigStorage::getInstance()->getNameForOomee(displayedOomeeNumber);
    std::string jsonFileName = StringUtils::format("res/oomees/%s.json", oomeeName.c_str());
    std::string atlasFileName = StringUtils::format("res/oomees/%s.atlas", oomeeName.c_str());
    
    oomee = SkeletonAnimation::createWithFile(jsonFileName, atlasFileName, 0.6f);
    oomee->setAnimation(0, ConfigStorage::getInstance()->getGreetingAnimation().c_str(), false);
    oomee->setScale(2);
    oomee->setOpacity(0);
    this->addChild(oomee);
    
    oomee->runAction(Sequence::create(FadeTo::create(0, 255), DelayTime::create(0.1), FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, 255), NULL));
    
    addCompleteListenerToOomee(oomee);
}

void OomeeButtonLayer::addCompleteListenerToOomee(spine::SkeletonAnimation* toBeAddedTo)
{
    auto oomeeAnimationComplete = [=] (int trackIdx, int loopCount)
    {
        if(loopAnimation != "")
        {
            toBeAddedTo->addAnimation(0, loopAnimation, false);
        }
        else if(animationsTillWave <=0)
        {
            animationsTillWave = 3;
            toBeAddedTo->addAnimation(0, "Build_Simple_Wave", false);
        }
        else
        {
            toBeAddedTo->addAnimation(0, ConfigStorage::getInstance()->getRandomIdForAnimationType("idle").c_str(), false);
            animationsTillWave--;
        }
    };
    
    toBeAddedTo->setCompleteListener(oomeeAnimationComplete);
}
