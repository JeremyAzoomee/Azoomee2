//
//  BrushEraser.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 02/06/2017.
//
//

#ifndef BrushEraser_h
#define BrushEraser_h

#include "Brush.h"

class BrushEraser : public Brush
{
private:
    
public:
    BrushEraser();
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
};

#endif /* BrushEraser_h */
