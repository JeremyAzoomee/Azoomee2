//
//  AzoomeeArtApp.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 20/07/2017.
//
//

#ifndef AzoomeeArtApp_h
#define AzoomeeArtApp_h

// Some convenience macros for namespace
#define NS_AZOOMEE_AA_BEGIN namespace Azoomee { namespace ArtApp {
#define NS_AZOOMEE_AA_END }}


NS_AZOOMEE_AA_BEGIN

// Burnt in version number for the AzoomeeArtApp static library
const char* const Version = "0.1";


// Delegate is used to deal with navigating outside the chat app
struct Delegate {
    virtual void onArtAppNavigationBack() = 0;
    
    // TODO: On error callbacks
};

// Delegate instance in namespace
extern Delegate* delegate;

NS_AZOOMEE_AA_END

#endif /* AzoomeeArtApp_h */
