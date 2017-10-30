//
//  BrushSprayCan.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 31/05/2017.
//
//

#ifndef BrushSprayCan_h
#define BrushSprayCan_h

#include "Brush.h"
#include <time.h>
#include "AzoomeeArtApp.h"

NS_AZOOMEE_AA_BEGIN

class BrushSprayCan : public Brush
{
private:
    int _drawnElementsCount;
public:
    BrushSprayCan();
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
};

NS_AZOOMEE_AA_END

#endif /* BrushSprayCan_h */
