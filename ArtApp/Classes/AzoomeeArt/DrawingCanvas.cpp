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
        return false;
    
    background = LayerColor::create(Color4B(Style::Color_4F::pureWhite));
    
    this->addChild(background,BACKGROUND_LAYER);
    
    const Size& visibleSize = Director::getInstance()->getWinSize();
    
    drawing = RenderTexture::create(visibleSize.width, visibleSize.height);
    drawing->setAnchorPoint(Vec2(0.5,0.5));
    drawing->setPosition(visibleSize/2);
    drawing->beginWithClear(Style::Color_4F::pureWhite.r, Style::Color_4F::pureWhite.g, Style::Color_4F::pureWhite.b, Style::Color_4F::pureWhite.a);
    drawing->end();
    
    this->addChild(drawing);
    
    actionCounter = 0;
    
    return true;
}

void DrawingCanvas::onEnter()
{
    Super::onEnter();
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    this->setContentSize(visibleSize);
    
    addBrushes();
    
    
    for(Brush* b : brushes)
    {
        b->setupDrawNode(visibleSize);
    }
    
    setupTouchHandling();
    setupMenus();
    
    stickerNode = StickerPlacementNode::create();
    this->addChild(stickerNode,MAIN_UI_LAYER);
    stickerNode->setVisible(false);
    stickerNode->setTouchListenerEnabled(false);
}

void DrawingCanvas::onExit()
{
    Super::onExit();
    
    for(Brush* b : brushes)
    {
        delete b;
    }
    
    brushes.clear();
    
}

void DrawingCanvas::setBaseImage(const std::string& fileName)
{
    Director* director = Director::getInstance();
    
    Sprite* baseImage = Sprite::create(fileName);
    baseImage->setAnchorPoint(Vec2(0.5,0.5));
    baseImage->setPosition(director->getVisibleOrigin() + director->getVisibleSize()/2);
    baseImage->setScale(director->getWinSize().width/baseImage->getContentSize().width);
    drawing->begin();
    baseImage->visit();
    drawing->end();
    director->getRenderer()->render();
}

void DrawingCanvas::saveImage(const std::string& filePath)
{
    
    drawing->begin();
    for(Node* n : drawingStack)
    {
        n->visit();
    }
    drawing->end();
    Director::getInstance()->getRenderer()->render();
    
    Sprite* drawingSprite = Sprite::createWithTexture(drawing->getSprite()->getTexture());
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

    drawCanvasTouchListener = EventListenerTouchOneByOne::create();
    
    drawCanvasTouchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
            
        activeBrush->onTouchBegin(touch, event);

        return true;
    };
    
    drawCanvasTouchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        activeBrush->onTouchMoved(touch, event);
        
    };
    
    drawCanvasTouchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        this->getEventDispatcher()->setEnabled(false);
        
        activeBrush->onTouchEnded(touch, event);
        
        if(drawingStack.size() == 0)
        {
            clearButton->loadTextures(ArtAppAssetLoc + "art_button_undo.png", ArtAppAssetLoc + "undo.png");
        }
        drawingStack.push_back(activeBrush->getDrawNode());
        this->addChild(activeBrush->addDrawNode(Director::getInstance()->getVisibleSize()));
        
        if(drawingStack.size()>numberOfUndos)
        {
            Node* mergingLayer = drawingStack[0];
            drawingStack.erase(drawingStack.begin());
            drawing->begin();
            mergingLayer->visit();
            drawing->end();
            Director::getInstance()->getRenderer()->render();
            this->removeChild(mergingLayer);
        }
        this->getEventDispatcher()->setEnabled(true);
        
        actionCounter++;
        
    };
    drawCanvasTouchListener->onTouchCancelled = [&](Touch* touch, Event* event)
    {
        
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(drawCanvasTouchListener, this);
    
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
    brushes.clear();
    
    Brush* pen = new BrushPen();
    Brush* paintbrush = new BrushPaintBrush();
    Brush* highlighter = new BrushHighlighter();
    Brush* sprayCan = new BrushSprayCan();
    Brush* eraser = new BrushEraser();
    
    brushes.push_back(pen);
    brushes.push_back(paintbrush);
    brushes.push_back(highlighter);
    brushes.push_back(sprayCan);
    brushes.push_back(eraser);
    
    for (Brush* b : brushes) {
        b->init();
        b->setSelectedColour(&selectedColour);
        b->setBrushRadius(&brushRadius);
    }
    activeBrush = brushes[0];
    this->addChild(activeBrush->getDrawNode());
}

