//
//  DrawingCanvas.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 24/05/2017.
//
//

#include "DrawingCanvas.h"

using namespace cocos2d;

bool DrawingCanvas::init()
{
    
    if(!Node::init())
        return false;
    
    background = LayerColor::create(Color4B(COLOUR_WHITE));
    
    this->addChild(background,-2);
    
    Size visibleSize = Director::getInstance()->getWinSize();
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    drawing = RenderTexture::create(visibleSize.width, visibleSize.height);
    drawing->setAnchorPoint(Vec2(0.5,0.5));
    drawing->setPosition(visibleSize/2);
    drawing->beginWithClear(0, 0, 0, 0);
    drawing->end();
    
    this->addChild(drawing, -1);
    
    //addBrushes();
    
    return true;
}

void DrawingCanvas::onEnter()
{
    Node::onEnter();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    this->setContentSize(visibleSize);
    
    addBrushes();
    
    
    for(int i = 0; i < brushes.size(); i++)
    {
        brushes[i]->setupDrawNode(visibleSize);
    }
    
    setupTouchHandling();
    setupMenus();
    
    stickerNode = StickerPlacementNode::create();
    this->addChild(stickerNode,10);
    stickerNode->setVisible(false);
    stickerNode->setTouchListenerEnabled(false);
}

void DrawingCanvas::onExit()
{
    Node::onExit();
}

void DrawingCanvas::setupTouchHandling()
{
    static bool touchProcessed = false;

    drawCanvasTouchListener = EventListenerTouchOneByOne::create();
    
    drawCanvasTouchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        if(touchProcessed)
            return false;
            
        activeBrush->onTouchBegin(touch, event);
        touchProcessed = true;

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
        
        touchProcessed = false;

        if(drawingStack.size() == 0)
            clearButton->loadTextures("res/artapp/style/images/articons/art_button_undo.png", "res/artapp/style/images/articons/undo.png");
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
        
    };
    drawCanvasTouchListener->onTouchCancelled = [&](Touch* touch, Event* event)
    {
        touchProcessed = false;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(drawCanvasTouchListener, this);
    
}

void DrawingCanvas::setupMenus()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();

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
    
    for (int i = 0; i < brushes.size(); ++i) {
        brushes[i]->init();
        brushes[i]->setSelectedColour(&selectedColour);
        brushes[i]->setBrushRadius(&brushRadius);
    }
    activeBrush = brushes[0];
    this->addChild(activeBrush->getDrawNode());
    
    selectedColour = COLOUR_1;
}

//UI LOADING

void DrawingCanvas::addClearButton(Size visibleSize, Point visibleOrigin)
{
    clearButton = ui::Button::create();
    clearButton->setAnchorPoint(Vec2(1,1));
    clearButton->setPosition(Vec2(visibleOrigin.x + visibleSize.width,visibleOrigin.y + visibleSize.height));
    clearButton->loadTextures("res/artapp/style/images/articons/art_button_bin.png", "res/artapp/style/images/articons/bin.png");
    clearButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onClearButtonPressed, this));
    this->addChild(clearButton,1);
    
    confirmDeleteImagePopup = Node::create();
    confirmDeleteImagePopup->setContentSize(Size(visibleSize.width/2, visibleSize.height/2));
    ui::Scale9Sprite* background = ui::Scale9Sprite::create("res/artapp/style/images/articons/gallery/art_painting_placeholder.png");
    background->setContentSize(Size(visibleSize.width/2, visibleSize.height/2));
    background->setAnchorPoint(Vec2(0.5,0.5));
    background->setPosition(visibleOrigin + visibleSize/2);
    confirmDeleteImagePopup->addChild(background);
    
    Label* text = Label::createWithTTF("Delete Drawing?", "fonts/azoomee.ttf", 128);
    text->setAnchorPoint(Vec2(0.5,0.5));
    text->setPosition(visibleOrigin + visibleSize/2 + Vec2(0,200));
    text->setColor(Color3B(0,0,0));
    
    confirmDeleteImagePopup->addChild(text);
    
    confrimDeleteButton = ui::Button::create();
    confrimDeleteButton->loadTextures("res/artapp/style/images/articons/stickerConfirm.png", "res/artapp/style/images/articons/stickerConfirm.png");
    confrimDeleteButton->setAnchorPoint(Vec2(1,0));
    confrimDeleteButton->setPosition(background->getPosition() + Vec2(background->getContentSize().width/3, -background->getContentSize().height/3));
    confrimDeleteButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onConfirmDeletePressed, this));
    
    confirmDeleteImagePopup->addChild(confrimDeleteButton);
    
    cancelDeleteButton = ui::Button::create();
    cancelDeleteButton->loadTextures("res/artapp/style/images/articons/close.png", "res/artapp/style/images/articons/close.png");
    cancelDeleteButton->setAnchorPoint(Vec2(0,0));
    cancelDeleteButton->setPosition(background->getPosition() - Vec2(background->getContentSize().width/3, background->getContentSize().height/3));
    cancelDeleteButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCancelDeletePressed, this));
    
    confirmDeleteImagePopup->addChild(cancelDeleteButton);
    
    confirmDeleteImagePopup->setVisible(false);
    
    this->addChild(confirmDeleteImagePopup);
    
    
}

