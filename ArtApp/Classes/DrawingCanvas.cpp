//
//  DrawingCanvas.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 24/05/2017.
//
//

#include "DrawingCanvas.h"
#include <dirent.h>

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

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

void DrawingCanvas::setBaseImage(std::string fileName)
{
    Sprite* baseImage = Sprite::create(fileName);
    baseImage->setAnchorPoint(Vec2(0.5,0.5));
    baseImage->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize()/2);
    drawing->begin();
    baseImage->visit();
    drawing->end();
    Director::getInstance()->getRenderer()->render();
}

void DrawingCanvas::saveImage(std::string filePath)
{
    
    drawing->begin();
    for(int i = 0; i < drawingStack.size(); i++)
    {
        drawingStack[i]->visit();
    }
    drawing->end();
    drawing->saveToFile(filePath, Image::Format::PNG);
    Director::getInstance()->getRenderer()->render();
    
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
            clearButton->loadTextures("res/artapp/style/images/artIcons/art_button_undo.png", "res/artapp/style/images/artIcons/undo.png");
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
    clearButton->setAnchorPoint(Vec2(1,0));
    clearButton->setPosition(Vec2(visibleOrigin.x + visibleSize.width,visibleOrigin.y));
    clearButton->loadTextures("res/artapp/style/images/artIcons/bin.png", "res/artapp/style/images/artIcons/bin.png");
    clearButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onClearButtonPressed, this));
    this->addChild(clearButton,1);
    
    confirmDeleteImagePopup = Node::create();
    confirmDeleteImagePopup->setContentSize(Size(visibleSize.width/2, visibleSize.height/2));
    ui::Scale9Sprite* background = ui::Scale9Sprite::create("res/artapp/style/images/artIcons/gallery/art_painting_placeholder.png");
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
    confrimDeleteButton->loadTextures("res/artapp/style/images/artIcons/stickerConfirm.png", "res/artapp/style/images/artIcons/stickerConfirm.png");
    confrimDeleteButton->setAnchorPoint(Vec2(1,0));
    confrimDeleteButton->setPosition(background->getPosition() + Vec2(background->getContentSize().width/3, -background->getContentSize().height/3));
    confrimDeleteButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onConfirmDeletePressed, this));
    
    confirmDeleteImagePopup->addChild(confrimDeleteButton);
    
    cancelDeleteButton = ui::Button::create();
    cancelDeleteButton->loadTextures("res/artapp/style/images/artIcons/close.png", "res/artapp/style/images/artIcons/close.png");
    cancelDeleteButton->setAnchorPoint(Vec2(0,0));
    cancelDeleteButton->setPosition(background->getPosition() - Vec2(background->getContentSize().width/3, background->getContentSize().height/3));
    cancelDeleteButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCancelDeletePressed, this));
    
    confirmDeleteImagePopup->addChild(cancelDeleteButton);
    
    confirmDeleteImagePopup->setVisible(false);
    
    this->addChild(confirmDeleteImagePopup);
    
    
}

void DrawingCanvas::addColourSelectButtons(Size visibleSize, Point visibleOrigin)
{
    selected = Sprite::create("res/artapp/style/images/artIcons/checkMark.png");
    selected->setAnchorPoint(Vec2(0.5,0.5));
    selected->setNormalizedPosition(Vec2(0.5,0.5));
    
    colourButtonLayout = Node::create();
    colourButtonLayout->setContentSize(Size(visibleSize.width*0.8,visibleSize.height*0.4));
    colourButtonLayout->setAnchorPoint(Vec2(0.5,0.5));
    colourButtonLayout->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y + visibleSize.height/2));
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
            button->setNormalizedPosition(Vec2(((i-1)*(1/7.0f)),(j*0.4)));
            button->loadTextures("res/artapp/style/images/artIcons/colorSwatch.png", "res/artapp/style/images/artIcons/colorSwatch.png");
            button->setColor(colours[(j*7)+i-1]);
            if((j*7+i) == 1)
                button->addChild(selected);
            button->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onColourChangePressed,this));
            colourButtonLayout->addChild(button,1);
        }
    }
    
    colourSelectButton = ui::Button::create();
    colourSelectButton->setAnchorPoint(Vec2(0,0));
    colourSelectButton->setPosition(Vec2(0, visibleOrigin.y));
    colourSelectButton->loadTextures("res/artapp/style/images/artIcons/art_button_colour.png", "res/artapp/style/images/artIcons/art_button_colour.png");
    colourSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onColourSelectPressed,this));
    colourSelectButton->setColor(Color3B(selectedColour));
    
    this->addChild(colourSelectButton,1);
}