//UI LOADING

void DrawingCanvas::addClearButton(const Size& visibleSize, const Point& visibleOrigin)
{
    
    rightBG = Sprite::create(ArtAppAssetLoc + "art_back_right.png");
    rightBG->setAnchorPoint(Vec2(1,0));
    rightBG->setPosition(visibleOrigin.x + visibleSize.width,visibleOrigin.y - BOTTOM_UI_Y_OFFSET);
    this->addChild(rightBG,MAIN_UI_LAYER);
    
    clearButton = ui::Button::create();
    clearButton->setAnchorPoint(Vec2(0.5,0));
    clearButton->setPosition(Vec2(rightBG->getPosition().x - rightBG->getContentSize().width/2, visibleOrigin.y));
    clearButton->loadTextures(ArtAppAssetLoc + "bin.png", ArtAppAssetLoc + "bin.png");
    clearButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onClearButtonPressed, this));
    this->addChild(clearButton,MAIN_UI_LAYER);
    
    confirmDeleteImagePopup = Node::create();
    confirmDeleteImagePopup->setContentSize(Size(visibleSize.width/2, visibleSize.height/2));
    ui::Scale9Sprite* background = ui::Scale9Sprite::create(ArtAppAssetLoc + "gallery/art_painting_placeholder.png");
    background->setContentSize(Size(visibleSize.width/2, visibleSize.height/2));
    background->setAnchorPoint(Vec2(0.5,0.5));
    background->setPosition(visibleOrigin + visibleSize/2);
    confirmDeleteImagePopup->addChild(background);
    
    Label* text = Label::createWithTTF("Delete Drawing?", Style::Font::Regular, 128);
    text->setAnchorPoint(Vec2(0.5,0.5));
    text->setPosition(visibleOrigin + visibleSize/2 + Vec2(0,200));
    text->setColor(Style::Color::black);
    
    confirmDeleteImagePopup->addChild(text);
    
    confrimDeleteButton = ui::Button::create();
    confrimDeleteButton->loadTextures(ArtAppAssetLoc + "stickerConfirm.png", ArtAppAssetLoc + "stickerConfirm.png");
    confrimDeleteButton->setAnchorPoint(Vec2(1,0));
    confrimDeleteButton->setPosition(background->getPosition() + Vec2(background->getContentSize().width/3, -background->getContentSize().height/3));
    confrimDeleteButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onConfirmDeletePressed, this));
    
    confirmDeleteImagePopup->addChild(confrimDeleteButton);
    
    cancelDeleteButton = ui::Button::create();
    cancelDeleteButton->loadTextures(ArtAppAssetLoc + "close.png", ArtAppAssetLoc + "close.png");
    cancelDeleteButton->setAnchorPoint(Vec2(0,0));
    cancelDeleteButton->setPosition(background->getPosition() - Vec2(background->getContentSize().width/3, background->getContentSize().height/3));
    cancelDeleteButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCancelDeletePressed, this));
    
    confirmDeleteImagePopup->addChild(cancelDeleteButton);
    
    confirmDeleteImagePopup->setVisible(false);
    
    this->addChild(confirmDeleteImagePopup,POPUP_UI_LAYER);
    
    
}

