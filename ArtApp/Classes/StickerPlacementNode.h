//
//  StickerPlacementNode.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 05/06/2017.
//
//

#ifndef StickerPlacementNode_h
#define StickerPlacementNode_h

#include "cocos2d.h"
#include <vector>

class StickerPlacementNode : public cocos2d::Node
{
private:
    cocos2d::Sprite* sticker;
    cocos2d::EventListenerTouchOneByOne* touchListener;
public:
    CREATE_FUNC(StickerPlacementNode);
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    
    void setTouchListenerEnabled(bool enabled);
    
    void setupTouchHandling();
    void setSticker(cocos2d::Sprite* sticker);
    cocos2d::Sprite* getSticker();
};

#endif /* StickerPlacementNode_h */
