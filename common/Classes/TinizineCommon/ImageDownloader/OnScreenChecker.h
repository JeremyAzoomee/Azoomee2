//
//  OnScreenChecker.h
//  TinizineCommon
//
//  Created by Macauley on 27/07/2017.
//
//

#ifndef OnScreenChecker_h
#define OnScreenChecker_h

#include <cocos/cocos2d.h>
#include "../Tinizine.h"

NS_TZ_BEGIN

class OnScreenChecker
{
public:
    static bool kUseStrictBoundry;
    
	virtual ~OnScreenChecker();
	
    void startCheckingForOnScreenPosition(cocos2d::Node* sender);
    bool checkIfElementIsOnScreen(cocos2d::Node *item);
    void stopCheckingOnScreenPosition();
    
protected:
    virtual void elementDisappeared(cocos2d::Node *sender);
    virtual void elementAppeared(cocos2d::Node *sender);
    bool elementOnScreen;
};

NS_TZ_END
#endif /* OnScreenChecker_h */