void DrawingCanvas::addColourSelectButtons(Size visibleSize, Point visibleOrigin)
{
    selected = Sprite::create("res/artapp/style/images/articons/checkMark.png");
    selected->setAnchorPoint(Vec2(0.5,0.5));
    selected->setNormalizedPosition(Vec2(0.5,0.5));
    
    colourButtonLayout = Node::create();
    colourButtonLayout->setContentSize(Size(visibleSize.width*0.2,visibleSize.height));
    colourButtonLayout->setAnchorPoint(Vec2(0,0.5));
    colourButtonLayout->setPosition(Vec2(0,visibleOrigin.y + visibleSize.height/2));
    colourButtonLayout->setVisible(false);
    this->addChild(colourButtonLayout,1);
    
    std::vector<Color3B> colours;
    colours.push_back(Color3B(COLOUR_1));
    colours.push_back(Color3B(COLOUR_2));
    colours.push_back(Color3B(COLOUR_3));
    colours.push_back(Color3B(COLOUR_4));
    colours.push_back(Color3B(COLOUR_5));
    colours.push_back(Color3B(COLOUR_6));
    colours.push_back(Color3B(COLOUR_7));
    colours.push_back(Color3B(COLOUR_8));
    colours.push_back(Color3B(COLOUR_9));
    colours.push_back(Color3B(COLOUR_10));
    colours.push_back(Color3B(COLOUR_11));
    colours.push_back(Color3B(COLOUR_12));
    colours.push_back(Color3B(COLOUR_13));
    colours.push_back(Color3B(COLOUR_14));
    for (int j = 0; j<2; ++j)
    {
        for (int i = 1; i<=7; ++i)
        {
            ui::Button* button = ui::Button::create();
            button->setAnchorPoint(Vec2(0,0.5));
            button->setNormalizedPosition(Vec2((j*0.4),(i*(1/7.0f))-(1/14.0f)));
            button->loadTextures("res/artapp/style/images/articons/colorSwatch.png", "res/artapp/style/images/articons/colorSwatch.png");
            button->setColor(colours[(j*7)+i-1]);
            if((j*7+i) == 1)
                button->addChild(selected);
            button->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onColourChangePressed,this));
            colourButtonLayout->addChild(button,1);
        }
    }
    
    colourSelectButton = ui::Button::create();
    colourSelectButton->setAnchorPoint(Vec2(0,1));
    colourSelectButton->setPosition(Vec2(0, visibleOrigin.y + visibleSize.height));
    colourSelectButton->loadTextures("res/artapp/style/images/articons/colorSwatch.png", "res/artapp/style/images/articons/colorSwatch.png");
    colourSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onColourSelectPressed,this));
    colourSelectButton->setColor(Color3B(selectedColour));
    
    this->addChild(colourSelectButton,1);
}

void DrawingCanvas::addToolSelectButtons(Size visibleSize, Point visibleOrigin)
{
    toolSelectButton = ui::Button::create();
    toolSelectButton->setAnchorPoint(Vec2(0,0.5));
    toolSelectButton->setPosition(Vec2(0, visibleOrigin.y + visibleSize.height - (1.5*colourSelectButton->getContentSize().height)));
    toolSelectButton->loadTextures("res/artapp/style/images/articons/004-pen.png", "res/artapp/style/images/articons/004-pen.png");
    toolSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolSelectPressed,this));
    
    this->addChild(toolSelectButton,1);
    
    toolButtonLayout = Node::create();
    toolButtonLayout->setContentSize(Size(visibleSize.width*0.1,visibleSize.height));
    toolButtonLayout->setAnchorPoint(Vec2(0,0.5));
    toolButtonLayout->setPosition(Vec2(0,visibleOrigin.y + visibleSize.height/2));
    toolButtonLayout->setVisible(false);
    this->addChild(toolButtonLayout,1);
    
    ui::Button* brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0,0.5));
    brushButton->setNormalizedPosition(Vec2(0,0.9));
    brushButton->loadTextures("res/artapp/style/images/articons/004-pen.png", "res/artapp/style/images/articons/004-pen.png");
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,0));
    toolButtonLayout->addChild(brushButton,1);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0,0.5));
    brushButton->setNormalizedPosition(Vec2(0,0.7));
    brushButton->loadTextures("res/artapp/style/images/articons/002-paintbrush.png", "res/artapp/style/images/articons/002-paintbrush.png");
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,1));
    toolButtonLayout->addChild(brushButton,1);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0,0.5));
    brushButton->setNormalizedPosition(Vec2(0,0.5));
    brushButton->loadTextures("res/artapp/style/images/articons/003-highlighter.png", "res/artapp/style/images/articons/003-highlighter.png");
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,2));
    toolButtonLayout->addChild(brushButton,1);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0,0.5));
    brushButton->setNormalizedPosition(Vec2(0,0.3));
    brushButton->loadTextures("res/artapp/style/images/articons/005-spray-paint.png", "res/artapp/style/images/articons/005-spray-paint.png");
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,3));
    toolButtonLayout->addChild(brushButton,1);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0,0.5));
    brushButton->setNormalizedPosition(Vec2(0,0.1));
    brushButton->loadTextures("res/artapp/style/images/articons/eraser.png", "res/artapp/style/images/articons/eraser.png");
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,4));
    toolButtonLayout->addChild(brushButton,1);
}

