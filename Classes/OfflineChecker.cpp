#include "OfflineChecker.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>

using namespace cocos2d;
using namespace network;

NS_AZOOMEE_BEGIN

static OfflineChecker *_sharedOfflineChecker = NULL;

OfflineChecker* OfflineChecker::getInstance()
{
    if (! _sharedOfflineChecker)
    {
        _sharedOfflineChecker = new OfflineChecker();
        _sharedOfflineChecker->init();
    }
    
    
    CCLOG("Newscene set to true");
    _sharedOfflineChecker->newScene = true;
    _sharedOfflineChecker->startOfflineChecking();
    
    return _sharedOfflineChecker;
}

OfflineChecker::~OfflineChecker(void)
{
}

bool OfflineChecker::init(void)
{
    offlineStatus = false;
    return true;
}

bool OfflineChecker::getOfflineStatus()
{
    return offlineStatus;
}

void OfflineChecker::startOfflineChecking()
{
    if(Director::getInstance()->getRunningScene()->getChildByName("scheduleNode")) return;
    
    auto funcCallAction = CallFunc::create([=](){
        OfflineChecker::sendOfflineCheckRequest();
    });
    funcCallAction->setTag(1);
    
    auto nodeToSchedule = Node::create();
    nodeToSchedule->setName("scheduleNode");
    Director::getInstance()->getRunningScene()->addChild(nodeToSchedule);
    
    nodeToSchedule->stopActionByTag(1);
    nodeToSchedule->runAction(RepeatForever::create(Sequence::create(funcCallAction, DelayTime::create(4), NULL)));
}

//ALL functions below this line are private

void OfflineChecker::sendOfflineCheckRequest()
{
    BackEndCaller::getInstance()->offlineCheck();
}

void OfflineChecker::onOfflineCheckAnswerReceived()
{
    if(!Director::getInstance()->getRunningScene()->getChildByName("scheduleNode")) return;
    
    if(newScene||offlineStatus)
    {
        amountOfFailures = 0;
        CCLOG("Online!");
        offlineStatus = false;
        newScene = false;
        
        ChildDataParser::getInstance()->setChildLoggedIn(false);
        
        if(this->getDelegate())
        {
            this->getDelegate()->connectivityStateChanged(true);
        }
        
    }
}

void OfflineChecker::onOfflineCheckFailed()
{
    if(newScene||!offlineStatus)
    {
        amountOfFailures++;
        if(amountOfFailures < 2) return;
        
        CCLOG("Offline!");
        offlineStatus = true;
        newScene = false;
        if(this->getDelegate())
        {
            this->getDelegate()->connectivityStateChanged(false);
        }

    }
}

NS_AZOOMEE_END
