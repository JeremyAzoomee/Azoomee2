//
//  DrawingCanvas.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 24/05/2017.
//
//

#include "DrawingCanvas.h"
#include <AzoomeeCommon/UI/Style.h>
#include <dirent.h>
#include <math.h>

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

bool DrawingCanvas::init()
{
    
    if(!Super::init())
    {
        return false;
    }
    
    _background = LayerColor::create(Color4B(Style::Color_4F::pureWhite));
    _bgImageFilename = kArtAppAssetLoc + "white_bg.png";
    
    this->addChild(_background,BACKGROUND_LAYER);
    
    const Size& visibleSize = Director::getInstance()->getWinSize();
    
    _drawing = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    _drawing->setAnchorPoint(Vec2(0.5,0.5));
    _drawing->setPosition(visibleSize/2);
    _drawing->beginWithClear(Style::Color_4F::pureWhite.r, Style::Color_4F::pureWhite.g, Style::Color_4F::pureWhite.b, Style::Color_4F::pureWhite.a);
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
    
    
    for(Brush* b : _brushes)
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
    
    for(Brush* b : _brushes)
    {
        delete b;
    }
    
    _brushes.clear();
    
}

void DrawingCanvas::setBrushRadius(float brushRadius)
{
    _brushRadius = brushRadius;
}

void DrawingCanvas::setSelectedColour(cocos2d::Color4F colour)
{
    _selectedColour = colour;
}

Color4F DrawingCanvas::getSelectedColour()
{
    return _selectedColour;
}

void DrawingCanvas::setSelectedPattern(const std::string& pattern)
{
    _selectedPattern = pattern;
}

void DrawingCanvas::setSelectedPatternTansparant(const std::string &pattern)
{
    _selectedPatternTransparant = pattern;
}

std::string DrawingCanvas::getSelectedPattern()
{
    return _selectedPattern;
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
    _drawing->beginWithClear(Style::Color_4F::pureWhite.r,Style::Color_4F::pureWhite.g,Style::Color_4F::pureWhite.b,Style::Color_4F::pureWhite.a);
    _drawing->end();
    Director::getInstance()->getRenderer()->render();
    _actionCounter++;
}

void DrawingCanvas::setStickerNodeVisible(bool isVisible)
{
    _stickerNode->setVisible(isVisible);
    _stickerNode->setTouchListenerEnabled(isVisible);
}

void DrawingCanvas::setupStickerNode(const std::string& stickerFile)
{
    Sprite* newSticker = Sprite::create(stickerFile);
    newSticker->setAnchorPoint(Vec2(0.5,0.5));
    newSticker->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize()/2);
    _stickerNode->reset();
    _stickerNode->setSticker(newSticker);
}

void DrawingCanvas::addStickerToDrawing()
{
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
    baseImage->setAnchorPoint(Vec2(0,0));
    baseImage->setPosition(director->getVisibleOrigin() + Vec2(director->getVisibleSize().width*0.09,director->getVisibleSize().height*0.175));
    baseImage->setScale((director->getVisibleSize().width * 0.89)/baseImage->getContentSize().width);
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
    outputTexture->saveToFile(filePath,Image::Format::PNG);
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
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_drawCanvasTouchListener, this);
    
}

void DrawingCanvas::addBrushes()
{
    _brushes.clear();
    
    Brush* pen = new BrushPen();
    Brush* paintbrush = new BrushPaintBrush();
    Brush* highlighter = new BrushHighlighter();
    Brush* sprayCan = new BrushSprayCan();
    Brush* eraser = new BrushEraser();
    
    _brushes.push_back(pen);
    _brushes.push_back(paintbrush);
    _brushes.push_back(highlighter);
    _brushes.push_back(sprayCan);
    _brushes.push_back(eraser);
    
    for (Brush* b : _brushes) {
        b->init();
        b->setSelectedColour(&_selectedColour);
        b->setBrushRadius(&_brushRadius);
        b->setBgImageFile(&_selectedPattern);
    }
    eraser->setBgImageFile(&_bgImageFilename);
    highlighter->setBgImageFile(&_selectedPatternTransparant);
    _activeBrush = _brushes[0];
    this->addChild(_activeBrush->getDrawNode());
}


NS_AZOOMEE_AA_END
