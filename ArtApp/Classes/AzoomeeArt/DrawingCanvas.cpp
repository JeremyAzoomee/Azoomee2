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

const std::vector<Color3B> DrawingCanvas::_kColours = {
    Color3B(Style::Color_4F::darkBrown),
    Color3B(Style::Color_4F::brown),
    Color3B(Style::Color_4F::lightBrown),
    Color3B(Style::Color_4F::orangeBrown),
    Color3B(Style::Color_4F::orange),
    Color3B(Style::Color_4F::darkYellow),
    Color3B(Style::Color_4F::yellow),
    Color3B(Style::Color_4F::darkBlue),
    Color3B(Style::Color_4F::blue),
    Color3B(Style::Color_4F::lightBlue),
    Color3B(Style::Color_4F::greenBlue),
    Color3B(Style::Color_4F::green),
    Color3B(Style::Color_4F::grassGreen),
    Color3B(Style::Color_4F::neonGreen),
    Color3B(Style::Color_4F::darkPurple),
    Color3B(Style::Color_4F::purple),
    Color3B(Style::Color_4F::neonPink),
    Color3B(Style::Color_4F::palePink),
    Color3B(Style::Color_4F::pink),
    Color3B(Style::Color_4F::darkPink),
    Color3B(Style::Color_4F::red),
    Color3B(Style::Color_4F::neonBlue),
    Color3B(Style::Color_4F::paleGreen),
    Color3B(Style::Color_4F::paleYellow),
    Color3B(Style::Color_4F::lightGrey),
    Color3B(Style::Color_4F::grey),
    Color3B(Style::Color_4F::darkGrey),
    Color3B(Style::Color_4F::black)
};

bool DrawingCanvas::init()
{
    
    if(!Super::init())
        return false;
    
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
    
    _overlay = LayerColor::create(Style::Color_4B::semiTransparentOverlay, visibleSize.width, visibleSize.height);
    _overlay->setPosition(Director::getInstance()->getVisibleOrigin());
    _overlay->setName("overlay");
    this->addChild(_overlay,POPUP_UI_LAYER);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true; //block touches
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), _overlay);
    _overlay->setVisible(false);
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(_overlay);
    
    setupTouchHandling();
    setupMenus();
    
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

void DrawingCanvas::setBaseImage(const std::string& fileName)
{
    Director* director = Director::getInstance();
    
    Sprite* baseImage = Sprite::create(fileName);
    baseImage->setAnchorPoint(Vec2(0.5,0.5));
    baseImage->setPosition(director->getVisibleOrigin() + director->getVisibleSize()/2);
    baseImage->setScale(director->getWinSize().width/baseImage->getContentSize().width);
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
    
    Sprite* drawingSprite = Sprite::createWithTexture(_drawing->getSprite()->getTexture());
    drawingSprite->setScale(0.5);
    drawingSprite->setAnchorPoint(Vec2(0,0));
    drawingSprite->setPosition(Vec2(0,0));
    drawingSprite->setFlippedY(true);
    
    RenderTexture* outputTexture = RenderTexture::create(drawingSprite->getContentSize().width/2, drawingSprite->getContentSize().height/2);
    outputTexture->begin();
    drawingSprite->visit();
    outputTexture->end();
    outputTexture->saveToFile(filePath,Image::Format::PNG);
    Director::getInstance()->getRenderer()->render();
    
}

void DrawingCanvas::setupTouchHandling()
{

    _drawCanvasTouchListener = EventListenerTouchOneByOne::create();
    
    _drawCanvasTouchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
            
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
        
        if(_drawingStack.size() == 0)
        {
            _undoButton->setEnabled(true);
        }
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
        
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_drawCanvasTouchListener, this);
    
}

void DrawingCanvas::setupMenus()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    const Point& visibleOrigin = Director::getInstance()->getVisibleOrigin();

    addClearButton(visibleSize, visibleOrigin);
    addColourSelectButtons(visibleSize, visibleOrigin);
    addToolSelectButtons(visibleSize, visibleOrigin);
    addBrushRadiusSlider(visibleSize, visibleOrigin);
    addStickerSelectButtons(visibleSize, visibleOrigin);
    
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
    }
    _activeBrush = _brushes[0];
    this->addChild(_activeBrush->getDrawNode());
}