void DrawingCanvas::addColourSelectButtons(const Size& visibleSize, const Point& visibleOrigin)
{
    selected = Sprite::create(ArtAppAssetLoc + "checkMark.png");
    selected->setAnchorPoint(Vec2(0.5,0.5));
    selected->setNormalizedPosition(Vec2(0.5,0.5));
    
    leftBG = Sprite::create(ArtAppAssetLoc + "art_back_left.png");
    leftBG->setAnchorPoint(Vec2(0,0));
    leftBG->setPosition(Vec2(visibleOrigin.x, visibleOrigin.y - BOTTOM_UI_Y_OFFSET));
    this->addChild(leftBG,MAIN_UI_LAYER);
    
    std::vector<Color3B> colours;
    colours.push_back(Color3B(Style::Color_4F::darkBrown));
    colours.push_back(Color3B(Style::Color_4F::brown));
    colours.push_back(Color3B(Style::Color_4F::lightBrown));
    colours.push_back(Color3B(Style::Color_4F::orangeBrown));
    colours.push_back(Color3B(Style::Color_4F::orange));
    colours.push_back(Color3B(Style::Color_4F::darkYellow));
    colours.push_back(Color3B(Style::Color_4F::yellow));
    
    colours.push_back(Color3B(Style::Color_4F::darkBlue));
    colours.push_back(Color3B(Style::Color_4F::blue));
    colours.push_back(Color3B(Style::Color_4F::lightBlue));
    colours.push_back(Color3B(Style::Color_4F::greenBlue));
    colours.push_back(Color3B(Style::Color_4F::green));
    colours.push_back(Color3B(Style::Color_4F::grassGreen));
    colours.push_back(Color3B(Style::Color_4F::neonGreen));
    
    colours.push_back(Color3B(Style::Color_4F::darkPurple));
    colours.push_back(Color3B(Style::Color_4F::purple));
    colours.push_back(Color3B(Style::Color_4F::neonPink));
    colours.push_back(Color3B(Style::Color_4F::palePink));
    colours.push_back(Color3B(Style::Color_4F::pink));
    colours.push_back(Color3B(Style::Color_4F::darkPink));
    colours.push_back(Color3B(Style::Color_4F::red));
    
    colours.push_back(Color3B(Style::Color_4F::neonBlue));
    colours.push_back(Color3B(Style::Color_4F::paleGreen));
    colours.push_back(Color3B(Style::Color_4F::paleYellow));
    colours.push_back(Color3B(Style::Color_4F::lightGrey));
    colours.push_back(Color3B(Style::Color_4F::grey));
    colours.push_back(Color3B(Style::Color_4F::darkGrey));
    colours.push_back(Color3B(Style::Color_4F::black));
    
    const int rows = ceilf(colours.size()/7.0f);
    const int columns = 7;
    
    colourButtonLayout = Node::create();
    colourButtonLayout->setContentSize(Size(visibleSize.width*0.7,visibleSize.height*(0.2*rows)));
    colourButtonLayout->setAnchorPoint(Vec2(0.5,0.5));
    colourButtonLayout->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y + visibleSize.height/2 + 100));
    colourButtonLayout->setVisible(false);
    this->addChild(colourButtonLayout,POPUP_UI_LAYER);
    
    ui::Scale9Sprite* colourBG = ui::Scale9Sprite::create(ArtAppAssetLoc + "gallery/art_painting_placeholder.png");
    colourBG->setContentSize(colourButtonLayout->getContentSize());
    colourBG->setAnchorPoint(Vec2(0.5,0.5));
    colourBG->setNormalizedPosition(Vec2(0.5,0.5));
    colourButtonLayout->addChild(colourBG);
    
    int colourCount = 0;
    
    for (int j = 1; j<=rows; j++)
    {
        for (int i = 0; i<columns; i++)
        {
            ui::Button* button = ui::Button::create();
            button->setAnchorPoint(Vec2(0,0.5));
            float xPos = i / (float)columns;
            float yPos = (j-0.5) / (float)rows;
            button->setNormalizedPosition(Vec2(xPos,yPos));
            button->loadTextures(ArtAppAssetLoc + "art_app_pallete_circle.png", ArtAppAssetLoc + "art_app_pallete_circle.png");
            button->setColor(colours[colourCount]);
            if(colourCount == 21)
            {
                selectedColour = Color4F(colours[colourCount]);
                button->addChild(selected);
            }
            button->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onColourChangePressed,this));
            colourButtonLayout->addChild(button);
            
            colourCount++;
        }
    }
    
    ui::Button* closeButton = ui::Button::create();
    closeButton->setAnchorPoint(Vec2(0,1));
    closeButton->setPosition(colourButtonLayout->getContentSize());
    closeButton->loadTextures(ArtAppAssetLoc + "close.png", ArtAppAssetLoc + "close.png");
    closeButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCloseColourSelectPressed, this));
    colourButtonLayout->addChild(closeButton);
    
    colourSelectButton = ui::Button::create();
    colourSelectButton->setAnchorPoint(Vec2(1,0));
    colourSelectButton->setPosition(Vec2(leftBG->getPosition().x + leftBG->getContentSize().width/2, visibleOrigin.y));
    colourSelectButton->loadTextures(ArtAppAssetLoc + "art_button_colour.png", ArtAppAssetLoc + "art_button_colour.png");
    colourSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onColourSelectPressed,this));
    colourSelectButton->setColor(Color3B(selectedColour));
    
    this->addChild(colourSelectButton,MAIN_UI_LAYER);
}

