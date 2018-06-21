//
//  DrawingCanvasUILayer.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 16/10/2017.
//
//

#include "DrawingCanvasUILayer.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

const std::vector<Color3B> DrawingCanvasUILayer::_kColours = { // top left
    Color3B(Style::Color_4F::grey), //row 1
    Color3B(Style::Color_4F::darkGrey),
    Color3B(Style::Color_4F::black),
    Color3B(Style::Color_4F::darkPurple), //row 2
    Color3B(Style::Color_4F::purple),
    Color3B(Style::Color_4F::neonPink),
    Color3B(Style::Color_4F::babyPink),
    Color3B(Style::Color_4F::palePink),
    Color3B(Style::Color_4F::pink),
    Color3B(Style::Color_4F::darkPink),
    Color3B(Style::Color_4F::red),
    Color3B(Style::Color_4F::darkBlue), // row 3
    Color3B(Style::Color_4F::blue),
    Color3B(Style::Color_4F::lightBlue),
    Color3B(Style::Color_4F::neonBlue),
    Color3B(Style::Color_4F::greenBlue),
    Color3B(Style::Color_4F::green),
    Color3B(Style::Color_4F::grassGreen),
    Color3B(Style::Color_4F::neonGreen),
    Color3B(Style::Color_4F::darkBrown), //row 4
    Color3B(Style::Color_4F::brown),
    Color3B(Style::Color_4F::lightBrown),
    Color3B(Style::Color_4F::orangeBrown),
    Color3B(Style::Color_4F::orange),
    Color3B(Style::Color_4F::darkYellow),
    Color3B(Style::Color_4F::yellow),
    Color3B(Style::Color_4F::paleYellow) // bottom right
};

const std::vector<std::pair<std::string, std::string>> DrawingCanvasUILayer::_kPatterns = {
    std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/glitter_gold_btn.png","gold"),
    std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/glitter_silver_btn.png","silver"),
    std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/glitter_pink_btn.png","pink"),
    std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/glitter_blue_btn.png","blue"),
    std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/glitter_green_btn.png","green")
};

bool DrawingCanvasUILayer::init()
{
    if(!Super::init())
    {
        return false;
    }
    return true;
}

void DrawingCanvasUILayer::onEnter()
{
    Super::onEnter();
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    const Point& visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    _overlay = LayerColor::create(Style::Color_4B::semiTransparentOverlay, visibleSize.width, visibleSize.height);
    _overlay->setPosition(visibleOrigin);
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
    
    addBackgroundFrame(visibleSize, visibleOrigin);
    addClearButton(visibleSize, visibleOrigin);
    addColourSelectButtons(visibleSize, visibleOrigin);
    addToolSelectButtons(visibleSize, visibleOrigin);
    addBrushRadiusSlider(visibleSize, visibleOrigin);
    addStickerSelectButtons(visibleSize, visibleOrigin);
    
}

void DrawingCanvasUILayer::onExit()
{
    Super::onExit();
}

void DrawingCanvasUILayer::setDrawingCanvas(DrawingCanvas *drawingCanvas)
{
    this->_drawingCanvas = drawingCanvas;
}

//UI LOADING

void DrawingCanvasUILayer::addBackgroundFrame(const Size& visibleSize, const Point& visibleOrigin)
{
    ClippingNode* bgClipNode = ClippingNode::create();
    Sprite* stencil = Sprite::create(kArtAppAssetLoc + "white_bg.png");
    Vec2 drawWindowSize = Vec2(visibleSize.width * 0.89, visibleSize.height * 0.8);
    stencil->setScale(drawWindowSize.x/stencil->getContentSize().width, drawWindowSize.y/stencil->getContentSize().height);
    stencil->setPosition(visibleOrigin + Vec2(visibleSize.width * 0.09,visibleSize.height * 0.175));
    stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bgClipNode->setStencil(stencil);
    LayerColor* bgColour = LayerColor::create(Color4B(Style::Color_4F::black));
    bgColour->setContentSize(Director::getInstance()->getWinSize());
    bgClipNode->addChild(bgColour);
    bgClipNode->setInverted(true);
    this->addChild(bgClipNode);
}

