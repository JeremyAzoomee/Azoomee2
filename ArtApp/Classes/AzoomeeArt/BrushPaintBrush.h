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

USING_NS_TZ

NS_AZ_AA_BEGIN

class BrushPaintBrush: public Brush
{
private:
    const int _kSpriteCountLimit = 1000;

    cocos2d::Node* _brushLayer = nullptr;
    cocos2d::RenderTexture* _paintLayer = nullptr;
    int _spriteCount = 0;
public:
    BrushPaintBrush();
    
    cocos2d::Node* addDrawNode(const cocos2d::Size& visibleSize) override;
    cocos2d::Node* getDrawNode() override;
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
};

NS_AZ_AA_END

#endif /* BrushPaintBrush_h */
