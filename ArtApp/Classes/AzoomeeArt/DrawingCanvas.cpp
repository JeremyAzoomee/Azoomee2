//
//  DrawingCanvas.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 24/05/2017.
//
//

#include "DrawingCanvas.h"
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/Analytics/AnalyticsSingleton.h>
#include <TinizineCommon/Utils/DirUtil.h>
#include <dirent.h>
#include <math.h>

using namespace cocos2d;

USING_NS_TZ
NS_AZ_ART_BEGIN

bool DrawingCanvas::init()
{
    
    if(!Super::init())
    {
        return false;
    }
    
    _brushConfig = BrushConfig::create();
    
    _background = LayerColor::create(Color4B(Colours::Color_4F::pureWhite));
     _brushConfig->setBgImageFilename(kArtAppAssetLoc + "white_bg.png");
    
    this->addChild(_background,BACKGROUND_LAYER);
    
    const Size& visibleSize = Director::getInstance()->getWinSize();
    
    _drawing = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    _drawing->setAnchorPoint(Vec2(0.5,0.5));
    _drawing->setPosition(visibleSize/2);
    _drawing->beginWithClear(Colours::Color_4F::pureWhite.r, Colours::Color_4F::pureWhite.g, Colours::Color_4F::pureWhite.b, Colours::Color_4F::pureWhite.a);
    _drawing->end();
    
    this->addChild(_drawing);
    
    _actionCounter = 0;
    
    return true;
}

void DrawingCanvas::onEnter()
{
    Super::onEnter();
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    this->setContentSize(visibleSize);
    
    addBrushes();
    
    
    for(BrushRef b : _brushes)
    {
        b->setupDrawNode(visibleSize);
    }
    
    setupTouchHandling();

    _stickerNode = StickerPlacementNode::create();
    this->addChild(_stickerNode,MAIN_UI_LAYER);
    _stickerNode->setVisible(false);
    _stickerNode->setTouchListenerEnabled(false);
}

void DrawingCanvas::onExit()
{
    Super::onExit();
    
    _brushes.clear();
    
}

void DrawingCanvas::setBrushRadius(float brushRadius)
{
    _brushConfig->setBrushRadius(brushRadius);
}

void DrawingCanvas::setSelectedColour(cocos2d::Color4F colour)
{
    _brushConfig->setSelectedColour(colour);
}

Color4F DrawingCanvas::getSelectedColour()
{
    return _brushConfig->getSelectedColour();
}

void DrawingCanvas::setSelectedPattern(const std::string& pattern)
{
    _brushConfig->setSelectedPattern(pattern);
}

void DrawingCanvas::setSelectedPatternTansparant(const std::string &pattern)
{
    _brushConfig->setSelectedPatternTansparant(pattern);
}

std::string DrawingCanvas::getSelectedPattern()
{
    return _brushConfig->getSelectedPattern();
}

void DrawingCanvas::setListenerEnabled(bool isEnabled)
{
    _drawCanvasTouchListener->setEnabled(isEnabled);
}

int DrawingCanvas::getDrawingStackSize()
{
    return (int)_drawingStack.size();
}

void DrawingCanvas::undoAction()
{
    this->removeChild(_drawingStack.back());
    _drawingStack.pop_back();
    _actionCounter--;
}

void DrawingCanvas::clearDrawing()
{
    for(Node* layer : _drawingStack)
    {
        layer->removeFromParent();
    }
    _drawingStack.clear();
    _drawing->beginWithClear(Colours::Color_4F::pureWhite.r,Colours::Color_4F::pureWhite.g,Colours::Color_4F::pureWhite.b,Colours::Color_4F::pureWhite.a);
    _drawing->end();
    Director::getInstance()->getRenderer()->render();
    _actionCounter++;
}

void DrawingCanvas::setStickerNodeVisible(bool isVisible)
{
    _stickerNode->setVisible(isVisible);
    _stickerNode->setTouchListenerEnabled(isVisible);
}

void DrawingCanvas::setupStickerNode(const std::string& stickerFile, const std::string& identifier)
{
    Sprite* newSticker = Sprite::create(stickerFile);
    newSticker->setName(identifier);
    newSticker->setAnchorPoint(Vec2(0.5,0.5));
    newSticker->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize()/2);
    _stickerNode->reset();
    _stickerNode->setSticker(newSticker);
}

void DrawingCanvas::addStickerToDrawing()
{
    AnalyticsSingleton::getInstance()->stickerSelectedEvent(_stickerNode->getSticker()->getName());
    _drawingStack.push_back(_stickerNode->getSticker());
    Sprite* temp = _stickerNode->getSticker();
    temp->retain(); //move sticker from sticker node to drawing canvas
    temp->removeFromParent();
    this->addChild(temp);
    temp->release();
    if(_drawingStack.size()>_kNumberOfUndos)
    {
        Node* mergingLayer = _drawingStack[0];
        _drawingStack.erase(_drawingStack.begin());
        _drawing->begin();
        mergingLayer->visit();
        _drawing->end();
        Director::getInstance()->getRenderer()->render();
        this->removeChild(mergingLayer);
    }
    this->removeChild(_activeBrush->getDrawNode());
    this->addChild(_activeBrush->addDrawNode(Director::getInstance()->getVisibleSize()));
    
    _actionCounter++;
}

void DrawingCanvas::changeTool(int index)
{
    _activeBrush->getDrawNode()->removeFromParent();
    _activeBrush = _brushes[index];
    this->addChild(_activeBrush->addDrawNode(Director::getInstance()->getVisibleSize()));

}