//UI LOADING

void DrawingCanvas::addClearButton(const Size& visibleSize, const Point& visibleOrigin)
{
    
    _rightBG = Sprite::create(kArtAppAssetLoc + "art_back_left.png");
    _rightBG->setAnchorPoint(Vec2(1,0));
    _rightBG->setPosition(visibleOrigin.x + visibleSize.width,visibleOrigin.y - BOTTOM_UI_Y_OFFSET);
    _rightBG->setColor(Color3B::BLACK);
    this->addChild(_rightBG,MAIN_UI_LAYER);
    
    _clearButton = ui::Button::create();
    _clearButton->setAnchorPoint(Vec2(0.5,0));
    _clearButton->setPosition(Vec2(_rightBG->getPosition().x - 2*_rightBG->getContentSize().width/3, visibleOrigin.y));
    _clearButton->loadTextures(kArtAppAssetLoc + "delete.png", kArtAppAssetLoc + "delete.png");
    _clearButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onClearButtonPressed, this));
    this->addChild(_clearButton,MAIN_UI_LAYER);
    
    _undoButton = ui::Button::create();
    _undoButton->setAnchorPoint(Vec2(0.5,0));
    _undoButton->setPosition(Vec2(_rightBG->getPosition().x - _rightBG->getContentSize().width/3, visibleOrigin.y));
    _undoButton->loadTextures(kArtAppAssetLoc + "redo.png", kArtAppAssetLoc + "redo.png");
    _undoButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onUndoButtonPressed, this));
    _undoButton->setEnabled(false);
    this->addChild(_undoButton,MAIN_UI_LAYER);
    
    _confirmDeleteImagePopup = Node::create();
    _confirmDeleteImagePopup->setContentSize(Size(visibleSize.width/2, visibleSize.height/2));
    ui::Scale9Sprite* background = ui::Scale9Sprite::create(kArtAppAssetLoc + "gallery/art_painting_placeholder.png");
    background->setContentSize(Size(visibleSize.width/2, visibleSize.height/2));
    background->setAnchorPoint(Vec2(0.5,0.5));
    background->setPosition(visibleOrigin + visibleSize/2);
    _confirmDeleteImagePopup->addChild(background);
    
    Label* text = Label::createWithTTF("Delete Drawing?", Style::Font::Regular, 128);
    text->setAnchorPoint(Vec2(0.5,0.5));
    text->setPosition(visibleOrigin + visibleSize/2 + Vec2(0,200));
    text->setColor(Style::Color::black);
    
    _confirmDeleteImagePopup->addChild(text);
    
    _confrimDeleteButton = ui::Button::create();
    _confrimDeleteButton->loadTextures(kArtAppAssetLoc + "stickerConfirm.png", kArtAppAssetLoc + "stickerConfirm.png");
    _confrimDeleteButton->setAnchorPoint(Vec2(1,0));
    _confrimDeleteButton->setPosition(background->getPosition() + Vec2(background->getContentSize().width/3, -background->getContentSize().height/3));
    _confrimDeleteButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onConfirmDeletePressed, this));
    
    _confirmDeleteImagePopup->addChild(_confrimDeleteButton);
    
    _cancelDeleteButton = ui::Button::create();
    _cancelDeleteButton->loadTextures(kArtAppAssetLoc + "close.png", kArtAppAssetLoc + "close.png");
    _cancelDeleteButton->setAnchorPoint(Vec2(0,0));
    _cancelDeleteButton->setPosition(background->getPosition() - Vec2(background->getContentSize().width/3, background->getContentSize().height/3));
    _cancelDeleteButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCancelDeletePressed, this));
    
    _confirmDeleteImagePopup->addChild(_cancelDeleteButton);
    
    _confirmDeleteImagePopup->setVisible(false);
    
    this->addChild(_confirmDeleteImagePopup,POPUP_UI_LAYER);
    
    
}

