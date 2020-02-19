//
//  AzoomeeOomeeMaker.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 14/02/2018.
//

#ifndef AzoomeeOomeeMaker_h
#define AzoomeeOomeeMaker_h

//#define STANDALONE_APP YES

#define NS_AZ_OM_BEGIN namespace AZ { namespace OomeeMaker {
#define NS_AZ_OM_END }}

#include <string>
#include <AzoomeeCommon/Tinizine.h>

USING_NS_TZ

NS_AZ_OM_BEGIN

// Delegate is used to deal with navigating outside the oomee maker app
struct Delegate {
    virtual void onOomeeMakerNavigationBack() = 0;
    virtual void onOomeeMakerShareOomee(const std::string& filename) = 0;
    virtual void onOomeeMakerUpdateAvatar(const std::string& filename) = 0;
	
	std::string _newAccessoryId = "";
};

// Delegate instance in namespace
extern Delegate* delegate;

NS_AZ_OM_END

#endif /* AzoomeeOomeeMaker_h */
