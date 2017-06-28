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

class BrushSprayCan : public Brush
{
private:
    
public:
    BrushSprayCan();
    
    cocos2d::Node* addDrawNode(cocos2d::Size visibleSize);
    cocos2d::Node* getDrawNode();
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif /* BrushSprayCan_h */