void DrawingCanvas::addToolSelectButtons(const Size& visibleSize, const Point& visibleOrigin)
{
    toolBG = Sprite::create(ArtAppAssetLoc + "art_back_middle.png");
    toolBG->setAnchorPoint(Vec2(0.5,0));
    toolBG->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y - BOTTOM_UI_Y_OFFSET));
    
    this->addChild(toolBG,MAIN_UI_LAYER);
    
    toolButtonLayout = Node::create();
    toolButtonLayout->setContentSize(toolBG->getContentSize());
    toolButtonLayout->setAnchorPoint(Vec2(0.5,0));
    toolButtonLayout->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y - BOTTOM_UI_Y_OFFSET));
    
    this->addChild(toolButtonLayout,MAIN_UI_LAYER);
    
    ui::Button* brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.1,0));
    brushButton->loadTextures(ArtAppAssetLoc + "brushes/pencil.png", ArtAppAssetLoc + "brushes/pencil.png");
    brushButton->setColor(Color3B(selectedColour));
    brushButton->setScale(((toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height) * 1.15);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,PEN));
    SelectedToolButton = brushButton;
    toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.3,0));
    brushButton->loadTextures(ArtAppAssetLoc + "brushes/brush.png", ArtAppAssetLoc + "brushes/brush.png");
    brushButton->setColor(Color3B(Style::Color_4F::defaultBrush));
    brushButton->setScale((toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,PAINTBRUSH));
    toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.5,0));
    brushButton->loadTextures(ArtAppAssetLoc + "brushes/highlighter.png", ArtAppAssetLoc + "brushes/highlighter.png");
    brushButton->setColor(Color3B(Style::Color_4F::defaultBrush));
    brushButton->setScale((toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,HIGHLIGHTER));
    toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.7,0));
    brushButton->loadTextures(ArtAppAssetLoc + "brushes/spray.png", ArtAppAssetLoc + "brushes/spray.png");
    brushButton->setColor(Color3B(Style::Color_4F::defaultBrush));
    brushButton->setScale((toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,SPRAYCAN));
    toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.9,0));
    brushButton->loadTextures(ArtAppAssetLoc + "brushes/eraser.png", ArtAppAssetLoc + "brushes/eraser.png");
    brushButton->setScale((toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->setName("eraser");
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,ERASER));
    toolButtonLayout->addChild(brushButton);
}