void DrawingCanvasUILayer::addClearButton(const Size& visibleSize, const Point& visibleOrigin)
{
    _clearButton = ui::Button::create();
    _clearButton->setAnchorPoint(Vec2(0.5,0.5));
    _clearButton->loadTextures(kArtAppAssetLoc + "delete.png", kArtAppAssetLoc + "delete.png");
    _clearButton->setPosition(Vec2(visibleSize.width - _clearButton->getContentSize().width * 2.5, visibleOrigin.y + visibleSize.height * 0.0825f));
    _clearButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onClearButtonPressed, this));
    this->addChild(_clearButton,MAIN_UI_LAYER);
    
    _undoButton = ui::Button::create();
    _undoButton->setAnchorPoint(Vec2(0.5,0.5));
    _undoButton->loadTextures(kArtAppAssetLoc + "redo.png", kArtAppAssetLoc + "redo.png");
    _undoButton->setPosition(Vec2(visibleSize.width - _undoButton->getContentSize().width, _clearButton->getPosition().y));
    _undoButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onUndoButtonPressed, this));
    this->addChild(_undoButton,MAIN_UI_LAYER);
    
    _confirmDeleteImagePopup = Node::create();
    _confirmDeleteImagePopup->setContentSize(Size(visibleSize.width*0.6, visibleSize.height*0.53));
    _confirmDeleteImagePopup->setPosition(visibleOrigin + Vec2(visibleSize.width * 0.5,visibleSize.height * 0.5625));
    _confirmDeleteImagePopup->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    ui::Scale9Sprite* backgroundFrame = ui::Scale9Sprite::create(kArtAppAssetLoc + "gallery/art_painting_placeholder.png");
    backgroundFrame->setContentSize(_confirmDeleteImagePopup->getContentSize());
    backgroundFrame->setAnchorPoint(Vec2(0.5,0.5));
    backgroundFrame->setNormalizedPosition(Vec2(0.5,0.5));
    backgroundFrame->setColor(Color3B(Style::Color_4F::brightAqua));
    _confirmDeleteImagePopup->addChild(backgroundFrame);
    
    ui::Scale9Sprite* background = ui::Scale9Sprite::create(kArtAppAssetLoc + "gallery/art_painting_placeholder.png");
    background->setContentSize(_confirmDeleteImagePopup->getContentSize() - Size(24,24));
    background->setAnchorPoint(Vec2(0.5,0.5));
    background->setNormalizedPosition(Vec2(0.5,0.5));
    background->setColor(Color3B(Style::Color_4F::black));
    _confirmDeleteImagePopup->addChild(background);
    
    Sprite* oomee = Sprite::create(kArtAppAssetLoc + "you_sure_4_x_8.png");
    oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    oomee->setNormalizedPosition(Vec2(0.23,0.5));
    _confirmDeleteImagePopup->addChild(oomee);
    
    Label* text = Label::createWithTTF("Are you sure you\nwant to delete\nyour picture?", Style::Font::Regular, 100);
    text->setAnchorPoint(Vec2(0,1));
    text->setNormalizedPosition(Vec2(0.45,0.88));
    text->setColor(Color3B(Style::Color_4F::brightAqua));
    text->setLineSpacing(20);
    
    _confirmDeleteImagePopup->addChild(text);
    
    _confrimDeleteButton = ui::Button::create();
    _confrimDeleteButton->loadTextures(kArtAppAssetLoc + "yes.png", kArtAppAssetLoc + "yes.png");
    _confrimDeleteButton->setAnchorPoint(Vec2(0.5,0.5));
    _confrimDeleteButton->setNormalizedPosition(Vec2(0.779,0.275));
    _confrimDeleteButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onConfirmDeletePressed, this));
    
    _confirmDeleteImagePopup->addChild(_confrimDeleteButton);
    
    _cancelDeleteButton = ui::Button::create();
    _cancelDeleteButton->loadTextures(kArtAppAssetLoc + "no.png", kArtAppAssetLoc + "no.png");
    _cancelDeleteButton->setAnchorPoint(Vec2(0.5,0.5));
    _cancelDeleteButton->setNormalizedPosition(Vec2(0.526,0.275));
    _cancelDeleteButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onCancelDeletePressed, this));
    
    _confirmDeleteImagePopup->addChild(_cancelDeleteButton);
    
    _confirmDeleteImagePopup->setVisible(false);
    
    this->addChild(_confirmDeleteImagePopup,POPUP_UI_LAYER);
    
    
}

