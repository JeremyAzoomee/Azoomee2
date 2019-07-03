//
//  OnScreenChecker.h
//  AzoomeeCommon
//
//  Created by Macauley on 27/07/2017.
//
//

#ifndef OnScreenChecker_h
#define OnScreenChecker_h

#include <cocos/cocos2d.h>

namespace Azoomee {

class OnScreenChecker
{
public:
    void startCheckingForOnScreenPosition(cocos2d::Node* sender);
    bool checkIfElementIsOnScreen(cocos2d::Node *item);
    void endCheck();
    
protected:
    virtual void elementDisappeared(cocos2d::Node *sender);
    virtual void elementAppeared(cocos2d::Node *sender);
    bool elementOnScreen;
};

}
#endif /* OnScreenChecker_h */