void DrawingCanvas::addColourSelectButtons(const Size& visibleSize, const Point& visibleOrigin)
{
    _selected = Sprite::create(kArtAppAssetLoc + "checkMark.png");
    _selected->setAnchorPoint(Vec2(0.5,0.5));
    _selected->setNormalizedPosition(Vec2(0.5,0.5));
    
    _leftBG = Sprite::create(kArtAppAssetLoc + "art_back_left.png");
    _leftBG->setAnchorPoint(Vec2(0,0));
    _leftBG->setPosition(Vec2(visibleOrigin.x, visibleOrigin.y - BOTTOM_UI_Y_OFFSET));
    _leftBG->setColor(Color3B::BLACK);
    this->addChild(_leftBG,MAIN_UI_LAYER);
    
    const Vec2& tableDimensions =  Vec2(7,ceilf(_kColours.size()/7.0f));
    
    _colourButtonLayout = Node::create();
    _colourButtonLayout->setContentSize(Size(visibleSize.width*0.75,visibleSize.height*(0.18*tableDimensions.y)));
    _colourButtonLayout->setAnchorPoint(Vec2(0.5,0.5));
    _colourButtonLayout->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y + visibleSize.height/2 + 100));
    _colourButtonLayout->setVisible(false);
    this->addChild(_colourButtonLayout,POPUP_UI_LAYER);
    
    ui::Scale9Sprite* colourBG = ui::Scale9Sprite::create(kArtAppAssetLoc + "gallery/art_painting_placeholder.png");
    colourBG->setContentSize(_colourButtonLayout->getContentSize());
    colourBG->setAnchorPoint(Vec2(0.5,0.5));
    colourBG->setNormalizedPosition(Vec2(0.5,0.5));
    _colourButtonLayout->addChild(colourBG);
    
    int colourCount = 0;
    
    for (int j = 1; j<=tableDimensions.y; j++)
    {
        for (int i = 0; i<tableDimensions.x; i++)
        {
            ui::Button* button = ui::Button::create();
            button->setAnchorPoint(Vec2(0.5,0.5));
            float xPos = (i+0.5) / tableDimensions.x;
            float yPos = (j-0.5) / tableDimensions.y;
            button->setNormalizedPosition(Vec2(xPos,yPos));
            button->loadTextures(kArtAppAssetLoc + "art_app_pallete_circle.png", kArtAppAssetLoc + "art_app_pallete_circle.png");
            button->setColor(_kColours[colourCount]);
            if(colourCount == 21)
            {
                _selectedColour = Color4F(_kColours[colourCount]);
                button->addChild(_selected);
            }
            button->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onColourChangePressed,this));
            _colourButtonLayout->addChild(button);
            
            colourCount++;
        }
    }
    
    ui::Button* closeButton = ui::Button::create();
    closeButton->setAnchorPoint(Vec2(0.5,0.5));
    closeButton->setPosition(_colourButtonLayout->getContentSize());
    closeButton->loadTextures(kArtAppAssetLoc + "close_button.png", kArtAppAssetLoc + "close_buton.png");
    closeButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCloseColourSelectPressed, this));
    _colourButtonLayout->addChild(closeButton);
    
    _colourSelectButton = ui::Button::create();
    _colourSelectButton->setAnchorPoint(Vec2(1,0));
    _colourSelectButton->setPosition(Vec2(_leftBG->getPosition().x + _leftBG->getContentSize().width/2, visibleOrigin.y));
    _colourSelectButton->loadTextures(kArtAppAssetLoc + "art_button_colour.png", kArtAppAssetLoc + "art_button_colour.png");
    _colourSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onColourSelectPressed,this));
    _colourSelectButton->setColor(Color3B(_selectedColour));
    
    this->addChild(_colourSelectButton,MAIN_UI_LAYER);
}