void DrawingCanvasUILayer::addColourSelectButtons(const Size& visibleSize, const Point& visibleOrigin)
{
    _selected = Sprite::create(kArtAppAssetLoc + "checkMark.png");
    _selected->setAnchorPoint(Vec2(0.5,0.5));
    _selected->setNormalizedPosition(Vec2(0.5,0.5));
    
    const Vec2& tableDimensions =  Vec2(8,ceilf((_kColours.size()+ _kPatterns.size())/8.0f));
    const Size& windowSize = Director::getInstance()->getWinSize();
    
    _colourButtonLayout = Node::create();
    _colourButtonLayout->setContentSize(Size(windowSize.width*0.75,windowSize.height*(0.13*tableDimensions.y)));
    _colourButtonLayout->setAnchorPoint(Vec2(0.5,0.5));
    _colourButtonLayout->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y + visibleSize.height/2 + 100));
    _colourButtonLayout->setVisible(false);
    this->addChild(_colourButtonLayout,POPUP_UI_LAYER);
    
    ui::Scale9Sprite* colourBG = ui::Scale9Sprite::create(kArtAppAssetLoc + "popup_bg.png");
    colourBG->setContentSize(_colourButtonLayout->getContentSize() + Size(50,50));
    colourBG->setAnchorPoint(Vec2(0.5,0.5));
    colourBG->setNormalizedPosition(Vec2(0.5,0.5));
    _colourButtonLayout->addChild(colourBG);
    
    int colourCount = 0;
    int patternCount = 0;
    
    for (int j = tableDimensions.y; j > 0; j--)
    {
        for (int i = 0; i<tableDimensions.x; i++)
        {
            if(patternCount >= _kPatterns.size())
            {
                ui::Button* button = ui::Button::create();
                button->setAnchorPoint(Vec2(0.5,0.5));
                float xPos = (i+0.5) / tableDimensions.x;
                float yPos = (j-0.5) / tableDimensions.y;
                button->setNormalizedPosition(Vec2(xPos,yPos));
                button->loadTextures(kArtAppAssetLoc + "art_app_pallete_circle.png", kArtAppAssetLoc + "art_app_pallete_circle.png");
                button->setColor(_kColours[colourCount]);
                button->setName("");
                if(colourCount == 14)
                {
                    _drawingCanvas->setSelectedColour(Color4F(_kColours[colourCount]));
                    _drawingCanvas->setSelectedPattern("");
                    button->addChild(_selected);
                }
                button->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onColourChangePressed,this));
                
                Sprite* frame = Sprite::create(kArtAppAssetLoc + "button_frame.png");
                frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                frame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
                button->addChild(frame);
                _colourButtonLayout->addChild(button);
            
                colourCount++;
            }
            else
            {
                ui::Button* button = ui::Button::create();
                button->setAnchorPoint(Vec2(0.5,0.5));
                float xPos = (i+0.5) / tableDimensions.x;
                float yPos = (j-0.5) / tableDimensions.y;
                button->setNormalizedPosition(Vec2(xPos,yPos));
                button->loadTextures(_kPatterns[patternCount].first, _kPatterns[patternCount].first);
                button->setName(_kPatterns[patternCount].second);
                button->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onColourChangePressed,this));
                _colourButtonLayout->addChild(button);
                
                patternCount++;
            }
        }
    }
    
    ui::Button* closeButton = ui::Button::create();
    closeButton->setAnchorPoint(Vec2(0.9,0.9));
    closeButton->setPosition(colourBG->getContentSize());
    closeButton->loadTextures(kArtAppAssetLoc + "close_button.png", kArtAppAssetLoc + "close_button.png");
    closeButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onCloseColourSelectPressed, this));
    _colourButtonLayout->addChild(closeButton);
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        _colourButtonLayout->setScale(0.85);
    }
    
    _colourSelectButton = ui::Button::create();
    _colourSelectButton->setAnchorPoint(Vec2(0.5,0.5));
    _colourSelectButton->loadTextures(kArtAppAssetLoc + "colour_frame.png", kArtAppAssetLoc + "colour_frame.png");
    _colourSelectButton->setPosition(visibleOrigin + Vec2(_colourSelectButton->getContentSize().width*2.5,visibleSize.height * 0.0825f));
    _colourSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onColourSelectPressed,this));
    ClippingNode* buttonBody = ClippingNode::create();
    Sprite* stencil =Sprite::create(kArtAppAssetLoc + "colour.png");
    stencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    stencil->setPosition(_colourSelectButton->getContentSize()/2);
    buttonBody->setStencil(stencil);
    buttonBody->setName("body");
    buttonBody->setAlphaThreshold(0.5f);
    Sprite* clippedImage = Sprite::create(kArtAppAssetLoc + "white_bg.png");
    clippedImage->setColor(Color3B(_drawingCanvas->getSelectedColour()));
    clippedImage->setPosition(_colourSelectButton->getContentSize()/2);
    clippedImage->setScale(clippedImage->getContentSize().width/_colourSelectButton->getContentSize().width);
    buttonBody->addChild(clippedImage);
    _colourSelectButton->addChild(buttonBody,-1);
    
    
    this->addChild(_colourSelectButton,MAIN_UI_LAYER);
}

void DrawingCanvasUILayer::addToolSelectButtons(const Size& visibleSize, const Point& visibleOrigin)
{
    
    _toolButtonLayout = Node::create();
    _toolButtonLayout->setContentSize(Size(visibleSize.width*0.6f,visibleSize.height*0.175f));
    _toolButtonLayout->setAnchorPoint(Vec2(0.5,0));
    _toolButtonLayout->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y));
    this->addChild(_toolButtonLayout,MAIN_UI_LAYER);
    
    addBrushTool(kArtAppAssetLoc + "pencil_frame.png", kArtAppAssetLoc + "pencil.png", PEN, Vec2(0.1,0), true);
    addBrushTool(kArtAppAssetLoc + "brush_frame.png", kArtAppAssetLoc + "brush.png", PAINTBRUSH, Vec2(0.3,0), false);
    addBrushTool(kArtAppAssetLoc + "felt_tip_frame.png", kArtAppAssetLoc + "felt_tip.png", HIGHLIGHTER, Vec2(0.5,0), false);
    addBrushTool(kArtAppAssetLoc + "can_frame.png", kArtAppAssetLoc + "can.png", SPRAYCAN, Vec2(0.7,0), false);
    //eraser is built differently
    ui::Button* brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.9,0));
    brushButton->loadTextures(kArtAppAssetLoc + "eraser.png", kArtAppAssetLoc + "eraser.png");
    brushButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->setName("eraser");
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onToolChanged,this,ERASER));
    _toolButtonLayout->addChild(brushButton);
}