void DrawingCanvas::addToolSelectButtons(Size visibleSize, Point visibleOrigin)
{
    //toolSelectButton = ui::Button::create();
    //toolSelectButton->setAnchorPoint(Vec2(0,1));
    //toolSelectButton->setPosition(Vec2(0, colourSelectButton->getPosition().y - colourSelectButton->getContentSize().height));
    //toolSelectButton->loadTextures("res/artapp/style/images/artIcons/brushes/pencil.png", "res/artapp/style/images/artIcons/brushes/pencil.png");
    //toolSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolSelectPressed,this));
    
    //this->addChild(toolSelectButton,1);
    Sprite* toolBG = Sprite::create("res/artapp/style/images/artIcons/art_back_middle.png");
    toolBG->setAnchorPoint(Vec2(0.5,0));
    toolBG->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y - 50));
    
    this->addChild(toolBG,1);
    
    toolButtonLayout = Node::create();
    toolButtonLayout->setContentSize(toolBG->getContentSize());
    toolButtonLayout->setAnchorPoint(Vec2(0.5,0));
    toolButtonLayout->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y - 50));
    //toolButtonLayout->setVisible(true);
    
    this->addChild(toolButtonLayout,1);
    
    ui::Button* brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.1,0));
    brushButton->loadTextures("res/artapp/style/images/artIcons/brushes/pencil.png", "res/artapp/style/images/artIcons/brushes/pencil.png");
    brushButton->setColor(Color3B(selectedColour));
    brushButton->setScale(((toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height) * 1.15);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,0));
    SelectedToolButton = brushButton;
    toolButtonLayout->addChild(brushButton,1);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.3,0));
    brushButton->loadTextures("res/artapp/style/images/artIcons/brushes/brush.png", "res/artapp/style/images/artIcons/brushes/brush.png");
    brushButton->setColor(Color3B(COLOUR_DEFAULT));
    brushButton->setScale((toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,1));
    toolButtonLayout->addChild(brushButton,1);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.5,0));
    brushButton->loadTextures("res/artapp/style/images/artIcons/brushes/highlighter.png", "res/artapp/style/images/artIcons/brushes/highlighter.png");
    brushButton->setColor(Color3B(COLOUR_DEFAULT));
    brushButton->setScale((toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,2));
    toolButtonLayout->addChild(brushButton,1);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.7,0));
    brushButton->loadTextures("res/artapp/style/images/artIcons/brushes/spray.png", "res/artapp/style/images/artIcons/brushes/spray.png");
    brushButton->setColor(Color3B(COLOUR_DEFAULT));
    brushButton->setScale((toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,3));
    toolButtonLayout->addChild(brushButton,1);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.9,0));
    brushButton->loadTextures("res/artapp/style/images/artIcons/brushes/eraser.png", "res/artapp/style/images/artIcons/brushes/eraser.png");
    brushButton->setScale((toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->setName("eraser");
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onToolChanged,this,4));
    toolButtonLayout->addChild(brushButton,1);
}

