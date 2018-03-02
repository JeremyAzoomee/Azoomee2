//
//  AzoomeeOomeeMaker.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 14/02/2018.
//

#ifndef AzoomeeOomeeMaker_h
#define AzoomeeOomeeMaker_h

#define STANDALONE_APP YES

#define NS_AZOOMEE_OM_BEGIN namespace Azoomee { namespace OomeeMaker {
#define NS_AZOOMEE_OM_END }}

NS_AZOOMEE_OM_BEGIN

// Delegate is used to deal with navigating outside the oomee maker app
struct Delegate {
    virtual void onOomeeMakerNavigationBack() = 0;
};

// Delegate instance in namespace
extern Delegate* delegate;

NS_AZOOMEE_OM_END

#endif /* AzoomeeOomeeMaker_h */
