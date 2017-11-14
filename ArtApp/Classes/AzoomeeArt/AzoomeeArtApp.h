//
//  AzoomeeArtApp.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 20/07/2017.
//
//

#ifndef AzoomeeArtApp_h
#define AzoomeeArtApp_h

#include <string>
#include <AzoomeeCommon/Audio/AudioMixer.h>

// Some convenience macros for namespace
#define NS_AZOOMEE_AA_BEGIN namespace Azoomee { namespace ArtApp {
#define NS_AZOOMEE_AA_END }}


NS_AZOOMEE_AA_BEGIN

// Burnt in version number for the AzoomeeArtApp static library
const char* const Version = "1.0";

static const std::string kArtAppAssetLoc = "res/artapp/";
static const std::string kStickerLoc = "res/chat/stickers/";

// Delegate is used to deal with navigating outside the app
struct Delegate {
    virtual void onArtAppNavigationBack() = 0;
    virtual void onArtAppShareImage() = 0;
    virtual std::string getFileName() const = 0;
    virtual void setFileName(const std::string& fileName) = 0;
    bool ArtAppRunning = false;
    std::string filename = "";
    // TODO: On error callbacks
};

// Delegate instance in namespace
extern Delegate* delegate;

NS_AZOOMEE_AA_END

#endif /* AzoomeeArtApp_h */
