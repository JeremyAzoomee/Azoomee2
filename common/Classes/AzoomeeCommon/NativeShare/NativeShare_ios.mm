#include "NativeShare.h"
#import <UIKit/UIKit.h>
#include "../../../proj.ios_mac/ios/RootViewController.h"

USING_NS_CC;

namespace Azoomee
{

void shareKidCode()
{
    RootViewController* rootViewController =  (RootViewController*)[UIApplication sharedApplication].keyWindow.rootViewController;
    
    NSString *textToShare = @"Look at this awesome website for aspiring iOS Developers!\n\n";
    NSURL *myWebsite = [NSURL URLWithString:@"http://www.azoomee.com/"];
    
    NSArray *objectsToShare = @[textToShare, myWebsite];
    
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
        presentationController.sourceRect = CGRectMake(CGRectGetMidX(rootViewController.view.bounds), CGRectGetMidY(rootViewController.view.bounds),0,0);
        presentationController.permittedArrowDirections = UIPopoverArrowDirectionUp;
        
        [rootViewController presentViewController:activityVC animated:YES completion:nil];
    }
}
  
}