void DrawingCanvasUILayer::addStickerSelectButtons(const Size& visibleSize, const Point& visibleOrigin)
{
    _addStickerButton = ui::Button::create();
    _addStickerButton->setAnchorPoint(Vec2(0.5,0.5));
    _addStickerButton->loadTextures(kArtAppAssetLoc + "star_3.png", kArtAppAssetLoc + "star_3.png");
    _addStickerButton->setPosition(Vec2(_addStickerButton->getContentSize().width, _colourSelectButton->getPosition().y));
    _addStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onAddStickerButtonPressed,this));
    
    this->addChild(_addStickerButton,MAIN_UI_LAYER);
    
    _cancelStickerButton = ui::Button::create();
    _cancelStickerButton->setAnchorPoint(Vec2(1,0));
    _cancelStickerButton->setPosition(Vec2(visibleOrigin.x + visibleSize.width * 0.846, visibleOrigin.y + visibleSize.height * 0.03));
    _cancelStickerButton->loadTextures(kArtAppAssetLoc + "delete.png", kArtAppAssetLoc + "delete.png");
    _cancelStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onCancelStickerPressed,this));
    _cancelStickerButton->setVisible(false);
    this->addChild(_cancelStickerButton,STICKER_UI_LAYER);
    
    _confirmStickerButton = ui::Button::create();
    _confirmStickerButton->setAnchorPoint(Vec2(1,0));
    _confirmStickerButton->setPosition(Vec2(visibleOrigin.x + visibleSize.width*0.97, visibleOrigin.y + visibleSize.height * 0.02));
    _confirmStickerButton->loadTextures(kArtAppAssetLoc + "confirm.png", kArtAppAssetLoc + "confirm.png");
    _confirmStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onConfirmStickerPressed,this));
    _confirmStickerButton->setVisible(false);
    this->addChild(_confirmStickerButton,STICKER_UI_LAYER);
    
    _stickerPopupBG = ui::Scale9Sprite::create(kArtAppAssetLoc + "popup_bg.png");
    _stickerPopupBG->setContentSize(Size(visibleSize.width * 0.74, visibleSize.height * 0.68));
    _stickerPopupBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _stickerPopupBG->setPosition(visibleOrigin + Vec2(visibleSize.width * 0.54, visibleSize.height * 0.56));
    _stickerPopupBG->setVisible(false);
    this->addChild(_stickerPopupBG,POPUP_UI_LAYER);
    
    _stickerScrollView = cocos2d::ui::ScrollView::create();
    _stickerScrollView->setContentSize(Size(_stickerPopupBG->getContentSize().width - 35, visibleSize.height/2));
    _stickerScrollView->setInnerContainerSize(Size(visibleSize.width*2, visibleSize.height/2));
    _stickerScrollView->setAnchorPoint(Vec2(0.5,0.5));
    _stickerScrollView->setPosition(_stickerPopupBG->getPosition() - Vec2(0,_stickerPopupBG->getContentSize().height/6));
    _stickerScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    _stickerScrollView->setBounceEnabled(true);
    _stickerScrollView->setTouchEnabled(true);
    _stickerScrollView->setSwallowTouches(true);
    _stickerScrollView->setScrollBarEnabled(false);
    _stickerScrollView->setVisible(false);
    this->addChild(_stickerScrollView,POPUP_UI_LAYER);
    
    _stickerCatBG = ui::Scale9Sprite::create(kArtAppAssetLoc + "popup_menu_bg.png");
    _stickerCatBG->setContentSize(Size(_stickerPopupBG->getContentSize().width - 35, visibleSize.height*0.165));
    _stickerCatBG->setAnchorPoint(Vec2(0.5,1));
    _stickerCatBG->setPosition(Vec2(_stickerPopupBG->getPosition().x,_stickerPopupBG->getPosition().y + _stickerPopupBG->getContentSize().height/2 - 17.5));
    _stickerCatBG->setVisible(false);
    _stickerCatBG->setColor(Color3B::BLACK);
    
    this->addChild(_stickerCatBG,POPUP_UI_LAYER);
    
    
    
    //load stickers
    
    _stickerCategoryLayout = Node::create();
    _stickerCategoryLayout->setAnchorPoint(Vec2(0.5,0));
    _stickerCategoryLayout->setContentSize(Size(_stickerCatBG->getContentSize().width, _stickerCatBG->getContentSize().height));
    _stickerCategoryLayout->setPosition(Vec2(_stickerCatBG->getPosition().x,_stickerCatBG->getPosition().y - _stickerCatBG->getContentSize().height));
    _stickerCategoryLayout->setVisible(false);
    
    
    this->addChild(_stickerCategoryLayout,POPUP_UI_LAYER);
    
    getStickerFilesFromJSON();
    
    _selectionIndicator = Sprite::create(kArtAppAssetLoc + "popup/art_selection_indicator.png");
    _selectionIndicator->setAnchorPoint(Vec2(0.5,0.3));
    _selectionIndicator->setVisible(false);
    _selectionIndicator->setPositionY(_stickerScrollView->getPosition().y + _stickerScrollView->getContentSize().height/2);
    _stickerCatBG->addChild(_selectionIndicator);
    
    for(int i = 0; i < _stickerCats.size(); i++)
    {
        ui::Button* stickerCatButton = ui::Button::create();
        stickerCatButton->loadTextures(_stickerCats[i]->first, _stickerCats[i]->first);
        stickerCatButton->setAnchorPoint(Vec2(0.5,0.5));
        stickerCatButton->setNormalizedPosition(Vec2((i+0.5f)/(float)_stickerCats.size(),0.5));
        stickerCatButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onStickerCategoryChangePressed, this,i));
        _stickerCategoryLayout->addChild(stickerCatButton);
        if(i == 0)
        {
            this->onStickerCategoryChangePressed(stickerCatButton, cocos2d::ui::Widget::TouchEventType::ENDED, i); //load first sticker set
            _selectionIndicator->setNormalizedPosition(Vec2((i+0.5f)/(float)_stickerCats.size(),0));
        }
    }
    
    _closeStickerSelectButton = ui::Button::create();
    _closeStickerSelectButton->setAnchorPoint(Vec2(0.5,0.5));
    _closeStickerSelectButton->setPosition(_stickerCategoryLayout->getPosition() + Vec2(_stickerCategoryLayout->getContentSize().width/2,_stickerCategoryLayout->getContentSize().height));
    _closeStickerSelectButton->loadTextures(kArtAppAssetLoc + "close_button.png", kArtAppAssetLoc + "close_button.png");
    _closeStickerSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onCloseStickerSelectPressed, this));
    _closeStickerSelectButton->setVisible(false);
    
    this->addChild(_closeStickerSelectButton,POPUP_UI_LAYER);
}