void DrawingCanvas::addStickerSelectButtons(Size visibleSize, Point visibleOrigin)
{
    addStickerButton = ui::Button::create();
    addStickerButton->setAnchorPoint(Vec2(0,0));
    addStickerButton->setPosition(Vec2(colourSelectButton->getPosition().x + colourSelectButton->getContentSize().width, colourSelectButton->getPosition().y));
    addStickerButton->loadTextures("res/artapp/style/images/artIcons/art_button_sticker.png", "res/artapp/style/images/artIcons/art_button_sticker.png");
    addStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onAddStickerButtonPressed,this));
    
    this->addChild(addStickerButton,1);
    
    cancelStickerButton = ui::Button::create();
    cancelStickerButton->setAnchorPoint(Vec2(0,0));
    cancelStickerButton->setPosition(Vec2(visibleOrigin.x, visibleOrigin.y));
    cancelStickerButton->loadTextures("res/artapp/style/images/artIcons/stickerBin.png", "res/artapp/style/images/artIcons/stickerBin.png");
    cancelStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCancelStickerPressed,this));
    cancelStickerButton->setVisible(false);
    this->addChild(cancelStickerButton,11);
    
    confirmStickerButton = ui::Button::create();
    confirmStickerButton->setAnchorPoint(Vec2(1,0));
    confirmStickerButton->setPosition(Vec2(visibleOrigin.x + visibleSize.width, visibleOrigin.y));
    confirmStickerButton->loadTextures("res/artapp/style/images/artIcons/stickerConfirm.png", "res/artapp/style/images/artIcons/stickerConfirm.png");
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
    stickerScrollView->setBackGroundImage("res/artapp/style/images/artIcons/gallery/art_painting_placeholder.png");
    stickerScrollView->setVisible(false);
    this->addChild(stickerScrollView,12);
    
    //load stickers
    
    StickerCategoryLayout = Node::create();
    StickerCategoryLayout->setAnchorPoint(Vec2(0.5,0));
    StickerCategoryLayout->setContentSize(Size(visibleSize.width*2/3, visibleSize.height/5));
    StickerCategoryLayout->setPosition(Vec2(stickerScrollView->getPosition().x,stickerScrollView->getPosition().y + stickerScrollView->getContentSize().height/2));
    StickerCategoryLayout->setVisible(false);
    this->addChild(StickerCategoryLayout,13);
    
    stickerCats = getStickerFilesFromJSON();
    
    for(int i = 0; i < stickerCats.size(); i++)
    {
        ui::Button* stickerCatButton = ui::Button::create();
        stickerCatButton->loadTextures("res/chat/stickers/" + stickerCats[i].first, "res/chat/stickers/" + stickerCats[i].first);
        stickerCatButton->setAnchorPoint(Vec2(0,0.5));
        stickerCatButton->setNormalizedPosition(Vec2(i/(float)stickerCats.size(),0.5));
        stickerCatButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onStickerCategoryChangePressed, this,i));
        StickerCategoryLayout->addChild(stickerCatButton);
        if(i == 0)
            onStickerCategoryChangePressed(stickerCatButton, cocos2d::ui::Widget::TouchEventType::ENDED, i);
        
    }
    
    closeStickerSelectButton = ui::Button::create();
    closeStickerSelectButton->setAnchorPoint(Vec2(1,1));
    closeStickerSelectButton->setPosition(stickerScrollView->getPosition() + stickerScrollView->getContentSize()/2);
    closeStickerSelectButton->loadTextures("res/artapp/style/images/artIcons/close.png", "res/artapp/style/images/artIcons/close.png");
    closeStickerSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onCloseStickerSelectPressed, this));
    closeStickerSelectButton->setVisible(false);
    
    this->addChild(closeStickerSelectButton,12);
}

