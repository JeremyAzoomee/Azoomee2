#include "NativeShare.h"
#import <UIKit/UIKit.h>
#include "../Platform/iOS/AzoomeeViewController.h"

USING_NS_CC;

namespace Azoomee
{

void nativeShareScreenString(const std::string& shareString)
{
    AzoomeeViewController* rootViewController =  (AzoomeeViewController*)[UIApplication sharedApplication].keyWindow.rootViewController;

    NSArray *objectsToShare = @[[NSString stringWithUTF8String:shareString.c_str()]];
    
    UIActivityViewController *activityVC = [[UIActivityViewController alloc] initWithActivityItems:objectsToShare applicationActivities:nil];
    
    NSArray *excludeActivities = @[UIActivityTypeAirDrop,
                                   UIActivityTypePrint,
                                   UIActivityTypeAssignToContact,
                                   UIActivityTypeSaveToCameraRoll,
                                   UIActivityTypeAddToReadingList,
                                   UIActivityTypePostToFlickr,
                                   UIActivityTypePostToVimeo];
    
    activityVC.excludedActivityTypes = excludeActivities;
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
        [rootViewController presentViewController:activityVC animated:YES completion:nil];
    }
    else {
        activityVC.modalPresentationStyle = UIModalPresentationPopover;
        
        UIPopoverPresentationController *presentationController = (UIPopoverPresentationController*)activityVC.presentationController;
        presentationController.sourceView = rootViewController.view;
        presentationController.sourceRect = CGRectMake(CGRectGetMidX(rootViewController.view.bounds), rootViewController.view.bounds.size.height*.66,0,0);
        presentationController.permittedArrowDirections = UIPopoverArrowDirectionUp;
        
        [rootViewController presentViewController:activityVC animated:YES completion:nil];
    }
}
}
