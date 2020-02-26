//
//  ResizeNode.h
//  AzoomeeCommon
//
//  Created by Macauley on 18/03/2019.
//

#ifndef ResizeNode_h
#define ResizeNode_h

#include "../Tinizine.h"
#include <cocos/cocos2d.h>

class ResizeNode : public cocos2d::Node
{
	typedef cocos2d::Node Super;
	
public:
	virtual void onSizeChanged() = 0;
};

#endif /* ResizeNode_h */
