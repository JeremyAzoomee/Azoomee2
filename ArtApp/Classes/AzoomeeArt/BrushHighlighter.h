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
#include "AzoomeeArtApp.h"

USING_NS_TZ

NS_AZ_ART_BEGIN

class BrushHighlighter : public Brush
{
private:
    static float _alphaLevel;
    cocos2d::Vec2 _points[4];
    bool _lineStarted;
public:
    BrushHighlighter();
    
    cocos2d::Node* addDrawNode(const cocos2d::Size& visibleSize) override;
    cocos2d::Node* getDrawNode() override;
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
};

NS_AZ_ART_END

#endif /* BrushHighlighter_h */
