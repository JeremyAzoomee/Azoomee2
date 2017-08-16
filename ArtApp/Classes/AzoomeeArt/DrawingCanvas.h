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

static const std::string ArtAppAssetLoc = "res/artapp/style/images/artIcons/";
static const std::string stickerLoc = "res/chat/stickers/";

class DrawingCanvas : public cocos2d::Node{
    typedef cocos2d::Node Super;
public:
    CREATE_FUNC(DrawingCanvas);
    void setBaseImage(const std::string& fileName);
    void saveImage(const std::string& filePath);
    
    int actionCounter;
    
protected:
    static const int numberOfUndos = 3;
    
    cocos2d::LayerColor* background = nullptr;
    cocos2d::Sprite* currentDrawing = nullptr;
    cocos2d::RenderTexture* drawing = nullptr;
    std::vector<cocos2d::Node*> drawingStack;
    
    cocos2d::EventListenerTouchOneByOne* drawCanvasTouchListener;
    
    cocos2d::Sprite* selected = nullptr;
    cocos2d::Color4F selectedColour;
    cocos2d::Node* colourButtonLayout = nullptr;
    cocos2d::ui::Button* colourSelectButton = nullptr;
    cocos2d::ui::Button* SelectedToolButton = nullptr;
    cocos2d::Node* toolButtonLayout = nullptr;
    cocos2d::ui::Button* clearButton = nullptr;
    cocos2d::ui::Slider* brushSizeSlider = nullptr;
    cocos2d::Sprite* leftBG = nullptr;
    cocos2d::Sprite* rightBG = nullptr;
    cocos2d::Sprite* toolBG = nullptr;
    
    cocos2d::Node* confirmDeleteImagePopup = nullptr;
    cocos2d::ui::Button* confrimDeleteButton = nullptr;
    cocos2d::ui::Button* cancelDeleteButton = nullptr;
    
    cocos2d::ui::Button* addStickerButton = nullptr;
    cocos2d::ui::Button* closeStickerSelectButton = nullptr;
    cocos2d::ui::Button* confirmStickerButton = nullptr;
    cocos2d::ui::Button* cancelStickerButton = nullptr;
    
    cocos2d::ui::ScrollView* stickerScrollView = nullptr;
    cocos2d::Node* StickerCategoryLayout = nullptr;
    StickerFileStore stickerCats;
    cocos2d::ui::Scale9Sprite* stickerCatBG = nullptr;
    cocos2d::Sprite* selectionIndicator = nullptr;
    
    StickerPlacementNode* stickerNode = nullptr;
    
    Brush* activeBrush = nullptr;
    std::vector<Brush*> brushes;
    
    float brushRadius;
    
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
    void onColourChangePressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void onColourSelectPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void onToolSelectPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
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