void DrawingCanvas::addToolSelectButtons(const Size& visibleSize, const Point& visibleOrigin)
{
    _toolBG = Sprite::create(kArtAppAssetLoc + "art_back_middle.png");
    _toolBG->setAnchorPoint(Vec2(0.5,0));
    _toolBG->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y - BOTTOM_UI_Y_OFFSET));
    _toolBG->setColor(Color3B::BLACK);
    
    this->addChild(_toolBG,MAIN_UI_LAYER);
    
    _toolButtonLayout = Node::create();
    _toolButtonLayout->setContentSize(_toolBG->getContentSize());
    _toolButtonLayout->setAnchorPoint(Vec2(0.5,0));
    _toolButtonLayout->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y - BOTTOM_UI_Y_OFFSET));
    
    this->addChild(_toolButtonLayout,MAIN_UI_LAYER);
    
    ui::Button* brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.1,0));
    brushButton->loadTextures(kArtAppAssetLoc + "pencil.png", kArtAppAssetLoc + "pencil.png");
    brushButton->setColor(Color3B(_selectedColour));
    brushButton->setScale(((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height) * 1.15);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,PEN));
    _selectedToolButton = brushButton;
    _toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.3,0));
    brushButton->loadTextures(kArtAppAssetLoc + "brush.png", kArtAppAssetLoc + "brush.png");
    brushButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,PAINTBRUSH));
    _toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.5,0));
    brushButton->loadTextures(kArtAppAssetLoc + "felt_tip.png", kArtAppAssetLoc + "felt_tip.png");
    brushButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,HIGHLIGHTER));
    _toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.7,0));
    brushButton->loadTextures(kArtAppAssetLoc + "can.png", kArtAppAssetLoc + "can.png");
    brushButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,SPRAYCAN));
    _toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.9,0));
    brushButton->loadTextures(kArtAppAssetLoc + "eraser.png", kArtAppAssetLoc + "eraser.png");
    brushButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->setName("eraser");
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,ERASER));
    _toolButtonLayout->addChild(brushButton);
}

