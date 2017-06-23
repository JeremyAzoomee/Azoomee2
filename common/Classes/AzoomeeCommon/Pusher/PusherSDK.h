#ifndef AzoomeeCommon_PusherSDK_h
#define AzoomeeCommon_PusherSDK_h

#include "../Azoomee.h"
#include <string>


NS_AZOOMEE_BEGIN

/**
 * Cross platform class for interacting with the Pusher SDK on the local platform.
 * Under the hood this class makes calls to the iOS or Android native SDK.
 */
class PusherSDK
{
private:
    
    /// Private construction - use getInstance()
    PusherSDK();
    
    
public:
    
    /// Returns the shared instance of this Pusher class
    static PusherSDK* getInstance();
    
    // TODO: Observer event callbacks
    // void addObserver();
    // void removeObserver();
    
    /// Open the account wide pusher channel
    void openParentAccountChannel();
    /// Close the account wide pusher channel
    void closeParentAccountChannel();
    
    // - Platform independant, see Pusher_android.cpp & Pusher_ios.mm
    
    /// Subscribe to a channel
    void subscribeToChannel(const std::string& channelName);
    
    /// Close all channel subscriptions
    void closeAllChannels();
    /// Close a specific channel if it is open
    void closeChannel(const std::string& channelName);
    
};

NS_AZOOMEE_END

#endif
