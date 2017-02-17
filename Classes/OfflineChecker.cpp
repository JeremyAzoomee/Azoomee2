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
    
    return _sharedOfflineChecker;
}

OfflineChecker::~OfflineChecker(void)
{
}

bool OfflineChecker::init(void)
{
    offlineStatus = false;
    this->startOfflineChecking();
    return true;
}

bool OfflineChecker::getOfflineStatus()
{
    return offlineStatus;
}

void OfflineChecker::startOfflineChecking()
{
    auto funcCallAction = CallFunc::create([=](){
        OfflineChecker::startOfflineChecking();
    });
    
    auto nodeToSchedule = Node::create();
    nodeToSchedule->runAction(RepeatForever::create(Sequence::create(funcCallAction, DelayTime::create(10), NULL)));
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
    if(response->getResponseCode() == 200)
    {
        offlineStatus = false;
    }
    else
    {
        offlineStatus = true;
    }
}