void DrawingCanvas::addStickerSelectButtons(const Size& visibleSize, const Point& visibleOrigin)
{
    _addStickerButton = ui::Button::create();
    _addStickerButton->setAnchorPoint(Vec2(1,0));
    _addStickerButton->setPosition(Vec2(_leftBG->getPositionX() + _leftBG->getContentSize().width, _colourSelectButton->getPosition().y));
    _addStickerButton->loadTextures(kArtAppAssetLoc + "art_button_sticker.png", kArtAppAssetLoc + "art_button_sticker.png");
    _addStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onAddStickerButtonPressed,this));
    
    this->addChild(_addStickerButton,MAIN_UI_LAYER);
    
    _cancelStickerButton = ui::Button::create();
    _cancelStickerButton->setAnchorPoint(Vec2(0,0));
    _cancelStickerButton->setPosition(Vec2(visibleOrigin.x + BUTTON_OFFSET.x, visibleOrigin.y + BUTTON_OFFSET.y));
    _cancelStickerButton->loadTextures(kArtAppAssetLoc + "delete.png", kArtAppAssetLoc + "delete.png");
    _cancelStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCancelStickerPressed,this));
    _cancelStickerButton->setVisible(false);
    this->addChild(_cancelStickerButton,STICKER_UI_LAYER);
    
    _confirmStickerButton = ui::Button::create();
    _confirmStickerButton->setAnchorPoint(Vec2(1,0));
    _confirmStickerButton->setPosition(Vec2(visibleOrigin.x + visibleSize.width - BUTTON_OFFSET.x, visibleOrigin.y + BUTTON_OFFSET.y));
    _confirmStickerButton->loadTextures(kArtAppAssetLoc + "confirm.png", kArtAppAssetLoc + "confirm.png");
    _confirmStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onConfirmStickerPressed,this));
    _confirmStickerButton->setVisible(false);
    this->addChild(_confirmStickerButton,STICKER_UI_LAYER);
    
    _stickerScrollView = cocos2d::ui::ScrollView::create();
    _stickerScrollView->setContentSize(Size(visibleSize.width*2/3.0f, visibleSize.height/2));
    _stickerScrollView->setInnerContainerSize(Size(visibleSize.width*2, visibleSize.height/2));
    _stickerScrollView->setAnchorPoint(Vec2(0.5,0.5));
    _stickerScrollView->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y + visibleSize.height/2));
    _stickerScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    _stickerScrollView->setBounceEnabled(true);
    _stickerScrollView->setTouchEnabled(true);
    _stickerScrollView->setSwallowTouches(true);
    _stickerScrollView->setScrollBarEnabled(false);
    _stickerScrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    _stickerScrollView->setBackGroundColor(Color3B::WHITE);
    _stickerScrollView->setBackGroundColorOpacity(0);
    _stickerScrollView->setBackGroundImageScale9Enabled(true);
    _stickerScrollView->setBackGroundImage(kArtAppAssetLoc + "gallery/art_painting_placeholder.png");
    _stickerScrollView->setVisible(false);
    this->addChild(_stickerScrollView,POPUP_UI_LAYER);
    
    //load stickers
    
    _stickerCategoryLayout = Node::create();
    _stickerCategoryLayout->setAnchorPoint(Vec2(0.5,0));
    _stickerCategoryLayout->setContentSize(Size(visibleSize.width*2/3, visibleSize.height/5));
    _stickerCategoryLayout->setPosition(Vec2(_stickerScrollView->getPosition().x,_stickerScrollView->getPosition().y + _stickerScrollView->getContentSize().height/2));
    _stickerCategoryLayout->setVisible(false);
    
    _stickerCatBG = ui::Scale9Sprite::create(kArtAppAssetLoc + "gallery/art_painting_placeholder.png");
    _stickerCatBG->setColor(Color3B(125,125,125));
    _stickerCatBG->setContentSize(_stickerCategoryLayout->getContentSize());
    _stickerCatBG->setAnchorPoint(Vec2(0.5,0));
    _stickerCatBG->setPosition(_stickerCategoryLayout->getPosition());
    _stickerCatBG->setVisible(false);
    
    this->addChild(_stickerCatBG,POPUP_UI_LAYER);
    this->addChild(_stickerCategoryLayout,POPUP_UI_LAYER);
    
    _stickerCats = getStickerFilesFromJSON();
    
    _selectionIndicator = Sprite::create(kArtAppAssetLoc + "popup/art_selection_indicator.png");
    _selectionIndicator->setAnchorPoint(Vec2(0.5,0));
    _selectionIndicator->setVisible(false);
    _selectionIndicator->setPositionY(_stickerScrollView->getPosition().y + _stickerScrollView->getContentSize().height/2);
    _stickerCatBG->addChild(_selectionIndicator);

    for(int i = 0; i < _stickerCats.size(); i++)
    {
        ui::Button* stickerCatButton = ui::Button::create();
        stickerCatButton->loadTextures(kStickerLoc + _stickerCats[i].first, kStickerLoc + _stickerCats[i].first);
        stickerCatButton->setAnchorPoint(Vec2(0.5,0.5));
        stickerCatButton->setNormalizedPosition(Vec2((i+0.5f)/(float)_stickerCats.size(),0.5));
        stickerCatButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onStickerCategoryChangePressed, this,i));
        _stickerCategoryLayout->addChild(stickerCatButton);
        if(i == 0)
        {
            this->onStickerCategoryChangePressed(stickerCatButton, cocos2d::ui::Widget::TouchEventType::ENDED, i); //load first sticker set
            _selectionIndicator->setNormalizedPosition(Vec2((i+0.5f)/(float)_stickerCats.size(),0));
        }
    }
    
    _closeStickerSelectButton = ui::Button::create();
    _closeStickerSelectButton->setAnchorPoint(Vec2(0,0.5));
    _closeStickerSelectButton->setPosition(_stickerCategoryLayout->getPosition() + _stickerCategoryLayout->getContentSize()/2 + Vec2(50,0) );
    _closeStickerSelectButton->loadTextures(kArtAppAssetLoc + "close.png", kArtAppAssetLoc + "close.png");
    _closeStickerSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCloseStickerSelectPressed, this));
    _closeStickerSelectButton->setVisible(false);
    
    this->addChild(_closeStickerSelectButton,POPUP_UI_LAYER);
}