void DrawingCanvas::addStickerSelectButtons(const Size& visibleSize, const Point& visibleOrigin)
{
    addStickerButton = ui::Button::create();
    addStickerButton->setAnchorPoint(Vec2(1,0));
    addStickerButton->setPosition(Vec2(leftBG->getPositionX() + leftBG->getContentSize().width, colourSelectButton->getPosition().y));
    addStickerButton->loadTextures(ArtAppAssetLoc + "art_button_sticker.png", ArtAppAssetLoc + "art_button_sticker.png");
    addStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onAddStickerButtonPressed,this));
    
    this->addChild(addStickerButton,MAIN_UI_LAYER);
    
    cancelStickerButton = ui::Button::create();
    cancelStickerButton->setAnchorPoint(Vec2(0,0));
    cancelStickerButton->setPosition(Vec2(visibleOrigin.x + BUTTON_OFFSET.x, visibleOrigin.y + BUTTON_OFFSET.y));
    cancelStickerButton->loadTextures(ArtAppAssetLoc + "stickerBin.png", ArtAppAssetLoc + "stickerBin.png");
    cancelStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCancelStickerPressed,this));
    cancelStickerButton->setVisible(false);
    this->addChild(cancelStickerButton,STICKER_UI_LAYER);
    
    confirmStickerButton = ui::Button::create();
    confirmStickerButton->setAnchorPoint(Vec2(1,0));
    confirmStickerButton->setPosition(Vec2(visibleOrigin.x + visibleSize.width - BUTTON_OFFSET.x, visibleOrigin.y + BUTTON_OFFSET.y));
    confirmStickerButton->loadTextures(ArtAppAssetLoc + "stickerConfirm.png", ArtAppAssetLoc + "stickerConfirm.png");
    confirmStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onConfirmStickerPressed,this));
    confirmStickerButton->setVisible(false);
    this->addChild(confirmStickerButton,STICKER_UI_LAYER);
    
    stickerScrollView = cocos2d::ui::ScrollView::create();
    stickerScrollView->setContentSize(Size(visibleSize.width*2/3.0f, visibleSize.height/2));
    stickerScrollView->setInnerContainerSize(Size(visibleSize.width*2, visibleSize.height/2));
    stickerScrollView->setAnchorPoint(Vec2(0.5,0.5));
    stickerScrollView->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y + visibleSize.height/2));
    stickerScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    stickerScrollView->setBounceEnabled(true);
    stickerScrollView->setTouchEnabled(true);
    stickerScrollView->setSwallowTouches(true);
    stickerScrollView->setScrollBarEnabled(false);
    stickerScrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    stickerScrollView->setBackGroundColor(Color3B::WHITE);
    stickerScrollView->setBackGroundColorOpacity(0);
    stickerScrollView->setBackGroundImageScale9Enabled(true);
    stickerScrollView->setBackGroundImage(ArtAppAssetLoc + "gallery/art_painting_placeholder.png");
    stickerScrollView->setVisible(false);
    this->addChild(stickerScrollView,POPUP_UI_LAYER);
    
    //load stickers
    
    StickerCategoryLayout = Node::create();
    StickerCategoryLayout->setAnchorPoint(Vec2(0.5,0));
    StickerCategoryLayout->setContentSize(Size(visibleSize.width*2/3, visibleSize.height/5));
    StickerCategoryLayout->setPosition(Vec2(stickerScrollView->getPosition().x,stickerScrollView->getPosition().y + stickerScrollView->getContentSize().height/2));
    StickerCategoryLayout->setVisible(false);
    
    stickerCatBG = ui::Scale9Sprite::create(ArtAppAssetLoc + "gallery/art_painting_placeholder.png");
    stickerCatBG->setColor(Color3B(125,125,125));
    stickerCatBG->setContentSize(StickerCategoryLayout->getContentSize());
    stickerCatBG->setAnchorPoint(Vec2(0.5,0));
    stickerCatBG->setPosition(StickerCategoryLayout->getPosition());
    stickerCatBG->setVisible(false);
    
    this->addChild(stickerCatBG,POPUP_UI_LAYER);
    this->addChild(StickerCategoryLayout,POPUP_UI_LAYER);
    
    stickerCats = getStickerFilesFromJSON();
    
    selectionIndicator = Sprite::create(ArtAppAssetLoc + "popup/art_selection_indicator.png");
    selectionIndicator->setAnchorPoint(Vec2(0.5,0));
    selectionIndicator->setVisible(false);
    selectionIndicator->setPositionY(stickerScrollView->getPosition().y + stickerScrollView->getContentSize().height/2);
    stickerCatBG->addChild(selectionIndicator);

    for(int i = 0; i < stickerCats.size(); i++)
    {
        ui::Button* stickerCatButton = ui::Button::create();
        stickerCatButton->loadTextures(stickerLoc + stickerCats[i].first, stickerLoc + stickerCats[i].first);
        stickerCatButton->setAnchorPoint(Vec2(0.5,0.5));
        stickerCatButton->setNormalizedPosition(Vec2((i+0.5f)/(float)stickerCats.size(),0.5));
        stickerCatButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onStickerCategoryChangePressed, this,i));
        StickerCategoryLayout->addChild(stickerCatButton);
        if(i == 0)
        {
            this->onStickerCategoryChangePressed(stickerCatButton, cocos2d::ui::Widget::TouchEventType::ENDED, i); //load first sticker set
            selectionIndicator->setNormalizedPosition(Vec2((i+0.5f)/(float)stickerCats.size(),0));
        }
    }
    
    closeStickerSelectButton = ui::Button::create();
    closeStickerSelectButton->setAnchorPoint(Vec2(0,0.5));
    closeStickerSelectButton->setPosition(StickerCategoryLayout->getPosition() + StickerCategoryLayout->getContentSize()/2 + Vec2(50,0) );
    closeStickerSelectButton->loadTextures(ArtAppAssetLoc + "close.png", ArtAppAssetLoc + "close.png");
    closeStickerSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCloseStickerSelectPressed, this));
    closeStickerSelectButton->setVisible(false);
    
    this->addChild(closeStickerSelectButton,POPUP_UI_LAYER);
}