void DrawingCanvas::addStickerSelectButtons(Size visibleSize, Point visibleOrigin)
{
    addStickerButton = ui::Button::create();
    addStickerButton->setAnchorPoint(Vec2(0,0.5));
    addStickerButton->setPosition(Vec2(0, visibleOrigin.y + visibleSize.height - (2.5*colourSelectButton->getContentSize().height)));
    addStickerButton->loadTextures("res/artapp/style/images/articons/art_button_sticker.png", "res/artapp/style/images/articons/art_button_sticker.png");
    addStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onAddStickerButtonPressed,this));
    
    this->addChild(addStickerButton,1);
    
    cancelStickerButton = ui::Button::create();
    cancelStickerButton->setAnchorPoint(Vec2(0,0));
    cancelStickerButton->setPosition(Vec2(visibleOrigin.x, visibleOrigin.y));
    cancelStickerButton->loadTextures("res/artapp/style/images/articons/stickerBin.png", "res/artapp/style/images/articons/stickerBin.png");
    cancelStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCancelStickerPressed,this));
    cancelStickerButton->setVisible(false);
    this->addChild(cancelStickerButton,11);
    
    confirmStickerButton = ui::Button::create();
    confirmStickerButton->setAnchorPoint(Vec2(1,0));
    confirmStickerButton->setPosition(Vec2(visibleOrigin.x + visibleSize.width, visibleOrigin.y));
    confirmStickerButton->loadTextures("res/artapp/style/images/articons/stickerConfirm.png", "res/artapp/style/images/articons/stickerConfirm.png");
    confirmStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onConfirmStickerPressed,this));
    confirmStickerButton->setVisible(false);
    this->addChild(confirmStickerButton,11);
    
    stickerScrollView = cocos2d::ui::ScrollView::create();
    stickerScrollView->setContentSize(Size(visibleSize.width*2/3, visibleSize.height*2/3));
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
    stickerScrollView->setBackGroundImage("res/artapp/style/images/articons/gallery/art_painting_placeholder.png");
    
    this->addChild(stickerScrollView,12);
    
    //load stickers
    
    for(int i = 0; i < 15; ++i)
    {
        for(int j = -1; j <= 1; j+=2)
        {
            ui::Button* temp = ui::Button::create();
            temp->setAnchorPoint(Vec2(0.5,0.5));
            temp->loadTextures("res/artapp/style/images/articons/art_button_sticker.png","res/artapp/style/images/articons/art_button_sticker.png");
            temp->setPosition(Vec2(visibleSize.width*((i+0.5)/7.5f),visibleSize.height/3 + j*temp->getContentSize().height));
            temp->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onAddStickerPressed, this));
            stickerScrollView->addChild(temp);
        }
    }
    
    stickerScrollView->setVisible(false);
    
    closeStickerSelectButton = ui::Button::create();
    closeStickerSelectButton->setAnchorPoint(Vec2(1,1));
    closeStickerSelectButton->setPosition(stickerScrollView->getPosition() + stickerScrollView->getContentSize()/2);
    closeStickerSelectButton->loadTextures("res/artapp/style/images/articons/close.png", "res/artapp/style/images/articons/close.png");
    closeStickerSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCloseStickerSelectPressed, this));
    closeStickerSelectButton->setVisible(false);
    
    this->addChild(closeStickerSelectButton,12);
}