void DrawingCanvas::addBrushRadiusSlider(const Size& visibleSize, const Point& visibleOrigin)
{
    _brushSizeSlider = ui::Slider::create();
    _brushSizeSlider->setTouchEnabled(true);
    _brushSizeSlider->loadBarTexture(kArtAppAssetLoc + "slideBack.png");
    _brushSizeSlider->loadSlidBallTextures(kArtAppAssetLoc + "sliderIcon.png",kArtAppAssetLoc + "sliderIcon.png","");
    _brushSizeSlider->setPercent(50);
    _brushRadius = INITIAL_RADIUS + _brushSizeSlider->getPercent();
    _brushSizeSlider->setAnchorPoint(Vec2(0.5,0.5));
    _brushSizeSlider->setRotation(-90);
    _brushSizeSlider->setPosition(Vec2(visibleOrigin.x + _brushSizeSlider->getContentSize().height,visibleOrigin.y + visibleSize.height/2));
    _brushSizeSlider->addEventListener(CC_CALLBACK_2(DrawingCanvas::onRadiusSliderInteract, this));
    
    this->addChild(_brushSizeSlider,MAIN_UI_LAYER);
}

/*************************************************************************************
 ***********************   BUTTON CALLBACKS   ****************************************
 *************************************************************************************/

void DrawingCanvas::onClearButtonPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        _drawCanvasTouchListener->setEnabled(false);
        _confirmDeleteImagePopup->setVisible(true);
        setUIEnabled(false);
    }
}

void DrawingCanvas::onUndoButtonPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        this->removeChild(_drawingStack.back());
        _drawingStack.pop_back();
        if(_drawingStack.size() == 0)
        {
            _undoButton->setEnabled(false);
        }
        
        _actionCounter--;
    }
}