void DrawingCanvasUILayer::addBrushRadiusSlider(const Size& visibleSize, const Point& visibleOrigin)
{
    _brushSizeSlider = ui::Slider::create();
    _brushSizeSlider->setTouchEnabled(true);
    _brushSizeSlider->loadBarTexture(kArtAppAssetLoc + "size_grove.png");
    _brushSizeSlider->loadSlidBallTextures(kArtAppAssetLoc + "size_controler.png",kArtAppAssetLoc + "size_controler.png","");
    _brushSizeSlider->setPercent(50);
    _drawingCanvas->setBrushRadius(INITIAL_RADIUS + _brushSizeSlider->getPercent());
    _brushSizeSlider->setAnchorPoint(Vec2(0.5,0.5));
    _brushSizeSlider->setRotation(-90);
    _brushSizeSlider->setPosition(Vec2(visibleOrigin.x + _brushSizeSlider->getContentSize().height*2,visibleOrigin.y + visibleSize.height - _brushSizeSlider->getContentSize().width/2 - visibleSize.height * 0.35f));
    _brushSizeSlider->addEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onRadiusSliderInteract, this));
    
    Sprite* plusIcon = Sprite::create(kArtAppAssetLoc + "fill_1.png");
    plusIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    plusIcon->setNormalizedPosition(Vec2(1.1,0.5));
    _brushSizeSlider->addChild(plusIcon);
    
    Sprite* minusIcon = Sprite::create(kArtAppAssetLoc + "fill_1_copy.png");
    minusIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    minusIcon->setNormalizedPosition(Vec2(-0.1,0.5));
    _brushSizeSlider->addChild(minusIcon);
    
    this->addChild(_brushSizeSlider,MAIN_UI_LAYER);
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        _brushSizeSlider->setScale(0.75);
        _brushSizeSlider->setPosition(_brushSizeSlider->getPosition() + Vec2(50,0));
        
    }
    
}

