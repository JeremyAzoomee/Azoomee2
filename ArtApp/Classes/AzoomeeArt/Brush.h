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
#include "BrushConfig.h"
#include "AzoomeeArtApp.h"

NS_AZOOMEE_AA_BEGIN

enum BrushType{PEN,PAINTBRUSH,HIGHLIGHTER,SPRAYCAN,ERASER,NUM_BRUSHES};

class Brush;
typedef std::shared_ptr<Brush> BrushRef;

class Brush
{
protected:
    
    cocos2d::DrawNode* _drawNode = nullptr;
    cocos2d::Vec2 _lastTouchPos;
    cocos2d::ClippingNode* _maskingNode = nullptr;
    BrushConfigRef _brushConfig;
public:
    
    Brush();
    
    bool init();
    
    void setupDrawNode(const cocos2d::Size& visibleSize);
    
    void setBrushConfig(BrushConfigRef brushConfig);
    
    virtual cocos2d::Node* addDrawNode(const cocos2d::Size& visibleSize);
    virtual cocos2d::Node* getDrawNode();
    void clearBrushDrawing();
    
    virtual void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    BrushType _type;
};

NS_AZOOMEE_AA_END

#endif /* Brush_h */
