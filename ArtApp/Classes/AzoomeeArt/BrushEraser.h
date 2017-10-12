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
    cocos2d::ClippingNode* maskingNode = nullptr;
    std::string bgImageFile;
public:
    BrushEraser();
    
    cocos2d::Node* addDrawNode(const cocos2d::Size& visibleSize) override;
    cocos2d::Node* getDrawNode() override;
    
    void setBgImageFile(const std::string& filename);
    
    void onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    
};

NS_AZOOMEE_AA_END

#endif /* BrushEraser_h */
