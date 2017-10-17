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
    
    void setBrushRadius(float brushRadius);
    void setSelectedColour(cocos2d::Color4F colour);
    cocos2d::Color4F getSelectedColour();
    
    void setListenerEnabled(bool isEnabled);
    
    int getDrawingStackSize();
    void undoAction();
    void clearDrawing();
    
    void setStickerNodeVisible(bool isVisible);
    void setupStickerNode(const std::string& stickerFile);
    void addStickerToDrawing();
    void changeTool(int index);
    
protected:
    static const int _kNumberOfUndos = 3;
    
    cocos2d::LayerColor* _background = nullptr;
    cocos2d::Sprite* _currentDrawing = nullptr;
    cocos2d::RenderTexture* _drawing = nullptr;
    std::vector<cocos2d::Node*> _drawingStack;
    
    cocos2d::EventListenerTouchOneByOne* _drawCanvasTouchListener;
    
    StickerPlacementNode* _stickerNode = nullptr;
    
    Brush* _activeBrush = nullptr;
    std::vector<Brush*> _brushes;
    
    float _brushRadius;
    cocos2d::Color4F _selectedColour;
    
    std::string _bgImageFilename;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    
    //Internal methods
    
    void setupTouchHandling();
    
    void addBrushes();
    
private:
    
    StickerFileStore getStickerFilesFromJSON();
    
};

NS_AZOOMEE_AA_END

#endif /* DrawingCanvas_h */
