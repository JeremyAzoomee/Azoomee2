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
#include "../HQDataObject/HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class RewardManager : private HttpRequestCreatorResponseDelegate
{
public:
    static RewardManager* getInstance();
    virtual ~RewardManager();
    
    /// Get the latest reward strategy from the server
    void getLatestRewardStrategy();
    /// Redeems any pending rewards on the server. The user won't recieve any notification but the inventory will update
    void checkForPendingRewards();
    
    /// Calculate the reward for a user after watching or playing a piece of content
    void calculateRewardForContent(const HQContentItemObjectRef& content, long timeInContent);
    /// Does the user have any pending reward notifications?
    size_t pendingRewardNotificationCount() const;
    /// Pop the oldest reward notification off the stack
    /// If the stack is empty, nullptr is returned
    RewardItemRef popPendingRewardNotification();
    
    /// Check an API response for the existence of a reward
    void checkResponseForNewRewards(const std::string& requestTag, const std::string& headers);

private:
    // Private construction - Use ::getInstance()
    RewardManager();
    
    /// Called once the reward strategy has been successfully loaded
    void onRewardStrategyLoaded();
    /// Start redeeming rewards from _rewardsPendingRedemption
    void redeemPendingRewards();
    /// Add a reward to be redeemed. This will start the redemption queue if it hasn't already
    void addPendingRewardToRedemptionQueue(const RewardItemRef& reward);
    
    /// Handle the network response for reward feed
    void handleRewardFeedResponse(const std::string& headers, const std::string& body);
    /// New reward recieved
    void handleNewRewardResponse(const std::string& headers, const std::string& body);
    /// Pending rewards recieved
    void handlePendingRewardsResponse(const std::string& headers, const std::string& body);
    
    /// - HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
    /// Rewards calculated locally but not yet shown to the user
    std::deque<RewardItemRef> _rewardNotifications;
    
    /// Rewards pending redemption
    std::deque<RewardItemRef> _rewardsPendingRedemption;
    /// Rewards currently being redeemed
    std::map<std::string, RewardItemRef> _rewardsRedemptionInProgress;
    
    
    // - Variables
    
    /// True once the reward strategy has been loaded
    bool _strategyLoaded = false;
    
    /// The min duration that must be reached in a piece of content before a reward is given
    int _minRewardDuration;
    /// The reward the user receives once they reach _minRewardDuration
    int _defaultRewardCoinValue;
    /// How often the user receives an addition reward after _minRewardDuration has been reached
    int _repeatRewardDuration;
    /// The reward the user receives for each _repeatRewardDuration reached
    int _repeatRewardCoinValue;
    /// Overrides for _defaultRewardCoinValue per contentID
    std::map<std::string, int> _rewardCoinValueContentOverride;
};

NS_AZOOMEE_END

#endif
