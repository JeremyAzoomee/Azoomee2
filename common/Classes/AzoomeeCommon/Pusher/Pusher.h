#ifndef AzoomeeCommon_Pusher_h
#define AzoomeeCommon_Pusher_h

#include "../Azoomee.h"
#include <string>


NS_AZOOMEE_BEGIN

/**
 * Cross platform class for interacting with the Pusher SDK on the local platform.
 * Under the hood this class makes calls to the iOS or Android native SDK.
 */
class Pusher
{
private:
    
    /// Private construction - use getInstance()
    Pusher();
    
    
public:
    
    /// Returns the shared instance of this Pusher class
    static Pusher* getInstance();
    
    // TODO: Observer event callbacks
    // void addObserver();
    // void removeObserver();
    
    
    // - Platform independant, see Pusher_android.cpp & Pusher_ios.mm
    
    /// Subscribe to a channel
    void subscribeToChannel(const std::string& channeName);
    
    /// Close all channel subscriptions
    void closeAllChannels();
    /// Close a specific channel if it is open
    void closeChannel(const std::string& channeName);
    
};

NS_AZOOMEE_END

#endif
