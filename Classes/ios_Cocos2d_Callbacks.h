#ifndef Azoomee_ios_Cocos2d_Callbacks_h
#define Azoomee_ios_Cocos2d_Callbacks_h

#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

#ifdef __cplusplus
extern "C" {
#endif
    
    void navigateToBaseScene();
    void navigateToLoginScene();
    void sendMixPanelData(const char* host, const char* query);
    const char* sendGameApiRequest(const char* method, const char* responseid, const char* score);
    const char* getLocalStorageForGame();
    void saveLocalStorageData(NSString* stringToSave);
    NSString* getVideoPlaylist();
    NSString* getRemoteWebGameAPIPath();
    bool isDeviceIphoneX();
    void favContent();
    void unFavContent();
    bool isFavContent();
    void shareContentInChat();
    bool isChatEntitled();
    bool isAnonUser();
    
    NSString* getPlaylistString();
    
#ifdef __cplusplus
}
#endif

NS_AZOOMEE_END

#endif