void DrawingCanvas::removeCurrentDrawNode()
{
    _activeBrush->getDrawNode()->removeFromParent();
}

void DrawingCanvas::addNewDrawNode()
{
    this->addChild(_activeBrush->addDrawNode(Director::getInstance()->getVisibleSize()));
}

void DrawingCanvas::setBaseImage(const std::string& fileName)
{
    Director* director = Director::getInstance();
    
    Sprite* baseImage = Sprite::create(fileName);
    baseImage->setAnchorPoint(Vec2(0.5,0.5));
    baseImage->setPosition(director->getVisibleOrigin() + Vec2(director->getVisibleSize().width*0.535,director->getVisibleSize().height*0.575));
    baseImage->setScale(MIN((director->getVisibleSize().width * 0.89)/baseImage->getContentSize().width,(director->getVisibleSize().height * 0.8)/baseImage->getContentSize().height));
    _drawing->begin();
    baseImage->visit();
    _drawing->end();
    director->getRenderer()->render();
}

void DrawingCanvas::saveImage(const std::string& filePath)
{
    
    _drawing->begin();
    for(Node* n : _drawingStack)
    {
        n->visit();
    }
    _drawing->end();
    Director::getInstance()->getRenderer()->render();
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite* drawingSprite = Sprite::createWithTexture(_drawing->getSprite()->getTexture());
    drawingSprite->setScale(0.5);
    drawingSprite->setAnchorPoint(Vec2(0,0));
    drawingSprite->setPosition(-Director::getInstance()->getVisibleOrigin()/2.0f - Vec2(visibleSize.width * 0.09/2.0f,visibleSize.height * 0.175/2.0f));
    drawingSprite->setFlippedY(true);
    
    RenderTexture* outputTexture = RenderTexture::create(visibleSize.width*0.89/2.0f, visibleSize.height*0.8/2.0f);
    outputTexture->begin();
    drawingSprite->visit();
    outputTexture->end();
    outputTexture->saveToFile(DirUtil::getCachesPath() + filePath,Image::Format::PNG);
    Director::getInstance()->getRenderer()->render();
    
}

void DrawingCanvas::setupTouchHandling()
{
    static bool touchDetected = false;
    _drawCanvasTouchListener = EventListenerTouchOneByOne::create();
    
    _drawCanvasTouchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        if(touchDetected)
        {
            return false;
        }
        
        touchDetected = true;
        
        _activeBrush->onTouchBegin(touch, event);

        return true;
    };
    
    _drawCanvasTouchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        _activeBrush->onTouchMoved(touch, event);
        
    };
    
    _drawCanvasTouchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        this->getEventDispatcher()->setEnabled(false);
        
        _activeBrush->onTouchEnded(touch, event);
        
        _drawingStack.push_back(_activeBrush->getDrawNode());
        this->addChild(_activeBrush->addDrawNode(Director::getInstance()->getVisibleSize()));
		
        if(_drawingStack.size()>_kNumberOfUndos)
        {
            Node* mergingLayer = _drawingStack[0];
            _drawingStack.erase(_drawingStack.begin());
            _drawing->begin();
            mergingLayer->visit();
            _drawing->end();
            Director::getInstance()->getRenderer()->render();
            this->removeChild(mergingLayer);
        }
        this->getEventDispatcher()->setEnabled(true);
        
        _actionCounter++;
        touchDetected = false;
        
    };
    
    _drawCanvasTouchListener->onTouchCancelled = _drawCanvasTouchListener->onTouchEnded;
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_drawCanvasTouchListener, this);
    
}

void DrawingCanvas::addBrushes()
{
    _brushes.clear();
    
    BrushRef pen = std::make_shared<BrushPen>();
    BrushRef paintbrush = std::make_shared<BrushPaintBrush>();
    BrushRef highlighter = std::make_shared<BrushHighlighter>();
    BrushRef sprayCan = std::make_shared<BrushSprayCan>();
    BrushRef eraser = std::make_shared<BrushEraser>();
    
    _brushes.push_back(pen);
    _brushes.push_back(paintbrush);
    _brushes.push_back(highlighter);
    _brushes.push_back(sprayCan);
    _brushes.push_back(eraser);
    
    for (BrushRef b : _brushes) {
        b->init();
        b->setBrushConfig(_brushConfig);
    }
    _activeBrush = _brushes[0];
    this->addChild(_activeBrush->getDrawNode());
}

void DrawingCanvas::reloadRenderTextureObject()
{
	const Size& visibleSize = Director::getInstance()->getWinSize();
	
	Sprite* drawingSprite = Sprite::createWithTexture(_drawing->getSprite()->getTexture());
	drawingSprite->setAnchorPoint(Vec2(0,0));
	drawingSprite->setFlippedY(true);
	
	auto newDrawing = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
	newDrawing->setAnchorPoint(Vec2(0.5,0.5));
	newDrawing->setPosition(visibleSize/2);
	newDrawing->beginWithClear(Colours::Color_4F::pureWhite.r, Colours::Color_4F::pureWhite.g, Colours::Color_4F::pureWhite.b, Colours::Color_4F::pureWhite.a);
	drawingSprite->visit();
	newDrawing->end();
	
	_drawing->removeFromParent();
	_drawing = newDrawing;
	this->addChild(_drawing, -1);
}

NS_AZ_ART_END
