//
//  BrushPen.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/05/2017.
//
//

#ifndef BrushPen_h
#define BrushPen_h

#include "Brush.h"

class BrushPen : public Brush
{
private:
    
public:
    
    BrushPen();
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
};

#endif /* BrushPen_h */