void DrawingCanvasUILayer::addBrushTool(const std::string &buttonFilename, const std::string &bodyFilename, BrushType type, Vec2 normalisedPos, bool selected)
{
    ui::Button* brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(normalisedPos);
    brushButton->loadTextures(buttonFilename, buttonFilename);
    if(selected)
    {
        brushButton->setScale(((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height) * 1.15);
    }
    else
    {
        brushButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    }
    
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onToolChanged,this,type));
    ClippingNode* buttonBody = ClippingNode::create();
    Sprite* stencil =Sprite::create(bodyFilename);
    stencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    stencil->setPosition(brushButton->getContentSize()/2);
    buttonBody->setStencil(stencil);
    buttonBody->setName("body");
    buttonBody->setAlphaThreshold(0.5f);
    Sprite* clippedImage = Sprite::create(kArtAppAssetLoc + "white_bg.png");
    if(selected)
    {
        clippedImage->setColor(Color3B(_drawingCanvas->getSelectedColour()));
    }
    else
    {
        clippedImage->setColor(Color3B::WHITE);
    }
    
    clippedImage->setPosition(brushButton->getContentSize()/2);
    clippedImage->setScale(clippedImage->getContentSize().width/brushButton->getContentSize().width);
    buttonBody->addChild(clippedImage);
    brushButton->addChild(buttonBody,-1);
    _toolButtonLayout->addChild(brushButton);
    if(selected)
    {
        _selectedToolButton = brushButton;
    }
}

/*************************************************************************************
 ***********************   BUTTON CALLBACKS   ****************************************
 *************************************************************************************/

void DrawingCanvasUILayer::onClearButtonPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        _drawingCanvas->setListenerEnabled(false);
        _confirmDeleteImagePopup->setVisible(true);
        setUIEnabled(false);
    }
}

void DrawingCanvasUILayer::onUndoButtonPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        if(_drawingCanvas->getDrawingStackSize() > 0)
        {
            _drawingCanvas->undoAction();
        }
    }
}

void DrawingCanvasUILayer::onColourChangePressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        _selected->retain();
        _selected->removeFromParent();
        pressedButton->addChild(_selected);
        _selected->release();
        pressedButton->setScale(baseScale / 0.85f);
        
        _colourButtonLayout->setVisible(false);
        _drawingCanvas->setListenerEnabled(true);
        setUIEnabled(true);
        _drawingCanvas->removeCurrentDrawNode();
        if(pressedButton->getName() == "")
        {
            const Color3B& colour = pressedButton->getColor();
            _drawingCanvas->setSelectedColour(Color4F(colour));
            _drawingCanvas->setSelectedPattern("");
            _drawingCanvas->setSelectedPatternTansparant("");
            
            setButtonBodyColour(_colourSelectButton, colour);
            
            if(_selectedToolButton->getName() != "eraser")
            {
                setButtonBodyColour(_selectedToolButton, colour);
            }
            setOnlyPatternBrushesEnabled(false);
            
        }
        else
        {
            const std::string& pattern = PatternFileStorage::getInstance()->getNormalFileByName(pressedButton->getName());
            _drawingCanvas->setSelectedPattern(pattern);
            _drawingCanvas->setSelectedPatternTansparant(PatternFileStorage::getInstance()->getTransparantFileByName(pressedButton->getName()));
            
            setButtonBodyPattern(_colourSelectButton, pattern);
            if(_selectedToolButton->getName() != "eraser")
            {
                setButtonBodyPattern(_selectedToolButton, pattern);
            }
            
            setOnlyPatternBrushesEnabled(true);
        }
        _drawingCanvas->addNewDrawNode();
    }
    else if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvasUILayer::onColourSelectPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        setUIEnabled(false);
        _colourButtonLayout->setVisible(true);
        _drawingCanvas->setListenerEnabled(false);
        
    }
    else if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvasUILayer::onCloseColourSelectPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        
        setUIEnabled(true);
        _colourButtonLayout->setVisible(false);
        _drawingCanvas->setListenerEnabled(true);
        
    }
    else if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvasUILayer::onAddStickerPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        setStickerPopupVisible(false);

        _drawingCanvas->setupStickerNode(pressedButton->getNormalFile().file);

        setStickerUIEnabled(true);
    }
    else if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvasUILayer::onAddStickerButtonPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        setUIEnabled(false);
        setStickerPopupVisible(true);
        
        _drawingCanvas->setListenerEnabled(false);
        
    }
    else if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvasUILayer::onCloseStickerSelectPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        
        setUIEnabled(true);
        setStickerPopupVisible(false);
        _drawingCanvas->setListenerEnabled(true);
        
    }
    else if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvasUILayer::onConfirmStickerPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        setStickerUIEnabled(false);
        _drawingCanvas->setListenerEnabled(true);
        
        //add sticker as node in drawing scene on undo stack
        
        _drawingCanvas->addStickerToDrawing();
        
    }
    else if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvasUILayer::onCancelStickerPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        setStickerUIEnabled(false);
        _drawingCanvas->setListenerEnabled(true);
        
    }
    else if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvasUILayer::onConfirmDeletePressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        _confirmDeleteImagePopup->setVisible(false);
        setUIEnabled(true);
        _drawingCanvas->setListenerEnabled(true);
        _drawingCanvas->clearDrawing();
    }
    else if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvasUILayer::onCancelDeletePressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(baseScale * 0.85f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        _confirmDeleteImagePopup->setVisible(false);
        setUIEnabled(true);
        _drawingCanvas->setListenerEnabled(true);
        
    }
    else if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(baseScale / 0.85f);
    }
}