void DrawingCanvas::onColourChangePressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();

    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        _selectedColour = Color4F(pressedButton->getColor());
        _selected->retain();
        _selected->removeFromParent();
        pressedButton->addChild(_selected);
        _selected->release();
        pressedButton->setScale(baseScale / 0.85f);
        
        _colourButtonLayout->setVisible(false);
        _colourSelectButton->setColor(Color3B(_selectedColour));
        setUIEnabled(true);
        _selectedToolButton->setColor(Color3B(_selectedColour));
        _drawCanvasTouchListener->setEnabled(true);
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onColourSelectPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        setUIEnabled(false);
        _colourButtonLayout->setVisible(true);
        _drawCanvasTouchListener->setEnabled(false);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onCloseColourSelectPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        
        setUIEnabled(true);
        _colourButtonLayout->setVisible(false);
        _drawCanvasTouchListener->setEnabled(true);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onToolSelectPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        _colourSelectButton->setVisible(false);
        _addStickerButton->setVisible(false);
        _toolButtonLayout->setVisible(true);
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onAddStickerPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        _stickerScrollView->setVisible(false);
        _stickerCategoryLayout->setVisible(false);
        _closeStickerSelectButton->setVisible(false);
        _stickerCatBG->setVisible(false);
        _selectionIndicator->setVisible(false);
        setUIVisible(false);
        _overlay->setVisible(false);
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(_overlay);
        _stickerNode->setVisible(true);
        _confirmStickerButton->setVisible(true);
        _cancelStickerButton->setVisible(true);
        _stickerNode->setTouchListenerEnabled(true);
        Sprite* newSticker = Sprite::create(pressedButton->getNormalFile().file);
        newSticker->setAnchorPoint(Vec2(0.5,0.5));
        newSticker->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize()/2);
        _stickerNode->reset();
        _stickerNode->setSticker(newSticker);
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onAddStickerButtonPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        setUIEnabled(false);
        _stickerScrollView->setVisible(true);
        _stickerCategoryLayout->setVisible(true);
        _closeStickerSelectButton->setVisible(true);
        _stickerCatBG->setVisible(true);
        _selectionIndicator->setVisible(true);
        _drawCanvasTouchListener->setEnabled(false);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onCloseStickerSelectPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);

        setUIEnabled(true);
        _stickerScrollView->setVisible(false);
        _stickerCategoryLayout->setVisible(false);
        _closeStickerSelectButton->setVisible(false);
        _stickerCatBG->setVisible(false);
        _selectionIndicator->setVisible(false);
        _drawCanvasTouchListener->setEnabled(true);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onConfirmStickerPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        _stickerNode->setVisible(false);
        setUIVisible(true);
        setUIEnabled(true);
        _confirmStickerButton->setVisible(false);
        _cancelStickerButton->setVisible(false);
        _drawCanvasTouchListener->setEnabled(true);
        _stickerNode->setTouchListenerEnabled(false);
        
        //add sticker as node in drawing scene on undo stack
        
        if(_drawingStack.size() == 0)
        {
            _undoButton->setEnabled(true);
        }
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
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onCancelStickerPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        _stickerNode->setVisible(false);
        setUIVisible(true);
        setUIEnabled(true);
        _confirmStickerButton->setVisible(false);
        _cancelStickerButton->setVisible(false);
        _drawCanvasTouchListener->setEnabled(true);
        _stickerNode->setTouchListenerEnabled(false);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onConfirmDeletePressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        _confirmDeleteImagePopup->setVisible(false);
        setUIEnabled(true);
        _drawing->beginWithClear(Style::Color_4F::pureWhite.r,Style::Color_4F::pureWhite.g,Style::Color_4F::pureWhite.b,Style::Color_4F::pureWhite.a);
        _drawing->end();
        Director::getInstance()->getRenderer()->render();
        _drawCanvasTouchListener->setEnabled(true);
        _undoButton->setEnabled(false);
        _actionCounter++;
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onCancelDeletePressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        _confirmDeleteImagePopup->setVisible(false);
        setUIEnabled(true);
        _drawCanvasTouchListener->setEnabled(true);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onToolChanged(Ref *pSender, ui::Widget::TouchEventType eEventType, int index)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        if(pressedButton != _selectedToolButton)
        {
            pressedButton->setScale(baseScale * 1.15f);
        }
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        _activeBrush->getDrawNode()->removeFromParent();
        _activeBrush = _brushes[index];
        if(_activeBrush->_type == ERASER)
        {
            BrushEraser* eraser = (BrushEraser*)_activeBrush;
            eraser->setBgImageFile(_bgImageFilename);
        }
        this->addChild(_activeBrush->addDrawNode(Director::getInstance()->getVisibleSize()));
        
        if(pressedButton != _selectedToolButton)
        {
            _selectedToolButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/_selectedToolButton->getContentSize().height);
            if(_selectedToolButton->getName() != "eraser")
            {
                _selectedToolButton->setColor(Color3B::WHITE);
            }
            if(pressedButton->getName() != "eraser")
            {
                pressedButton->setColor(Color3B(_selectedColour));
            }
            _selectedToolButton = pressedButton;
            
        }
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        if(pressedButton != _selectedToolButton)
        {
            pressedButton->setScale(baseScale / 1.15f);
        }
    }
}

void DrawingCanvas::onRadiusSliderInteract(Ref *pSender, ui::Slider::EventType eEventType)
{
    ui::Slider* slider = static_cast<ui::Slider*>(pSender);
    
    if(eEventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        _brushRadius = INITIAL_RADIUS + slider->getPercent();
    }
}

