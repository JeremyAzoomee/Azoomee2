//
//  RewardManager.cpp
//  AzoomeeCommon
//
//  Created by David Mitchell on 20/11/2019.
//

#include "RewardManager.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

NS_AZOOMEE_BEGIN


#pragma mark - Init

static std::auto_ptr<RewardManager> sRewardManagerSharedInstance;

RewardManager* RewardManager::getInstance()
{
    if(!sRewardManagerSharedInstance.get())
    {
        sRewardManagerSharedInstance.reset(new RewardManager());
    }
    return sRewardManagerSharedInstance.get();
}

RewardManager::RewardManager()
{
}

RewardManager::~RewardManager()
{
}

void RewardManager::getLatestRewardStrategy()
{
    // 1. HEAD request.
    // 2. Check etag in response.
    // 3. If etag changed, GET request. If not changed, load from cache.
    // 4. Save feed response to cache.
    HttpRequestCreator* request = API::GetRewardStrategy("GET", this);
    request->execute();
}

#pragma mark - Public

void RewardManager::calculateRewardForContent(const HQContentItemObjectRef& content, long timeInContent)
{
    // Ref: https://azoomee.atlassian.net/wiki/spaces/API/pages/1268580353/Reward+Strategy
    
    if(timeInContent < _minRewardDuration)
    {
        return;
    }
    
    auto it = _rewardCoinValueContentOverride.find(content->getContentItemId());
    const int baseReward = (it != _rewardCoinValueContentOverride.end()) ? it->second : _defaultRewardCoinValue;
    
    long bonusDuration = timeInContent - _minRewardDuration;
    long bonusReward = floor(bonusDuration * ((float)_repeatRewardCoinValue / (float)_repeatRewardDuration) );
    int fullReward = baseReward + (int)bonusReward;
    
    const RewardItemRef& reward = RewardItem::createCoinReward(fullReward);
    
    // Do we already have a coin reward pending we can append these coins to?
    if(_rewardCoinNotification)
    {
        _rewardCoinNotification->mergeRewards(reward);
    }
    else
    {
        _rewardCoinNotification = reward;
        _rewardNotifications.push_back(reward);
    }
}

size_t RewardManager::pendingRewardNotificationCount() const
{
    return _rewardNotifications.size();
}

RewardItemRef RewardManager::popPendingRewardNotification()
{
    if(_rewardNotifications.empty())
    {
        return nullptr;
    }
    
    RewardItemRef front = _rewardNotifications.front();
    _rewardNotifications.pop_front();
    
    if(front == _rewardCoinNotification)
    {
        _rewardCoinNotification = nullptr;
    }
    
    return front;
}

void RewardManager::checkResponseForNewRewards(const std::string& requestTag, const std::string& headers)
{
    const std::string& rewardData = StringFunctions::getValueFromHttpResponseHeaderForKey("X-AZ-REWARDS", headers);
    if(!rewardData.empty())
    {
        const std::vector<std::string>& urls = StringFunctions::splitStringToVector(rewardData, ";");
        for(const std::string& url : urls)
        {
            HttpRequestCreator* request = API::RewardCallback(url, this);
            request->execute();
        }
    }
}

void RewardManager::checkForPendingRewards()
{
    HttpRequestCreator* request = API::GetPendingRewards(ChildManager::getInstance()->getLoggedInChild()->getId(), this);
    request->execute();
}

#pragma mark - Private

void RewardManager::onRewardStrategyLoaded()
{
    _strategyLoaded = true;
}

void RewardManager::redeemPendingRewards()
{
    if(_rewardsPendingRedemption.size() == 0)
    {
        return;
    }
    
    const RewardItemRef& reward = _rewardsPendingRedemption.front();
    const std::string& rewardId = reward->getId();
    const int rewardPrice = reward->getItemPrice();
    _rewardsRedemptionInProgress[rewardId] = reward;
    _rewardsPendingRedemption.pop_front();
    
    auto onSuccess = [rewardId, rewardPrice, this](const std::string& requestTag, const std::string& headers, const std::string& body) {
        _rewardsRedemptionInProgress.erase(rewardId);
        AnalyticsSingleton::getInstance()->rewardRedeemedEvent(abs(rewardPrice));
        
        // Any more rewards to redeem?
        if(!_rewardsPendingRedemption.empty())
        {
            redeemPendingRewards();
        }
        // Have we finished all in progress redemptions?
        else if(_rewardsRedemptionInProgress.empty())
        {
            // Get latest inventory now all rewards have been redeemed
            ChildManager::getInstance()->updateInventory();
        }
    };
    
    auto onFailure = [](const std::string& requestTag, long errorCode) {
        // TODO: Properly handle failure because like this queue will get stuck
        //       We should probably retry, and also make sure child login triggers a full redemption attempt
        //       In addition, on failure should do a get inventory in any case
    };
    
    HttpRequestCreator* request = API::RedeemReward(rewardId, onSuccess, onFailure);
    request->execute();
}

void RewardManager::addPendingRewardToRedemptionQueue(const RewardItemRef& reward)
{
    if(reward && reward->getStatus() == "PENDING")
    {
        _rewardsPendingRedemption.push_back(reward);
        
        // Start redeeming if we're not already
        if(_rewardsPendingRedemption.size() == 1)
        {
            redeemPendingRewards();
        }
    }
}

