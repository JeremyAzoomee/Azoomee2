//
//  DrawingCanvas.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 24/05/2017.
//
//

#ifndef DrawingCanvas_h
#define DrawingCanvas_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Constants.h"
#include "Brush.h"
#include "BrushPen.h"
#include "BrushPaintBrush.h"
#include "BrushHighlighter.h"
#include "BrushSprayCan.h"
#include "BrushEraser.h"
#include "StickerPlacementNode.h"
#include <vector>
#include "AzoomeeArtApp.h"
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_AA_BEGIN
typedef std::pair<std::string,std::vector<std::string>> StickerSet;
typedef std::vector<StickerSet> StickerFileStore;

class DrawingCanvas : public cocos2d::Node{
    typedef cocos2d::Node Super;
public:
    CREATE_FUNC(DrawingCanvas);
    void setBaseImage(const std::string& fileName);
    void saveImage(const std::string& filePath);
    
    int _actionCounter;
    
protected:
    static const int _kNumberOfUndos = 3;
    static const std::vector<cocos2d::Color3B> _kColours;
    
    cocos2d::LayerColor* _background = nullptr;
    cocos2d::Sprite* _currentDrawing = nullptr;
    cocos2d::RenderTexture* _drawing = nullptr;
    std::vector<cocos2d::Node*> _drawingStack;
    
    cocos2d::EventListenerTouchOneByOne* _drawCanvasTouchListener;
    
    cocos2d::Sprite* _selected = nullptr;
    cocos2d::Color4F _selectedColour;
    cocos2d::Node* _colourButtonLayout = nullptr;
    cocos2d::ui::Button* _colourSelectButton = nullptr;
    cocos2d::ui::Button* _selectedToolButton = nullptr;
    cocos2d::Node* _toolButtonLayout = nullptr;
    cocos2d::ui::Button* _clearButton = nullptr;
    cocos2d::ui::Button* _undoButton = nullptr;
    cocos2d::ui::Slider* _brushSizeSlider = nullptr;
    cocos2d::Sprite* _leftBG = nullptr;
    cocos2d::Sprite* _rightBG = nullptr;
    cocos2d::Sprite* _toolBG = nullptr;
    
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
    
    StickerPlacementNode* _stickerNode = nullptr;
    
    Brush* _activeBrush = nullptr;
    std::vector<Brush*> _brushes;
    
    float _brushRadius;
    
    std::string _bgImageFilename;
    
    cocos2d::LayerColor* _overlay = nullptr;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    
    //Internal methods
    
    void setupTouchHandling();
    void setupMenus();
    
    void addBrushes();
    
    void addClearButton(const cocos2d::Size& visibleSize, const cocos2d::Point& visibleOrigin);
    void addColourSelectButtons(const cocos2d::Size& visibleSize, const cocos2d::Point& visibleOrigin);
    void addToolSelectButtons(const cocos2d::Size& visibleSize, const cocos2d::Point& visibleOrigin);
    void addStickerSelectButtons(const cocos2d::Size& visibleSize, const cocos2d::Point& visibleOrigin);
    void addBrushRadiusSlider(const cocos2d::Size& visibleSize, const cocos2d::Point& visibleOrigin);
    
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
    
    void setUIVisible(bool isVisible);
    void setUIEnabled(bool isEnabled);
    
private:
    
    StickerFileStore getStickerFilesFromJSON();
    
};

NS_AZOOMEE_AA_END

#endif /* DrawingCanvas_h */