void DrawingCanvas::onStickerCategoryChangePressed(Ref *pSender, ui::Widget::TouchEventType eEventType, int index)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(1.15f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        auto catButtons = _stickerCategoryLayout->getChildren();
        
        for(int i = 0; i < catButtons.size(); i++)
        {
            catButtons.at(i)->setScale(1);
        }
        
        pressedButton->setScale(1.15);
        
        _selectionIndicator->setNormalizedPosition(Vec2((index + 0.5f)/catButtons.size(),0));
        const Size& visibleSize = Director::getInstance()->getVisibleSize();
        
        int numStickers = (int)_stickerCats[index].second.size();
        
        _stickerScrollView->removeAllChildren();
        _stickerScrollView->setInnerContainerSize(Size(visibleSize.width/7.0f * numStickers/2.0f, visibleSize.height/2));
        
        for(int i = 0; i < numStickers; i+=2)
        {
            ui::Button* temp = ui::Button::create();
            temp->setAnchorPoint(Vec2(0.5,0.5));
            temp->loadTextures(kStickerLoc + _stickerCats[index].second[i],kStickerLoc + _stickerCats[index].second[i]);
            temp->setPosition(Vec2(_stickerScrollView->getInnerContainerSize().width*((i+1.0f)/(numStickers+1)),visibleSize.height/4 + temp->getContentSize().height/2));
            temp->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onAddStickerPressed, this));
            _stickerScrollView->addChild(temp);
            
            if(i+1 < numStickers)
            {
                ui::Button* temp2 = ui::Button::create();
                temp2->setAnchorPoint(Vec2(0.5,0.5));
                temp2->loadTextures(kStickerLoc + _stickerCats[index].second[i+1],kStickerLoc + _stickerCats[index].second[i+1]);
                temp2->setPosition(Vec2(_stickerScrollView->getInnerContainerSize().width*((i+1.0f)/(numStickers+1)),visibleSize.height/4 - temp2->getContentSize().height/2));
                temp2->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onAddStickerPressed, this));
                _stickerScrollView->addChild(temp2);
            }
            
        }
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(1.15f);
    }
}

void DrawingCanvas::setUIVisible(bool isVisible)
{
    _leftBG->setVisible(isVisible);
    _rightBG->setVisible(isVisible);
    _toolBG->setVisible(isVisible);
    _toolButtonLayout->setVisible(isVisible);
    _colourSelectButton->setVisible(isVisible);
    _addStickerButton->setVisible(isVisible);
    _clearButton->setVisible(isVisible);
    _undoButton->setVisible(isVisible);
    _brushSizeSlider->setVisible(isVisible);
}

void DrawingCanvas::setUIEnabled(bool isEnabled)
{
    auto toolButtons = _toolButtonLayout->getChildren();
    
    for(Node* btnNode : toolButtons)
    {
        ui::Widget* toolButt = (ui::Widget*)btnNode;
        toolButt->setEnabled(isEnabled);
    }
    _colourSelectButton->setEnabled(isEnabled);
    _addStickerButton->setEnabled(isEnabled);
    _clearButton->setEnabled(isEnabled);
    _brushSizeSlider->setEnabled(isEnabled);
    _overlay->setVisible(!isEnabled);
    if(isEnabled)
    {
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(_overlay);
    }
    else
    {
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(_overlay);
    }
}

//---------------------Sticker file collection methods ----------------------------------------//

StickerFileStore DrawingCanvas::getStickerFilesFromJSON()
{
    StickerFileStore fileStore = StickerFileStore();
    
    std::string fullFileText = FileUtils::getInstance()->getStringFromFile(kStickerLoc + "catalogue.json");
    
    rapidjson::Document json;
    json.Parse(fullFileText.c_str());
    
    const rapidjson::Value& categories = json["categories"];
    for(auto it = categories.Begin(); it != categories.End(); ++it)
    {
        std::string catName;
        std::vector<std::string> catStickers;
        const auto& jsonCatEntry = *it;
        catName = jsonCatEntry["image_location"].GetString();
        
        const rapidjson::Value& stickersJson = jsonCatEntry["stickers"];
        for(auto it = stickersJson.Begin(); it != stickersJson.End(); ++it)
        {
            const auto& jsonStickEntry = *it;
            const std::string& sticker = jsonStickEntry.GetString();
            catStickers.push_back(sticker);
        }
        
        StickerSet categorySet = StickerSet();
        categorySet.first = catName;
        categorySet.second = catStickers;
        
        fileStore.push_back(categorySet);
        
    }

    
    return fileStore;
}

NS_AZOOMEE_AA_END
