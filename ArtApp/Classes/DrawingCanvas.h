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
public:
    CREATE_FUNC(DrawingCanvas);
    void setBaseImage(std::string fileName);
    void saveImage(std::string filePath);
protected:
    static const int numberOfUndos = 3;
    
    cocos2d::LayerColor* background;
    cocos2d::Sprite* currentDrawing;
    cocos2d::RenderTexture* drawing;
    std::vector<cocos2d::Node*> drawingStack;
    
    cocos2d::EventListenerTouchOneByOne* drawCanvasTouchListener;
    
    cocos2d::Sprite* selected;
    cocos2d::Color4F selectedColour;
    cocos2d::Node* colourButtonLayout;
    cocos2d::ui::Button* colourSelectButton;
    cocos2d::ui::Button* SelectedToolButton;
    cocos2d::Node* toolButtonLayout;
    cocos2d::ui::Button* clearButton;
    cocos2d::ui::Slider* brushSizeSlider;
    cocos2d::Sprite* leftBG;
    cocos2d::Sprite* rightBG;
    cocos2d::Sprite* toolBG;
    
    cocos2d::Node* confirmDeleteImagePopup;
    cocos2d::ui::Button* confrimDeleteButton;
    cocos2d::ui::Button* cancelDeleteButton;
    
    cocos2d::ui::Button* addStickerButton;
    cocos2d::ui::Button* closeStickerSelectButton;
    cocos2d::ui::Button* confirmStickerButton;
    cocos2d::ui::Button* cancelStickerButton;
    
    cocos2d::ui::ScrollView* stickerScrollView;
    cocos2d::Node* StickerCategoryLayout;
    StickerFileStore stickerCats;
    cocos2d::ui::Scale9Sprite* stickerCatBG;
    cocos2d::Sprite* selectionIndicator;
    
    StickerPlacementNode* stickerNode;
    
    Brush* activeBrush;
    std::vector<Brush*> brushes;
    
    float brushRadius;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    
    //Internal methods
    
    void setupTouchHandling();
    void setupMenus();
    
    void addBrushes();
    
    void addClearButton(cocos2d::Size visibleSize, cocos2d::Point visibleOrigin);
    void addColourSelectButtons(cocos2d::Size visibleSize, cocos2d::Point visibleOrigin);
    void addToolSelectButtons(cocos2d::Size visibleSize, cocos2d::Point visibleOrigin);
    void addStickerSelectButtons(cocos2d::Size visibleSize, cocos2d::Point visibleOrigin);
    void addBrushRadiusSlider(cocos2d::Size visibleSize, cocos2d::Point visibleOrigin);
    
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
    
    std::vector<std::string> getStickerDirs();
    std::vector<std::string> getStickerFileNamesInDir(std::string stickerDir);
};

NS_AZOOMEE_AA_END

#endif /* DrawingCanvas_h */
