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
#include "AzoomeeArtApp.h"

NS_AZOOMEE_AA_BEGIN

class BrushEraser : public Brush
{
private:
    
public:
    BrushEraser();
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    
};

NS_AZOOMEE_AA_END

#endif /* BrushEraser_h */
