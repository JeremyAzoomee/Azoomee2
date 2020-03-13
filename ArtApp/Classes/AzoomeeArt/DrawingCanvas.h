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
#include "BrushConfig.h"
#include <vector>
#include "AzoomeeArtApp.h"
#include <TinizineCommon/Data/Json.h>

USING_NS_TZ
NS_AZ_ART_BEGIN

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
    void setSelectedPattern(const std::string& pattern);
    void setSelectedPatternTansparant(const std::string& pattern);
    std::string getSelectedPattern();
    
    void setListenerEnabled(bool isEnabled);
    
    int getDrawingStackSize();
    void undoAction();
    void clearDrawing();
    
    void setStickerNodeVisible(bool isVisible);
    void setupStickerNode(const std::string& stickerFile, const std::string& identifier);
    void addStickerToDrawing();
    void changeTool(int index);
    void removeCurrentDrawNode();
    void addNewDrawNode();
	
	void reloadRenderTextureObject(); // on background event on android messes with the depth buffer on render texture cus of openGL context getting killed.  needs new renderTex made to reset it to allow for clipping nodes to be baked into the texture. called via AppDelegate
    
protected:
    static const int _kNumberOfUndos = 3;
    
    cocos2d::LayerColor* _background = nullptr;
    cocos2d::Sprite* _currentDrawing = nullptr;
    cocos2d::RenderTexture* _drawing = nullptr;
    std::vector<cocos2d::Node*> _drawingStack;
    
    cocos2d::EventListenerTouchOneByOne* _drawCanvasTouchListener;
    
    StickerPlacementNode* _stickerNode = nullptr;
    
    BrushRef _activeBrush;
    std::vector<BrushRef> _brushes;
    
    BrushConfigRef _brushConfig;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    
    //Internal methods
    
    void setupTouchHandling();
    
    void addBrushes();
    
};

NS_AZ_ART_END

#endif /* DrawingCanvas_h */