void DrawingCanvas::addBrushRadiusSlider(const Size& visibleSize, const Point& visibleOrigin)
{
    brushSizeSlider = ui::Slider::create();
    brushSizeSlider->setTouchEnabled(true);
    brushSizeSlider->loadBarTexture(ArtAppAssetLoc + "slideBack.png");
    brushSizeSlider->loadSlidBallTextures(ArtAppAssetLoc + "sliderIcon.png",ArtAppAssetLoc + "sliderIcon.png","");
    brushSizeSlider->setPercent(50);
    brushRadius = INITIAL_RADIUS + brushSizeSlider->getPercent();
    brushSizeSlider->setAnchorPoint(Vec2(0.5,0.5));
    brushSizeSlider->setRotation(-90);
    brushSizeSlider->setPosition(Vec2(visibleOrigin.x + brushSizeSlider->getContentSize().height,visibleOrigin.y + visibleSize.height/2));
    brushSizeSlider->addEventListener(CC_CALLBACK_2(DrawingCanvas::onRadiusSliderInteract, this));
    
    this->addChild(brushSizeSlider,MAIN_UI_LAYER);
}

/*************************************************************************************
 ***********************   BUTTON CALLBACKS   ****************************************
 *************************************************************************************/

void DrawingCanvas::onClearButtonPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        if(drawingStack.size() == 0)
        {
            drawCanvasTouchListener->setEnabled(false);
            confirmDeleteImagePopup->setVisible(true);
            setUIEnabled(false);
        }
        else
        {
            this->removeChild(drawingStack.back());
            drawingStack.pop_back();
            if(drawingStack.size() == 0)
                clearButton->loadTextures(ArtAppAssetLoc + "art_button_bin.png", ArtAppAssetLoc + "bin.png");
            
            actionCounter--;
        }
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
        selectedColour = Color4F(pressedButton->getColor());
        selected->retain();
        selected->removeFromParent();
        pressedButton->addChild(selected);
        selected->release();
        pressedButton->setScale(baseScale / 0.85f);
        
        colourButtonLayout->setVisible(false);
        colourSelectButton->setColor(Color3B(selectedColour));
        setUIEnabled(true);
        SelectedToolButton->setColor(Color3B(selectedColour));
        drawCanvasTouchListener->setEnabled(true);
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
        colourButtonLayout->setVisible(true);
        drawCanvasTouchListener->setEnabled(false);
        
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
        colourButtonLayout->setVisible(false);
        drawCanvasTouchListener->setEnabled(true);
        
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
        colourSelectButton->setVisible(false);
        addStickerButton->setVisible(false);
        toolButtonLayout->setVisible(true);
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
        stickerScrollView->setVisible(false);
        StickerCategoryLayout->setVisible(false);
        closeStickerSelectButton->setVisible(false);
        stickerCatBG->setVisible(false);
        selectionIndicator->setVisible(false);
        setUIVisible(false);
        stickerNode->setVisible(true);
        confirmStickerButton->setVisible(true);
        cancelStickerButton->setVisible(true);
        stickerNode->setTouchListenerEnabled(true);
        Sprite* newSticker = Sprite::create(pressedButton->getNormalFile().file);
        newSticker->setAnchorPoint(Vec2(0.5,0.5));
        newSticker->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize()/2);
        stickerNode->setSticker(newSticker);
        stickerNode->reset();
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
        stickerScrollView->setVisible(true);
        StickerCategoryLayout->setVisible(true);
        closeStickerSelectButton->setVisible(true);
        stickerCatBG->setVisible(true);
        selectionIndicator->setVisible(true);
        drawCanvasTouchListener->setEnabled(false);
        
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
        stickerScrollView->setVisible(false);
        StickerCategoryLayout->setVisible(false);
        closeStickerSelectButton->setVisible(false);
        stickerCatBG->setVisible(false);
        selectionIndicator->setVisible(false);
        drawCanvasTouchListener->setEnabled(true);
        
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
        stickerNode->setVisible(false);
        setUIVisible(true);
        setUIEnabled(true);
        confirmStickerButton->setVisible(false);
        cancelStickerButton->setVisible(false);
        drawCanvasTouchListener->setEnabled(true);
        stickerNode->setTouchListenerEnabled(false);
        
        //add sticker as node in drawing scene on undo stack
        
        if(drawingStack.size() == 0)
            clearButton->loadTextures(ArtAppAssetLoc + "art_button_undo.png", ArtAppAssetLoc + "undo.png");
        drawingStack.push_back(stickerNode->getSticker());
        Sprite* temp = stickerNode->getSticker();
        temp->retain(); //move sticker from sticker node to drawing canvas
        temp->removeFromParent();
        this->addChild(temp);
        temp->release();
        if(drawingStack.size()>numberOfUndos)
        {
            Node* mergingLayer = drawingStack[0];
            drawingStack.erase(drawingStack.begin());
            drawing->begin();
            mergingLayer->visit();
            drawing->end();
            Director::getInstance()->getRenderer()->render();
            this->removeChild(mergingLayer);
        }
        this->removeChild(activeBrush->getDrawNode());
        this->addChild(activeBrush->addDrawNode(Director::getInstance()->getVisibleSize()));
        
        actionCounter++;
        
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
        stickerNode->setVisible(false);
        setUIVisible(true);
        setUIEnabled(true);
        confirmStickerButton->setVisible(false);
        cancelStickerButton->setVisible(false);
        drawCanvasTouchListener->setEnabled(true);
        stickerNode->setTouchListenerEnabled(false);
        
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
        confirmDeleteImagePopup->setVisible(false);
        setUIEnabled(true);
        drawing->beginWithClear(Style::Color_4F::pureWhite.r,Style::Color_4F::pureWhite.g,Style::Color_4F::pureWhite.b,Style::Color_4F::pureWhite.a);
        drawing->end();
        Director::getInstance()->getRenderer()->render();
        drawCanvasTouchListener->setEnabled(true);
        
        actionCounter++;
        
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
        confirmDeleteImagePopup->setVisible(false);
        setUIEnabled(true);
        drawCanvasTouchListener->setEnabled(true);
        
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
        if(pressedButton != SelectedToolButton)
            pressedButton->setScale(baseScale * 1.15f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        activeBrush->getDrawNode()->removeFromParent();
        activeBrush = brushes[index];
        this->addChild(activeBrush->addDrawNode(Director::getInstance()->getVisibleSize()));
        
        if(pressedButton != SelectedToolButton)
        {
            SelectedToolButton->setScale((toolButtonLayout->getContentSize().height*0.8)/SelectedToolButton->getContentSize().height);
            if(SelectedToolButton->getName() != "eraser")
                SelectedToolButton->setColor(Color3B(Style::Color_4F::defaultBrush));
            if(pressedButton->getName() != "eraser")
                pressedButton->setColor(Color3B(selectedColour));
            SelectedToolButton = pressedButton;
            
        }
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        if(pressedButton != SelectedToolButton)
            pressedButton->setScale(baseScale / 1.15f);
    }
}

