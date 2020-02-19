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
#include "AzoomeeArtApp.h"

USING_NS_TZ

NS_AZ_AA_BEGIN

class BrushPen : public Brush
{
private:
    
public:
    
    BrushPen();
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    
};

NS_AZ_AA_END

#endif /* BrushPen_h */