void DrawingCanvasUILayer::onToolChanged(Ref *pSender, ui::Widget::TouchEventType eEventType, int index)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    float baseScale = pressedButton->getScale();
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        if(pressedButton != _selectedToolButton)
        {
            pressedButton->setScale(baseScale * 1.15f);
            AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
        }
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        if(pressedButton != _selectedToolButton)
        {
            _selectedToolButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/_selectedToolButton->getContentSize().height);
            if(_selectedToolButton->getName() != "eraser")
            {
                setButtonBodyColour(_selectedToolButton, Color3B::WHITE);
            }
            if(pressedButton->getName() != "eraser")
            {
                const std::string& pattern = _drawingCanvas->getSelectedPattern();
                if(pattern != "")
                {
                    setButtonBodyPattern(pressedButton, pattern);
                }
                else
                {
                    setButtonBodyColour(pressedButton, Color3B(_drawingCanvas->getSelectedColour()));
                }
            }
            _selectedToolButton = pressedButton;
            _drawingCanvas->changeTool(index);
        }
        
    }
    else if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        if(pressedButton != _selectedToolButton)
        {
            pressedButton->setScale(baseScale / 1.15f);
        }
    }
}

void DrawingCanvasUILayer::onRadiusSliderInteract(Ref *pSender, ui::Slider::EventType eEventType)
{
    ui::Slider* slider = static_cast<ui::Slider*>(pSender);
    
    if(eEventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        _drawingCanvas->setBrushRadius(INITIAL_RADIUS + slider->getPercent());
    }
}

void DrawingCanvasUILayer::onStickerCategoryChangePressed(Ref *pSender, ui::Widget::TouchEventType eEventType, int index)
{
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    else if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        auto catButtons = _stickerCategoryLayout->getChildren();
        
        _selectionIndicator->setNormalizedPosition(Vec2((index + 0.5f)/catButtons.size(),0));
        const Size& visibleSize = Director::getInstance()->getVisibleSize();
        
        int numStickers = (int)_stickerCats[index]->second.size();
        
        _stickerScrollView->removeAllChildren();
        _stickerScrollView->setInnerContainerSize(Size(visibleSize.width/7.0f * numStickers/2.0f, visibleSize.height/2));
        
        for(int i = 0; i < numStickers; i+=2)
        {
            ui::Button* temp = ui::Button::create();
            temp->setAnchorPoint(Vec2(0.5,0.5));
            temp->loadTextures(_stickerCats[index]->second[i], _stickerCats[index]->second[i]);
            temp->setPosition(Vec2(_stickerScrollView->getInnerContainerSize().width*((i+1.0f)/(numStickers+1)),_stickerScrollView->getInnerContainerSize().height*0.8));
            temp->setScale(271.0f/temp->getContentSize().height);
            temp->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onAddStickerPressed, this));
            _stickerScrollView->addChild(temp);
            
            if(i+1 < numStickers)
            {
                ui::Button* temp2 = ui::Button::create();
                temp2->setAnchorPoint(Vec2(0.5,0.5));
                temp2->loadTextures( _stickerCats[index]->second[i+1], _stickerCats[index]->second[i+1]);
                temp2->setPosition(Vec2(_stickerScrollView->getInnerContainerSize().width*((i+1.0f)/(numStickers+1)),_stickerScrollView->getInnerContainerSize().height*0.4));
                temp2->setScale(271.0f/temp2->getContentSize().height);
                temp2->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onAddStickerPressed, this));
                _stickerScrollView->addChild(temp2);
            }
            
        }
        
    }
}

/*************************************************************************************
 ***********************   UI TOGGLE TOOLS    ****************************************
 *************************************************************************************/

void DrawingCanvasUILayer::setUIVisible(bool isVisible)
{
    _toolButtonLayout->setVisible(isVisible);
    _colourSelectButton->setVisible(isVisible);
    _addStickerButton->setVisible(isVisible);
    _clearButton->setVisible(isVisible);
    _undoButton->setVisible(isVisible);
    _brushSizeSlider->setVisible(isVisible);
}

void DrawingCanvasUILayer::setUIEnabled(bool isEnabled)
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
    _undoButton->setEnabled(isEnabled);
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

void DrawingCanvasUILayer::setOnlyPatternBrushesEnabled(bool isEnabled)
{
    auto toolButtons = _toolButtonLayout->getChildren();
    ui::Button* brushBtn = (ui::Button*)toolButtons.at(1);
    brushBtn->setEnabled(!isEnabled);
    
    if(isEnabled)
    {
        if(_selectedToolButton == brushBtn)
        {
            //force tool change
            _drawingCanvas->addNewDrawNode(); //re-add draw node from paintbruch because it will have been removed on colour change and will be removed again on tool change
            onToolChanged(toolButtons.at(0), ui::Widget::TouchEventType::BEGAN, PEN);
            onToolChanged(toolButtons.at(0), ui::Widget::TouchEventType::ENDED, PEN);
        }
        setButtonBodyColour(brushBtn, Style::Color::charcoalGrey);
    }
    
    if(!isEnabled && _selectedToolButton != brushBtn)
    {
        setButtonBodyColour(brushBtn, Color3B::WHITE);
    }
    
    
}

