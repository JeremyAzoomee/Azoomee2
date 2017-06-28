//
//  BrushPaintBrush.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/05/2017.
//
//

#ifndef BrushPaintBrush_h
#define BrushPaintBrush_h

#include "Brush.h"

class BrushPaintBrush: public Brush
{
private:
    static float startingAlpha;
    float lastAlpha;
public:
    BrushPaintBrush();
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif /* BrushPaintBrush_h */
