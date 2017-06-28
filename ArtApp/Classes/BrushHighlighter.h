//
//  BrushHighlighter.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/05/2017.
//
//

#ifndef BrushHighlighter_h
#define BrushHighlighter_h

#include "Brush.h"

class BrushHighlighter : public Brush
{
private:
    static float alphaLevel;
    cocos2d::Vec2 points[4];
    bool lineStarted;
public:
    BrushHighlighter();
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif /* BrushHighlighter_h */
