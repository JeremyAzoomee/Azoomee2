//
//  RewardManager.h
//  AzoomeeCommon
//
//  Created by David Mitchell on 20/11/2019.
//

#ifndef RewardManager_h
#define RewardManager_h

#include "../../Azoomee.h"
#include "../../API/API.h"
#include <memory>
#include "../Json.h"
#include "RewardItem.h"

NS_AZOOMEE_BEGIN

class RewardManager : private HttpRequestCreatorResponseDelegate
{
public:
    static RewardManager* getInstance();
    virtual ~RewardManager();
    
    /// Get the latest reward strategy from the server
    void getLatestRewardStrategy();
    
    /// Check an API response for the existence of a reward
    void checkResponseForNewRewards(const std::string& requestTag, const std::string& headers);

private:
    // Private construction - Use ::getInstance()
    RewardManager();
    
    /// Called once the reward strategy has been successfully loaded
    void onRewardStrategyLoaded();
    /// Start redeeming rewards from _rewardsPendingRedemption
    void redeemPendingRewards();
    
    /// Handle the network response for reward feed
    void handleRewardFeedResponse(const std::string& headers, const std::string& body);
    /// New reward recieved
    void handleNewRewardResponse(const std::string& headers, const std::string& body);
    
    /// - HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
    
    /// Rewards pending redemption
    std::deque<RewardItemRef> _rewardsPendingRedemption;
    /// Rewards currently being redeemed
    std::map<std::string, RewardItemRef> _rewardsRedemptionInProgress;
    
    /// True once the reward strategy has been loaded
    bool _strategyLoaded = false;
    
    /// The min duration that must be reached in a piece of content before a reward is given
    int _minRewardDuration;
    /// The reward the user recieves once they reach _minRewardDuration
    int _minRewardCoinValue;
    /// How often the user recieves an addition reward after _minRewardDuration has been reached
    int _repeatRewardDuration;
    /// The reward the user recieves for each _repeatRewardDuration reached
    int _repeatRewardCoinValue;
    /// Overrides for _minRewardCoinValue per contentID
    std::map<std::string, int> _minRewardCoinValueContentOverride;
};

NS_AZOOMEE_END

#endif
