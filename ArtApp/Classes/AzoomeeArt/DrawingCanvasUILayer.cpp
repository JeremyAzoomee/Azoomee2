//
//  DrawingCanvasUILayer.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 16/10/2017.
//
//

#include "DrawingCanvasUILayer.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

const std::vector<Color3B> DrawingCanvasUILayer::_kColours = {
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
    bgColour->setContentSize(visibleSize);
    bgColour->setPosition(visibleOrigin);
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
    
    const Vec2& tableDimensions =  Vec2(7,ceilf(_kColours.size()/7.0f));
    
    _colourButtonLayout = Node::create();
    _colourButtonLayout->setContentSize(Size(visibleSize.width*0.75,visibleSize.height*(0.18*tableDimensions.y)));
    _colourButtonLayout->setAnchorPoint(Vec2(0.5,0.5));
    _colourButtonLayout->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y + visibleSize.height/2 + 100));
    _colourButtonLayout->setVisible(false);
    this->addChild(_colourButtonLayout,POPUP_UI_LAYER);
    
    ui::Scale9Sprite* colourBG = ui::Scale9Sprite::create(kArtAppAssetLoc + "popup_bg.png");
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
                _drawingCanvas->setSelectedColour(Color4F(_kColours[colourCount]));
                button->addChild(_selected);
            }
            button->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onColourChangePressed,this));
            _colourButtonLayout->addChild(button);
            
            colourCount++;
        }
    }
    
    ui::Button* closeButton = ui::Button::create();
    closeButton->setAnchorPoint(Vec2(0.66,0.66));
    closeButton->setPosition(_colourButtonLayout->getContentSize());
    closeButton->loadTextures(kArtAppAssetLoc + "close_button.png", kArtAppAssetLoc + "close_button.png");
    closeButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onCloseColourSelectPressed, this));
    _colourButtonLayout->addChild(closeButton);
    
    _colourSelectButton = ui::Button::create();
    _colourSelectButton->setAnchorPoint(Vec2(0.5,0.5));
    _colourSelectButton->loadTextures(kArtAppAssetLoc + "colour.png", kArtAppAssetLoc + "colour.png");
    _colourSelectButton->setPosition(visibleOrigin + Vec2(_colourSelectButton->getContentSize().width*2.5,visibleSize.height * 0.0825f));
    _colourSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onColourSelectPressed,this));
    _colourSelectButton->setColor(Color3B(_drawingCanvas->getSelectedColour()));
    Sprite* buttonFrame = Sprite::create(kArtAppAssetLoc + "colour_frame.png");
    buttonFrame->setAnchorPoint(Vec2(0.5,0.5));
    buttonFrame->setNormalizedPosition(Vec2(0.5,0.5));
    _colourSelectButton->addChild(buttonFrame);
    
    
    this->addChild(_colourSelectButton,MAIN_UI_LAYER);
}

