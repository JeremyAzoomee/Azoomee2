//
//  Brush.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/05/2017.
//
//

#ifndef Brush_h
#define Brush_h

#include "cocos2d.h"
#include "Constants.h"
#include <vector>

enum BrushType{PEN,PAINTBRUSH,HIGHLIGHTER,SPRAYCAN,ROLLER,ERASER,NUM_BRUSHES};

class Brush
{
protected:
    cocos2d::DrawNode* drawNode;
    cocos2d::Vec2 lastTouchPos;
    cocos2d::Color4F* selectedColour;
    float* brushRadius;
public:
    
    Brush();
    
    bool init();
    
    void setupDrawNode(cocos2d::Size visibleSize);
    
    void setSelectedColour(cocos2d::Color4F* selectedColour);
    void setBrushRadius(float* brushRadius);
    
    virtual cocos2d::Node* addDrawNode(cocos2d::Size visibleSize);
    virtual cocos2d::Node* getDrawNode();
    void clearBrushDrawing();
    
    virtual void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
};

#endif /* Brush_h */
