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
#include "AzoomeeArtApp.h"

NS_AZOOMEE_AA_BEGIN

class BrushPaintBrush: public Brush
{
private:
    static float startingAlpha;
    float lastAlpha;
public:
    BrushPaintBrush();
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
};

NS_AZOOMEE_AA_END

#endif /* BrushPaintBrush_h */
