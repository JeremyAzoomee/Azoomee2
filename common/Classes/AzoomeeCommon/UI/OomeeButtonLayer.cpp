#include "OomeeButtonLayer.h"
#include <extensions/cocos-ext.h>
#include <spine/spine.h>
#include "../Data/ConfigStorage.h"
#include "../Data/Child/ChildDataProvider.h"
#include "../Data/Parent/ParentDataProvider.h"
#include "../Audio/AudioMixer.h"

USING_NS_CC;
using namespace spine;

namespace Azoomee
{

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
void OomeeButtonLayer::setupOomeeSpine()
{
    std::string oomeeName = ConfigStorage::getInstance()->getNameForOomee(displayedOomeeNumber);
    std::string jsonFileName = StringUtils::format("res/oomees/%s_Button.json", oomeeName.c_str());
    std::string atlasFileName = StringUtils::format("res/oomees/%s.atlas", oomeeName.c_str());
    
    oomee = SkeletonAnimation::createWithJsonFile(jsonFileName, atlasFileName);
    oomee->setAnimation(0, ConfigStorage::getInstance()->getGreetingAnimation().c_str(), false);
    oomee->setScale(1.2);
    oomee->setOpacity(0);
    this->addChild(oomee);
    
    oomee->runAction(Sequence::create(FadeTo::create(0, 255), DelayTime::create(0.1), FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, 255), NULL));
    
    addCompleteListenerToOomee(oomee);
}
    
void OomeeButtonLayer::addOomeeToLayer()
{
    
    auto scheduler = Director::getInstance()->getScheduler();
    
    scheduler-> performFunctionInCocosThread(CC_CALLBACK_0(OomeeButtonLayer::setupOomeeSpine, this));
}

void OomeeButtonLayer::addCompleteListenerToOomee(spine::SkeletonAnimation* toBeAddedTo)
{
    
    auto oomeeAnimationComplete = [=] (spTrackEntry* entry)
    {
        if(loopAnimation != "")
        {
            toBeAddedTo->setAnimation(0, loopAnimation, false);
        }
        else if(animationsTillWave <=0)
        {
            animationsTillWave = 3;
            toBeAddedTo->setAnimation(0, "Build_Simple_Wave", false);
        }
        else
        {
            toBeAddedTo->setAnimation(0, ConfigStorage::getInstance()->getRandomIdForAnimationType("button").c_str(), false);
            animationsTillWave--;
        }
    };
    
    toBeAddedTo->setCompleteListener(oomeeAnimationComplete);
}
  
}