void DrawingCanvas::addBrushRadiusSlider(Size visibleSize, Point visibleOrigin)
{
    brushSizeSlider = ui::Slider::create();
    brushSizeSlider->setTouchEnabled(true);
    brushSizeSlider->loadBarTexture("res/artapp/style/images/articons/slideBack.png");
    brushSizeSlider->loadSlidBallTextures("res/artapp/style/images/articons/sliderIcon.png","res/artapp/style/images/articons/sliderIcon.png","");
    brushSizeSlider->setPercent(50);
    brushRadius = INITIAL_RADIUS + brushSizeSlider->getPercent()/2;
    brushSizeSlider->setAnchorPoint(Vec2(0.5,0.5));
    brushSizeSlider->setRotation(-90);
    brushSizeSlider->setPosition(Vec2(visibleOrigin.x + visibleSize.width - brushSizeSlider->getContentSize().height,visibleOrigin.y + visibleSize.height/2));
    brushSizeSlider->addEventListener(CC_CALLBACK_2(DrawingCanvas::onRadiusSliderInteract, this));
    
    this->addChild(brushSizeSlider,1);
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
        }
        else
        {
            this->removeChild(drawingStack.back());
            drawingStack.pop_back();
            if(drawingStack.size() == 0)
                clearButton->loadTextures("res/artapp/style/images/articons/art_button_bin.png", "res/artapp/style/images/articons/bin.png");
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
        colourSelectButton->setVisible(true);
        toolSelectButton->setVisible(true);
        addStickerButton->setVisible(true);
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
        colourSelectButton->setVisible(false);
        toolSelectButton->setVisible(false);
        addStickerButton->setVisible(false);
        colourButtonLayout->setVisible(true);
        
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
        toolSelectButton->setVisible(false);
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
        closeStickerSelectButton->setVisible(false);
        stickerNode->setVisible(true);
        confirmStickerButton->setVisible(true);
        cancelStickerButton->setVisible(true);
        stickerNode->setTouchListenerEnabled(true);
        CCLOG("%s",pressedButton->getNormalFile().file.c_str());
        Sprite* newSticker = Sprite::create(pressedButton->getNormalFile().file);
        newSticker->setAnchorPoint(Vec2(0.5,0.5));
        newSticker->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize()/2);
        stickerNode->setSticker(newSticker);
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
        toolSelectButton->setVisible(false);
        colourSelectButton->setVisible(false);
        addStickerButton->setVisible(false);
        brushSizeSlider->setVisible(false);
        stickerScrollView->setVisible(true);
        closeStickerSelectButton->setVisible(true);
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
        toolSelectButton->setVisible(true);
        colourSelectButton->setVisible(true);
        addStickerButton->setVisible(true);
        brushSizeSlider->setVisible(true);
        stickerScrollView->setVisible(false);
        closeStickerSelectButton->setVisible(false);
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
        toolSelectButton->setVisible(true);
        colourSelectButton->setVisible(true);
        addStickerButton->setVisible(true);
        brushSizeSlider->setVisible(true);
        confirmStickerButton->setVisible(false);
        cancelStickerButton->setVisible(false);
        drawCanvasTouchListener->setEnabled(true);
        stickerNode->setTouchListenerEnabled(false);
        
        //add sticker as node in drawing scene on undo stack
        
        if(drawingStack.size() == 0)
            clearButton->loadTextures("res/artapp/style/images/articons/art_button_undo.png", "res/artapp/style/images/articons/undo.png");
        drawingStack.push_back(stickerNode->getSticker());
        Sprite* temp = stickerNode->getSticker();
        temp->retain();
        Sprite* newSticker = Sprite::create("res/artapp/style/images/articons/art_button_sticker.png");
        newSticker->setAnchorPoint(Vec2(0.5,0.5));
        newSticker->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize()/2);
        stickerNode->setSticker(newSticker);
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
        toolSelectButton->setVisible(true);
        colourSelectButton->setVisible(true);
        addStickerButton->setVisible(true);
        brushSizeSlider->setVisible(true);
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
        drawing->beginWithClear(COLOUR_WHITE.r, COLOUR_WHITE.g, COLOUR_WHITE.b, 1);
        drawing->end();
        Director::getInstance()->getRenderer()->render();
        drawCanvasTouchListener->setEnabled(true);
        
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
        pressedButton->setScale(baseScale * 0.85f);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        activeBrush->getDrawNode()->removeFromParent();
        activeBrush = brushes[index];
        pressedButton->setScale(baseScale / 0.85f);
        toolButtonLayout->setVisible(false);
        toolSelectButton->setVisible(true);
        colourSelectButton->setVisible(true);
        addStickerButton->setVisible(true);
        toolSelectButton->loadTextures(pressedButton->getNormalFile().file, pressedButton->getPressedFile().file);
        this->addChild(activeBrush->addDrawNode(Director::getInstance()->getVisibleSize()));
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvas::onRadiusSliderInteract(Ref *pSender, ui::Slider::EventType eEventType)
{
    ui::Slider* slider = static_cast<ui::Slider*>(pSender);
    
    if(eEventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        brushRadius = INITIAL_RADIUS + slider->getPercent()/2;
    }
}
