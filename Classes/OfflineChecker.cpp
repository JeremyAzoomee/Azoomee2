#include "OfflineChecker.h"
#include "BackEndCaller.h"

using namespace cocos2d;
using namespace network;

USING_NS_TZ

NS_AZ_BEGIN

static OfflineChecker *_sharedOfflineChecker = NULL;

OfflineChecker* OfflineChecker::getInstance()
{
    if (! _sharedOfflineChecker)
    {
        _sharedOfflineChecker = new OfflineChecker();
        _sharedOfflineChecker->init();
    }

    _sharedOfflineChecker->_newScene = true;
    _sharedOfflineChecker->startOfflineChecking();
    
    return _sharedOfflineChecker;
}

OfflineChecker::~OfflineChecker(void)
{
}

bool OfflineChecker::init(void)
{
    _offlineStatus = false;
    return true;
}

bool OfflineChecker::getOfflineStatus()
{
    return _offlineStatus;
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
    
    if(_newScene||_offlineStatus)
    {
        _amountOfFailures = 0;

        _offlineStatus = false;
        _newScene = false;
        
        if(this->getDelegate())
        {
            this->getDelegate()->connectivityStateChanged(true);
        }
        
    }
}

void OfflineChecker::onOfflineCheckFailed()
{
    if(_newScene||!_offlineStatus)
    {
        _amountOfFailures++;
        if(_amountOfFailures < 2) return;
        
        _offlineStatus = true;
        _newScene = false;
        if(this->getDelegate())
        {
            this->getDelegate()->connectivityStateChanged(false);
        }

    }
}

NS_AZ_END
