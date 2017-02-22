#include "OfflineChecker.h"

USING_NS_CC;

using namespace network;
using namespace cocos2d;

static OfflineChecker *_sharedOfflineChecker = NULL;

OfflineChecker* OfflineChecker::getInstance()
{
    if (! _sharedOfflineChecker)
    {
        _sharedOfflineChecker = new OfflineChecker();
        _sharedOfflineChecker->init();
    }
    
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
    nodeToSchedule->runAction(RepeatForever::create(Sequence::create(funcCallAction, DelayTime::create(5), NULL)));
}

//ALL functions below this line are private

void OfflineChecker::sendOfflineCheckRequest()
{
    auto jsonRequest = new HttpRequest();
    jsonRequest->setRequestType(HttpRequest::Type::GET);
    jsonRequest->setUrl("https://api.azoomee.com/api/comms/heartbeat");
    
    jsonRequest->setResponseCallback(CC_CALLBACK_2(OfflineChecker::onOfflineCheckRequestAnswerReceived, this));
    jsonRequest->setTag("offlineCheck");
    HttpClient::getInstance()->setTimeoutForConnect(1);
    HttpClient::getInstance()->setTimeoutForRead(1);
    HttpClient::getInstance()->send(jsonRequest);
}

void OfflineChecker::onOfflineCheckRequestAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if((response->getResponseCode() == 200)&&(offlineStatus))
    {
        CCLOG("Online!");
        offlineStatus = false;
        
        if(this->getDelegate())
        {
            this->getDelegate()->connectivityStateChanged(true);
        }
        
    }
    
    if((response->getResponseCode() != 200)&&(!offlineStatus))
    {
        CCLOG("Offline!");
        offlineStatus = true;
        if(this->getDelegate())
        {
            this->getDelegate()->connectivityStateChanged(false);
        }
    }
}
