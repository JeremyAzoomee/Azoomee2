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
#include <AzoomeeCommon/Gestures/PinchGestureRecognizer.h>

NS_AZOOMEE_AA_BEGIN

class StickerPlacementNode : public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    enum ControlMode {NONE = -1, MOVE = 0, ROTATE = 1, SCALE = 2};
    cocos2d::Sprite* _sticker = nullptr;
//    cocos2d::EventListenerTouchAllAtOnce* _touchListener = nullptr;
    cocos2d::PinchGestureRecognizer* _pinchGesture = nullptr;
    
    float _scaleFactor;
    float _rotationAngle;
    
    cocos2d::Sprite* _stickerButton_rotate = nullptr;
    cocos2d::Sprite* _stickerButton_scale = nullptr;
    cocos2d::ui::Scale9Sprite* _stickerFrame = nullptr;
    
    void onPinch(cocos2d::PinchGestureRecognizer* recognizer);
    void addGestureRecognizers();
    
public:
    CREATE_FUNC(StickerPlacementNode);
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    
    void setTouchListenerEnabled(bool enabled);
    
    void setupTouchHandling();
    void setSticker(cocos2d::Sprite* sticker);
    cocos2d::Sprite* getSticker();
    void reset();
    void updateStickerControls();
};

NS_AZOOMEE_AA_END

#endif /* StickerPlacementNode_h */