void DrawingCanvasUILayer::addToolSelectButtons(const Size& visibleSize, const Point& visibleOrigin)
{
    
    _toolButtonLayout = Node::create();
    _toolButtonLayout->setContentSize(Size(visibleSize.width*0.6f,visibleSize.height*0.175f));
    _toolButtonLayout->setAnchorPoint(Vec2(0.5,0));
    _toolButtonLayout->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,visibleOrigin.y));
    
    this->addChild(_toolButtonLayout,MAIN_UI_LAYER);
    
    ui::Button* brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.1,0));
    brushButton->loadTextures(kArtAppAssetLoc + "pencil.png", kArtAppAssetLoc + "pencil.png");
    brushButton->setColor(Color3B(_drawingCanvas->getSelectedColour()));
    brushButton->setScale(((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height) * 1.15);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onToolChanged,this,PEN));
    Sprite* buttonFrame = Sprite::create(kArtAppAssetLoc + "pencil_frame.png");
    buttonFrame->setAnchorPoint(Vec2(0.5,0));
    buttonFrame->setNormalizedPosition(Vec2(0.5,0));
    brushButton->addChild(buttonFrame);
    _selectedToolButton = brushButton;
    _toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.3,0));
    brushButton->loadTextures(kArtAppAssetLoc + "brush.png", kArtAppAssetLoc + "brush.png");
    brushButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onToolChanged,this,PAINTBRUSH));
    buttonFrame = Sprite::create(kArtAppAssetLoc + "brush_frame.png");
    buttonFrame->setAnchorPoint(Vec2(0.5,0));
    buttonFrame->setNormalizedPosition(Vec2(0.5,0));
    brushButton->addChild(buttonFrame);
    _toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.5,0));
    brushButton->loadTextures(kArtAppAssetLoc + "felt_tip.png", kArtAppAssetLoc + "felt_tip.png");
    brushButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onToolChanged,this,HIGHLIGHTER));
    buttonFrame = Sprite::create(kArtAppAssetLoc + "felt_tip_frame.png");
    buttonFrame->setAnchorPoint(Vec2(0.5,0));
    buttonFrame->setNormalizedPosition(Vec2(0.5,0));
    brushButton->addChild(buttonFrame);
    _toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
    brushButton->setAnchorPoint(Vec2(0.5,0));
    brushButton->setNormalizedPosition(Vec2(0.7,0));
    brushButton->loadTextures(kArtAppAssetLoc + "can.png", kArtAppAssetLoc + "can.png");
    brushButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/brushButton->getContentSize().height);
    brushButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onToolChanged,this,SPRAYCAN));
    buttonFrame = Sprite::create(kArtAppAssetLoc + "can_frame.png");
    buttonFrame->setAnchorPoint(Vec2(0.5,0));
    buttonFrame->setNormalizedPosition(Vec2(0.5,0));
    brushButton->addChild(buttonFrame);
    _toolButtonLayout->addChild(brushButton);
    
    brushButton = ui::Button::create();
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
    
    _stickerCats = getStickerFilesFromJSON();
    
    _selectionIndicator = Sprite::create(kArtAppAssetLoc + "popup/art_selection_indicator.png");
    _selectionIndicator->setAnchorPoint(Vec2(0.5,0.3));
    _selectionIndicator->setVisible(false);
    _selectionIndicator->setPositionY(_stickerScrollView->getPosition().y + _stickerScrollView->getContentSize().height/2);
    _stickerCatBG->addChild(_selectionIndicator);
    
    for(int i = 0; i < _stickerCats.size(); i++)
    {
        ui::Button* stickerCatButton = ui::Button::create();
        stickerCatButton->loadTextures(kStickerLoc + _stickerCats[i].first, kStickerLoc + _stickerCats[i].first);
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
    _brushSizeSlider->setPosition(Vec2(visibleOrigin.x + _brushSizeSlider->getContentSize().height*2,visibleOrigin.y + visibleSize.height/2));
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
    if(eEventType == ui::Widget::TouchEventType::ENDED)
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
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        _drawingCanvas->setSelectedColour(Color4F(pressedButton->getColor()));
        _selected->retain();
        _selected->removeFromParent();
        pressedButton->addChild(_selected);
        _selected->release();
        pressedButton->setScale(baseScale / 0.85f);
        
        _colourButtonLayout->setVisible(false);
        _colourSelectButton->setColor(Color3B(_drawingCanvas->getSelectedColour()));
        setUIEnabled(true);
        if(_selectedToolButton->getName() != "eraser")
        {
            _selectedToolButton->setColor(Color3B(_drawingCanvas->getSelectedColour()));
        }
        _drawingCanvas->setListenerEnabled(true);
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
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
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        setUIEnabled(false);
        _colourButtonLayout->setVisible(true);
        _drawingCanvas->setListenerEnabled(false);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
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
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        
        setUIEnabled(true);
        _colourButtonLayout->setVisible(false);
        _drawingCanvas->setListenerEnabled(true);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
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
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        _stickerScrollView->setVisible(false);
        _stickerCategoryLayout->setVisible(false);
        _closeStickerSelectButton->setVisible(false);
        _stickerCatBG->setVisible(false);
        _selectionIndicator->setVisible(false);
        _stickerPopupBG->setVisible(false);

        _drawingCanvas->setupStickerNode(pressedButton->getNormalFile().file);

        setStickerUIEnabled(true);
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
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
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        setUIEnabled(false);
        _stickerScrollView->setVisible(true);
        _stickerCategoryLayout->setVisible(true);
        _closeStickerSelectButton->setVisible(true);
        _stickerCatBG->setVisible(true);
        _selectionIndicator->setVisible(true);
        _stickerPopupBG->setVisible(true);
        
        _drawingCanvas->setListenerEnabled(false);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
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
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        
        setUIEnabled(true);
        _stickerScrollView->setVisible(false);
        _stickerCategoryLayout->setVisible(false);
        _closeStickerSelectButton->setVisible(false);
        _stickerCatBG->setVisible(false);
        _selectionIndicator->setVisible(false);
        _stickerPopupBG->setVisible(false);
        _drawingCanvas->setListenerEnabled(true);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
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
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        setStickerUIEnabled(false);
        _drawingCanvas->setListenerEnabled(true);
        
        //add sticker as node in drawing scene on undo stack
        
        _drawingCanvas->addStickerToDrawing();
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
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
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        setStickerUIEnabled(false);
        _drawingCanvas->setListenerEnabled(true);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
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
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        _confirmDeleteImagePopup->setVisible(false);
        setUIEnabled(true);
        _drawingCanvas->setListenerEnabled(true);
        _drawingCanvas->clearDrawing();
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
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
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        pressedButton->setScale(baseScale / 0.85f);
        _confirmDeleteImagePopup->setVisible(false);
        setUIEnabled(true);
        _drawingCanvas->setListenerEnabled(true);
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
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
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        if(pressedButton != _selectedToolButton)
        {
            _selectedToolButton->setScale((_toolButtonLayout->getContentSize().height*0.8)/_selectedToolButton->getContentSize().height);
            _selectedToolButton->setColor(Color3B::WHITE);
            if(pressedButton->getName() != "eraser")
            {
                pressedButton->setColor(Color3B(_drawingCanvas->getSelectedColour()));
            }
            _selectedToolButton = pressedButton;
            _drawingCanvas->changeTool(index);
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
    //ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        //pressedButton->setScale(1.15f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
    }
    
    if(eEventType == ui::Widget::TouchEventType::ENDED)
    {
        auto catButtons = _stickerCategoryLayout->getChildren();
        
        //for(int i = 0; i < catButtons.size(); i++)
        //{
        //    catButtons.at(i)->setScale(1);
        //}
        
        //pressedButton->setScale(1.15);
        
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
            temp->setPosition(Vec2(_stickerScrollView->getInnerContainerSize().width*((i+1.0f)/(numStickers+1)),_stickerScrollView->getInnerContainerSize().height*0.8));
            temp->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onAddStickerPressed, this));
            _stickerScrollView->addChild(temp);
            
            if(i+1 < numStickers)
            {
                ui::Button* temp2 = ui::Button::create();
                temp2->setAnchorPoint(Vec2(0.5,0.5));
                temp2->loadTextures(kStickerLoc + _stickerCats[index].second[i+1],kStickerLoc + _stickerCats[index].second[i+1]);
                temp2->setPosition(Vec2(_stickerScrollView->getInnerContainerSize().width*((i+1.0f)/(numStickers+1)),_stickerScrollView->getInnerContainerSize().height*0.4));
                temp2->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onAddStickerPressed, this));
                _stickerScrollView->addChild(temp2);
            }
            
        }
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        //pressedButton->setScale(1.15f);
    }
}

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
    _brushSizeSlider->setEnabled(!isEnabled);
    
    if(isEnabled)
    {
        _overlay->setVisible(!isEnabled);
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(_overlay);
    }
    
}

//---------------------Sticker file collection methods ----------------------------------------//

StickerFileStore DrawingCanvasUILayer::getStickerFilesFromJSON()
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