void DrawingCanvasUILayer::setStickerUIEnabled(bool isEnabled)
{
    _confirmStickerButton->setVisible(isEnabled);
    _cancelStickerButton->setVisible(isEnabled);
    _drawingCanvas->setStickerNodeVisible(isEnabled);
    
    _clearButton->setVisible(!isEnabled);
    _undoButton->setVisible(!isEnabled);
    
    auto toolButtons = _toolButtonLayout->getChildren();
    
    for(Node* btnNode : toolButtons)
    {
        ui::Widget* toolButt = (ui::Widget*)btnNode;
        toolButt->setEnabled(!isEnabled);
    }
    _colourSelectButton->setEnabled(!isEnabled);
    _addStickerButton->setEnabled(!isEnabled);
    _clearButton->setEnabled(!isEnabled);
    _undoButton->setEnabled(!isEnabled);
    _brushSizeSlider->setEnabled(!isEnabled);
    
    if(isEnabled)
    {
        _overlay->setVisible(!isEnabled);
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(_overlay);
    }
    
}

void DrawingCanvasUILayer::setStickerPopupVisible(bool isVisible)
{
    _stickerScrollView->setVisible(isVisible);
    _stickerCategoryLayout->setVisible(isVisible);
    _closeStickerSelectButton->setVisible(isVisible);
    _stickerCatBG->setVisible(isVisible);
    _selectionIndicator->setVisible(isVisible);
    _stickerPopupBG->setVisible(isVisible);
}

void DrawingCanvasUILayer::setButtonBodyColour(ui::Button *button, Color3B colour)
{
    Node* toolBody = button->getChildByName("body");
    toolBody->removeAllChildren();
    Sprite* toolClippedImage = Sprite::create(kArtAppAssetLoc + "white_bg.png");
    toolClippedImage->setColor(colour);
    toolClippedImage->setPosition(button->getContentSize()/2);
    toolClippedImage->setScale(toolClippedImage->getContentSize().height/button->getContentSize().height);
    toolBody->addChild(toolClippedImage);
}

void DrawingCanvasUILayer::setButtonBodyPattern(cocos2d::ui::Button *button, const std::string &pattern)
{
    Node* toolBody = button->getChildByName("body");
    toolBody->removeAllChildren();
    Sprite* toolClippedImage = Sprite::create(pattern);
    toolClippedImage->setPosition(button->getContentSize()/2);
    toolClippedImage->setScale(toolClippedImage->getContentSize().height/button->getContentSize().height);
    toolBody->addChild(toolClippedImage);
}

//---------------------Sticker file collection methods ----------------------------------------//

void DrawingCanvasUILayer::getStickerFilesFromJSON()
{
    
    const std::string& fullFileText = FileUtils::getInstance()->getStringFromFile(kStickerLoc + "catalogue.json");
    
    rapidjson::Document json;
    json.Parse(fullFileText.c_str());
    
    const rapidjson::Value& categories = json["categories"];
    for(auto it = categories.Begin(); it != categories.End(); ++it)
    {
        std::vector<std::string> catStickers;
        const auto& jsonCatEntry = *it;
        const std::string& catName = jsonCatEntry["image_location"].GetString();
        
        const rapidjson::Value& stickersJson = jsonCatEntry["stickers"];
        for(auto it = stickersJson.Begin(); it != stickersJson.End(); ++it)
        {
            const auto& jsonStickEntry = *it;
            const std::string& sticker = kStickerLoc + jsonStickEntry.GetString();
            catStickers.push_back(sticker);
        }
        
        StickerSetRef categorySet = std::make_shared<StickerSet>();
        categorySet->first = kStickerLoc + catName;
        categorySet->second = catStickers;
        
        _stickerCats.push_back(categorySet);
        
    }
    
    const std::string& oomeeStoragePath = FileUtils::getInstance()->getWritablePath() + "oomeeMaker/" + ChildDataProvider::getInstance()->getParentOrChildId();
    const std::vector<std::string>& oomeeImages = DirectorySearcher::getInstance()->getImagesInDirectory(oomeeStoragePath);
    
    if(oomeeImages.size() == 0)
    {
        return;
    }
    
    std::vector<std::string> fullFilenames;
    for(const std::string& img : oomeeImages)
    {
        fullFilenames.push_back(oomeeStoragePath + "/" + img);
    }
    StickerSetRef oomeeCat = std::make_shared<StickerSet>();
    oomeeCat->first = "res/navigation/menu_childSelect.png";
    oomeeCat->second = fullFilenames;
    
    _stickerCats.push_back(oomeeCat);
    
    
}





NS_AZOOMEE_AA_END
