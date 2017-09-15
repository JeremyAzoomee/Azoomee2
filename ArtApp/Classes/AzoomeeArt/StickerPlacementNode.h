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
#include "ui/CocosGUI.h"
#include <vector>
#include "AzoomeeArtApp.h"

NS_AZOOMEE_AA_BEGIN

class StickerPlacementNode : public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    cocos2d::Sprite* sticker = nullptr;
    cocos2d::EventListenerTouchOneByOne* touchListener;
    
    cocos2d::ui::Slider* scaleSlider = nullptr;
    float scaleFactor;

    cocos2d::ui::Slider* rotationSlider = nullptr;
    float rotationAngle;
    
    cocos2d::ui::Button* stickerButton_rotate;
    cocos2d::ui::Button* stickerButton_scale;
    
    void onScaleSliderInteract(cocos2d::Ref* pSender, cocos2d::ui::Slider::EventType eEventType);
    void onRotationSliderInteract(cocos2d::Ref* pSender, cocos2d::ui::Slider::EventType eEventType);
    
public:
    CREATE_FUNC(StickerPlacementNode);
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    
    void setTouchListenerEnabled(bool enabled);
    
    void setupTouchHandling();
    void setSticker(cocos2d::Sprite* sticker);
    cocos2d::Sprite* getSticker();
    void resetSliders();
};

NS_AZOOMEE_AA_END

#endif /* StickerPlacementNode_h */