void RewardManager::handleRewardFeedResponse(const std::string& headers, const std::string& body)
{
    /**
    {
        "strategies": [
            {
                "id": "956bd3a6-ef42-45fd-9630-5e08e735eba8",
                "rewardType": "DURATION",
                "data": {
                    "minTime": 120,
                    "minReward": {
                        "type": "COIN",
                        "value": 10
                    },
                    "repeatTime": 60,
                    "repeatReward": {
                        "type": "COIN",
                        "value": 1
                    }
                }
            }
        ],
        "content": {
            "epis_df6da0db978545e7a1303e9d4ef54c8b": {
                "price": -10
            },
            "epis_f21d2d481446426fb338a7aec87aad30": {
                "price": -10
            },
            "9275fcca-a88e-4583-b0ab-07d6a7b2566d": {
                "price": -10
            }
        }
    }**/
    
    // Reset rewards values before we parse the latest data
    _defaultRewardCoinValue = 0;
    _minRewardDuration = 0;
    _repeatRewardDuration = 0;
    _repeatRewardCoinValue = 0;
    _rewardCoinValueContentOverride.clear();
    
    
    // Parse the json
    rapidjson::Document json;
    json.Parse(body.c_str());
    
    // We must have a strategies entry to be able to do anything
    if(json.HasParseError() || !json.HasMember("strategies"))
    {
        // TODO: Handle parsing error for rewards feed
        return;
    }
    
    // Strategies
    
    // strategies contains the default reward logic
    const rapidjson::Value& strategies = json["strategies"];
    if(!strategies.IsArray())
    {
        // TODO: Handle parsing error for rewards feed
        return;
    }
    
    for(int i = 0; i < strategies.Size(); i++)
    {
        const rapidjson::Value& item = strategies[i];
        const std::string& rewardType = getStringFromJson("rewardType", item);
        if(rewardType == "DURATION" && item.HasMember("data"))
        {
            const rapidjson::Value& strategyData = item["data"];
            
            // Need to at least have minReward data, but repeat is optional (we assume it's 0)
            if(!strategyData.HasMember("minReward") || !strategyData.HasMember("minTime"))
            {
                continue;
            }
            
            const int minTime = getIntFromJson("minTime", strategyData, 0);
            const rapidjson::Value& minReward = strategyData["minReward"];
            const std::string& minRewardType = getStringFromJson("type", minReward);
            const int minRewardValue = getIntFromJson("value", minReward);
            
            // For now we only recognise coin rewards, so if this isn't one, ignore it
            if(minRewardType != "COIN")
            {
                continue;
            }
            
            const int repeatTime = getIntFromJson("repeatTime", strategyData, 0);
            int repeatRewardValue = 0;
            if(strategyData.HasMember("repeatReward"))
            {
                const rapidjson::Value& repeatReward = strategyData["repeatReward"];
                const std::string& repeatRewardType = getStringFromJson("type", repeatReward);
                if(repeatRewardType == "COIN")
                {
                    repeatRewardValue = getIntFromJson("value", repeatReward, 0);
                }
            }
            
            // Values are negative so pass it through abs to make sure we have a positive value for the reward
            _defaultRewardCoinValue = abs(minRewardValue);
            _minRewardDuration = minTime;
            _repeatRewardCoinValue = abs(repeatRewardValue);
            _repeatRewardDuration = repeatTime;
        }
    }
    
    // Content
    
    // content contains override values for minRewardValue per contentID
    // We consider the overrides optional so we check for the existence first
    if(json.HasMember("content"))
    {
        const rapidjson::Value& contentOverrides = json["content"];
        if(!contentOverrides.IsObject())
        {
            // TODO: Handle parsing error for rewards feed
            return;
        }
        
        for(auto it = contentOverrides.MemberBegin(); it != contentOverrides.MemberEnd(); ++it)
        {
            const std::string& key = it->name.GetString();
            const rapidjson::Value& value = it->value;
            if(value.IsObject() && value.HasMember("price"))
            {
                int price = getIntFromJson("price", value, 0);
                // Values are negative so pass it through abs to make sure we have a positive value for the reward
                price = abs(price);
                if(price > 0)
                {
                    _rewardCoinValueContentOverride[key] = price;
                }
            }
        }
    }
    
    onRewardStrategyLoaded();
}

void RewardManager::handleNewRewardResponse(const std::string& headers, const std::string& body)
{
    rapidjson::Document data;
    data.Parse(body.c_str());
    if(data.HasParseError())
    {
        return;
    }
    
    const RewardItemRef& reward = RewardItem::createWithJson(data);
    addPendingRewardToRedemptionQueue(reward);
}

void RewardManager::handlePendingRewardsResponse(const std::string& headers, const std::string& body)
{
    rapidjson::Document data;
    data.Parse(body.c_str());
    if(data.HasParseError())
    {
        return;
    }
    
    if(data.IsArray() && data.Size() > 0)
    {
        for(int i = 0; i < data.Size(); ++i)
        {
            const rapidjson::Value& rewardData = data[i];
            const RewardItemRef& reward = RewardItem::createWithJson(rewardData);
            addPendingRewardToRedemptionQueue(reward);
        }
    }
}

#pragma mark - HttpRequestCreatorResponseDelegate

void RewardManager::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    const static std::string rewardStrategyTagHead = cocos2d::StringUtils::format("%s-%s", API::TagGetRewardStrategy, "HEAD");
    const static std::string rewardStrategyTagGet = cocos2d::StringUtils::format("%s-%s", API::TagGetRewardStrategy, "GET");
    
    if(requestTag == rewardStrategyTagHead)
    {
        // TODO: Handle get rewards feed head response
    }
    else if(requestTag == rewardStrategyTagGet)
    {
        handleRewardFeedResponse(headers, body);
    }
    else if(requestTag == API::TagRewardCallback)
    {
        handleNewRewardResponse(headers, body);
    }
    else if(requestTag == API::TagGetPendingRewards)
    {
        handlePendingRewardsResponse(headers, body);
    }
}

void RewardManager::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    cocos2d::log("RewardManager::onHttpRequestFailed: %s, errorCode=%ld", requestTag.c_str(), errorCode);
}



NS_AZOOMEE_END