void DrawingCanvas::onRadiusSliderInteract(Ref *pSender, ui::Slider::EventType eEventType)
{
    ui::Slider* slider = static_cast<ui::Slider*>(pSender);
    
    if(eEventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        brushRadius = INITIAL_RADIUS + slider->getPercent();
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
        auto catButtons = StickerCategoryLayout->getChildren();
        
        for(int i = 0; i < catButtons.size(); i++)
        {
            catButtons.at(i)->setScale(1);
        }
        
        pressedButton->setScale(1.15);
        
        selectionIndicator->setNormalizedPosition(Vec2((index + 0.5f)/catButtons.size(),0));
        const Size& visibleSize = Director::getInstance()->getVisibleSize();
        
        int numStickers = (int)stickerCats[index].second.size();
        
        stickerScrollView->removeAllChildren();
        stickerScrollView->setInnerContainerSize(Size(visibleSize.width/7.0f * numStickers/2.0f, visibleSize.height/2));
        
        for(int i = 0; i < numStickers; i+=2)
        {
            ui::Button* temp = ui::Button::create();
            temp->setAnchorPoint(Vec2(0.5,0.5));
            temp->loadTextures(stickerLoc + stickerCats[index].second[i],stickerLoc + stickerCats[index].second[i]);
            temp->setPosition(Vec2(stickerScrollView->getInnerContainerSize().width*((i+1.0f)/(numStickers+1)),visibleSize.height/4 + temp->getContentSize().height/2));
            temp->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onAddStickerPressed, this));
            stickerScrollView->addChild(temp);
            
            if(i+1 < numStickers)
            {
                ui::Button* temp2 = ui::Button::create();
                temp2->setAnchorPoint(Vec2(0.5,0.5));
                temp2->loadTextures(stickerLoc + stickerCats[index].second[i+1],stickerLoc + stickerCats[index].second[i+1]);
                temp2->setPosition(Vec2(stickerScrollView->getInnerContainerSize().width*((i+1.0f)/(numStickers+1)),visibleSize.height/4 - temp2->getContentSize().height/2));
                temp2->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onAddStickerPressed, this));
                stickerScrollView->addChild(temp2);
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
    leftBG->setVisible(isVisible);
    rightBG->setVisible(isVisible);
    toolBG->setVisible(isVisible);
    toolButtonLayout->setVisible(isVisible);
    colourSelectButton->setVisible(isVisible);
    addStickerButton->setVisible(isVisible);
    clearButton->setVisible(isVisible);
    brushSizeSlider->setVisible(isVisible);
}

void DrawingCanvas::setUIEnabled(bool isEnabled)
{
    auto toolButtons = toolButtonLayout->getChildren();
    
    for(Node* btnNode : toolButtons)
    {
        ui::Widget* toolButt = (ui::Widget*)btnNode;
        toolButt->setEnabled(isEnabled);
    }
    colourSelectButton->setEnabled(isEnabled);
    addStickerButton->setEnabled(isEnabled);
    clearButton->setEnabled(isEnabled);
    brushSizeSlider->setEnabled(isEnabled);
}

//---------------------Sticker file collection methods ----------------------------------------//

StickerFileStore DrawingCanvas::getStickerFilesFromJSON()
{
    StickerFileStore fileStore = StickerFileStore();
    
    std::string fullFileText = FileUtils::getInstance()->getStringFromFile("res/chat/stickers/catalogue.json");
    
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
