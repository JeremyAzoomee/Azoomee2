//
//  DrawingCanvasUILayer.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 16/10/2017.
//
//

#ifndef DrawingCanvasUILayer_h
#define DrawingCanvasUILayer_h

#include "AzoomeeArtApp.h"
#include "DrawingCanvas.h"
#include "PatternFileStorage.h"

NS_AZOOMEE_AA_BEGIN

class DrawingCanvasUILayer: public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    static const std::vector<cocos2d::Color3B> _kColours;
    static const std::vector<std::pair<std::string,std::string>> _kPatterns;
    
    DrawingCanvas* _drawingCanvas = nullptr;
    
    cocos2d::Sprite* _selected = nullptr;
    cocos2d::Node* _colourButtonLayout = nullptr;
    cocos2d::ui::Button* _colourSelectButton = nullptr;
    cocos2d::ui::Button* _selectedToolButton = nullptr;
    cocos2d::Node* _toolButtonLayout = nullptr;
    cocos2d::ui::Button* _clearButton = nullptr;
    cocos2d::ui::Button* _undoButton = nullptr;
    cocos2d::ui::Slider* _brushSizeSlider = nullptr;
    
    cocos2d::Node* _confirmDeleteImagePopup = nullptr;
    cocos2d::ui::Button* _confrimDeleteButton = nullptr;
    cocos2d::ui::Button* _cancelDeleteButton = nullptr;
    
    cocos2d::ui::Button* _addStickerButton = nullptr;
    cocos2d::ui::Button* _closeStickerSelectButton = nullptr;
    cocos2d::ui::Button* _confirmStickerButton = nullptr;
    cocos2d::ui::Button* _cancelStickerButton = nullptr;
    
    cocos2d::ui::ScrollView* _stickerScrollView = nullptr;
    cocos2d::Node* _stickerCategoryLayout = nullptr;
    StickerFileStore _stickerCats;
    cocos2d::ui::Scale9Sprite* _stickerCatBG = nullptr;
    cocos2d::Sprite* _selectionIndicator = nullptr;
    cocos2d::ui::Scale9Sprite* _stickerPopupBG = nullptr;
    
    cocos2d::LayerColor* _overlay = nullptr;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    
    //setup functions
    void addBackgroundFrame(const cocos2d::Size& visibleSize, const cocos2d::Point& visibleOrigin);
    void addClearButton(const cocos2d::Size& visibleSize, const cocos2d::Point& visibleOrigin);
    void addColourSelectButtons(const cocos2d::Size& visibleSize, const cocos2d::Point& visibleOrigin);
    void addToolSelectButtons(const cocos2d::Size& visibleSize, const cocos2d::Point& visibleOrigin);
    void addStickerSelectButtons(const cocos2d::Size& visibleSize, const cocos2d::Point& visibleOrigin);
    void addBrushRadiusSlider(const cocos2d::Size& visibleSize, const cocos2d::Point& visibleOrigin);
    
    void addBrushTool(const std::string& buttonFilename, const std::string& bodyFilename, BrushType type, cocos2d::Vec2 normalisedPos, bool selected);
    
    //button callbacks
    void onClearButtonPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void onUndoButtonPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void onColourChangePressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void onColourSelectPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void onAddStickerPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void onAddStickerButtonPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void onCloseStickerSelectPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void onCloseColourSelectPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    
    void onConfirmStickerPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void onCancelStickerPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    
    void onConfirmDeletePressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void onCancelDeletePressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    
    void onToolChanged(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType, int index);
    
    void onRadiusSliderInteract(cocos2d::Ref* pSender, cocos2d::ui::Slider::EventType eEventType);
    
    void onStickerCategoryChangePressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType, int index);
    
    //UI toggle tools
    void setUIVisible(bool isVisible);
    void setUIEnabled(bool isEnabled);
    void setOnlyPatternBrushesEnabled(bool isEnabled);
    
    void setStickerUIEnabled(bool isEnabled);
    void setStickerPopupVisible(bool isVisible);
    
    void setButtonBodyColour(cocos2d::ui::Button* button, cocos2d::Color3B colour);
    void setButtonBodyPattern(cocos2d::ui::Button* button, const std::string& pattern);
    
    StickerFileStore getStickerFilesFromJSON();
    
public:
    CREATE_FUNC(DrawingCanvasUILayer);
    void setDrawingCanvas(DrawingCanvas* drawingCanvas);
    
};

NS_AZOOMEE_AA_END

#endif /* DrawingCanvasUILayer_h */