void DrawingCanvas::addBrushRadiusSlider(Size visibleSize, Point visibleOrigin)
{
    brushSizeSlider = ui::Slider::create();
    brushSizeSlider->setTouchEnabled(true);
    brushSizeSlider->loadBarTexture("res/artapp/style/images/artIcons/slideBack.png");
    brushSizeSlider->loadSlidBallTextures("res/artapp/style/images/artIcons/sliderIcon.png","res/artapp/style/images/artIcons/sliderIcon.png","");
    brushSizeSlider->setPercent(50);
    brushRadius = INITIAL_RADIUS + brushSizeSlider->getPercent()/2;
    brushSizeSlider->setAnchorPoint(Vec2(0.5,0.5));
    brushSizeSlider->setRotation(-90);
    brushSizeSlider->setPosition(Vec2(visibleOrigin.x + brushSizeSlider->getContentSize().height,visibleOrigin.y + visibleSize.height/2));
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
                clearButton->loadTextures("res/artapp/style/images/artIcons/art_button_bin.png", "res/artapp/style/images/artIcons/bin.png");
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
        //toolSelectButton->setVisible(true);
        addStickerButton->setVisible(true);
        SelectedToolButton->setColor(Color3B(selectedColour));
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
        //toolSelectButton->setVisible(false);
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
        //toolSelectButton->setVisible(false);
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
        stickerNode->setVisible(true);
        confirmStickerButton->setVisible(true);
        cancelStickerButton->setVisible(true);
        stickerNode->setTouchListenerEnabled(true);
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
        //toolSelectButton->setVisible(false);
        colourSelectButton->setVisible(false);
        addStickerButton->setVisible(false);
        brushSizeSlider->setVisible(false);
        stickerScrollView->setVisible(true);
        StickerCategoryLayout->setVisible(true);
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
        //toolSelectButton->setVisible(true);
        colourSelectButton->setVisible(true);
        addStickerButton->setVisible(true);
        brushSizeSlider->setVisible(true);
        stickerScrollView->setVisible(false);
        StickerCategoryLayout->setVisible(false);
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
        //toolSelectButton->setVisible(true);
        colourSelectButton->setVisible(true);
        addStickerButton->setVisible(true);
        brushSizeSlider->setVisible(true);
        confirmStickerButton->setVisible(false);
        cancelStickerButton->setVisible(false);
        drawCanvasTouchListener->setEnabled(true);
        stickerNode->setTouchListenerEnabled(false);
        
        //add sticker as node in drawing scene on undo stack
        
        if(drawingStack.size() == 0)
            clearButton->loadTextures("res/artapp/style/images/artIcons/art_button_undo.png", "res/artapp/style/images/artIcons/undo.png");
        drawingStack.push_back(stickerNode->getSticker());
        Sprite* temp = stickerNode->getSticker();
        temp->retain();
        Sprite* newSticker = Sprite::create("res/artapp/style/images/artIcons/art_button_sticker.png");
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
        //toolSelectButton->setVisible(true);
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
                SelectedToolButton->setColor(Color3B(COLOUR_DEFAULT));
            if(pressedButton->getName() != "eraser")
                pressedButton->setColor(Color3B(selectedColour));
            SelectedToolButton = pressedButton;
            
        }
        
        //toolButtonLayout->setVisible(false);
        //toolSelectButton->setVisible(true);
        colourSelectButton->setVisible(true);
        addStickerButton->setVisible(true);
        //toolSelectButton->loadTextures(pressedButton->getNormalFile().file, pressedButton->getPressedFile().file);
        
        
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
        brushRadius = INITIAL_RADIUS + slider->getPercent()/2;
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
        //pressedButton->setScale(baseScale / 1.15f);
        
        auto catButtons = StickerCategoryLayout->getChildren();
        
        for(int i = 0; i < catButtons.size(); i++)
        {
            catButtons.at(i)->setScale(1);
        }
        
        pressedButton->setScale(1.15);
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        
        //std::vector<std::string> fileNames = getStickerFileNamesInDir(FileUtils::getInstance()->fullPathForFilename("res/chat/stickers/" + stickerCats[index]));
        //int numStickers = (int)fileNames.size();
        
        int numStickers = (int)stickerCats[index].second.size();
        
        stickerScrollView->removeAllChildren();
        stickerScrollView->setInnerContainerSize(Size(visibleSize.width/7.0f * numStickers/2.0f, visibleSize.height/2));
        
        for(int i = 0; i < numStickers; i+=2)
        {
            ui::Button* temp = ui::Button::create();
            temp->setAnchorPoint(Vec2(0.5,0.5));
            temp->loadTextures("res/chat/stickers/" + stickerCats[index].second[i],"res/chat/stickers/" + stickerCats[index].second[i]);
            temp->setPosition(Vec2(stickerScrollView->getInnerContainerSize().width*((i+1.0f)/(numStickers+1)),visibleSize.height/3 + temp->getContentSize().height));
            temp->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::onAddStickerPressed, this));
            stickerScrollView->addChild(temp);
            
            if(i+1 < numStickers)
            {
                ui::Button* temp2 = ui::Button::create();
                temp2->setAnchorPoint(Vec2(0.5,0.5));
                temp2->loadTextures("res/chat/stickers/" + stickerCats[index].second[i+1],"res/chat/stickers/" + stickerCats[index].second[i+1]);
                temp2->setPosition(Vec2(stickerScrollView->getInnerContainerSize().width*((i+1.0f)/(numStickers+1)),visibleSize.height/3 - temp2->getContentSize().height));
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


std::vector<std::string> DrawingCanvas::getStickerDirs()
{
    std::string stickerDir = FileUtils::getInstance()->fullPathForFilename("res/chat/stickers/catalogue.json");
    stickerDir = stickerDir.substr(0,stickerDir.length() - 15);
    
    std::vector<std::string> dirNames;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (stickerDir.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if(ent->d_type == DT_DIR)
                dirNames.push_back(ent->d_name);
        }
        closedir (dir);
        dirNames.erase(dirNames.begin(), dirNames.begin() + 2);
        return dirNames;
    }
    else
    {
        CCLOG("dir doesnt exist");
        perror ("");
        return dirNames;
    }
    
}

std::vector<std::string> DrawingCanvas::getStickerFileNamesInDir(std::string stickerDir)
{
    std::vector<std::string> fileNames;
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (stickerDir.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if(ent->d_type == DT_REG && (std::string)ent->d_name != "category.png")
                fileNames.push_back(ent->d_name);
        }
        closedir (dir);
        return fileNames;
    }
    else
    {
        CCLOG("file dir doesnt exist");
        perror ("");
        return fileNames;
    }
    
}

NS_AZOOMEE_AA_END
