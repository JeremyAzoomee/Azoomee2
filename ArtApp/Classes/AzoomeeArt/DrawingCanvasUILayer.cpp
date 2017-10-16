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
    
    
    
    addClearButton(visibleSize, visibleOrigin);
    addColourSelectButtons(visibleSize, visibleOrigin);
    addToolSelectButtons(visibleSize, visibleOrigin);
    addBrushRadiusSlider(visibleSize, visibleOrigin);
    addStickerSelectButtons(visibleSize, visibleOrigin);
    
}

//UI LOADING

void DrawingCanvasUILayer::addClearButton(const Size& visibleSize, const Point& visibleOrigin)
{
    _clearButton = ui::Button::create();
    _clearButton->setAnchorPoint(Vec2(0.5,0));
    _clearButton->setPosition(Vec2(_rightBG->getPosition().x - 2*_rightBG->getContentSize().width/3, visibleOrigin.y));
    _clearButton->loadTextures(kArtAppAssetLoc + "delete.png", kArtAppAssetLoc + "delete.png");
    _clearButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onClearButtonPressed, this));
    this->addChild(_clearButton,MAIN_UI_LAYER);
    
    _undoButton = ui::Button::create();
    _undoButton->setAnchorPoint(Vec2(0.5,0));
    _undoButton->setPosition(Vec2(_rightBG->getPosition().x - _rightBG->getContentSize().width/3, visibleOrigin.y));
    _undoButton->loadTextures(kArtAppAssetLoc + "redo.png", kArtAppAssetLoc + "redo.png");
    _undoButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onUndoButtonPressed, this));
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
    _confrimDeleteButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onConfirmDeletePressed, this));
    
    _confirmDeleteImagePopup->addChild(_confrimDeleteButton);
    
    _cancelDeleteButton = ui::Button::create();
    _cancelDeleteButton->loadTextures(kArtAppAssetLoc + "close.png", kArtAppAssetLoc + "close.png");
    _cancelDeleteButton->setAnchorPoint(Vec2(0,0));
    _cancelDeleteButton->setPosition(background->getPosition() - Vec2(background->getContentSize().width/3, background->getContentSize().height/3));
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
                _drawingCanvas->setSelectedColour(Color4F(_kColours[colourCount]));
                button->addChild(_selected);
            }
            button->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onColourChangePressed,this));
            _colourButtonLayout->addChild(button);
            
            colourCount++;
        }
    }
    
    ui::Button* closeButton = ui::Button::create();
    closeButton->setAnchorPoint(Vec2(0.5,0.5));
    closeButton->setPosition(_colourButtonLayout->getContentSize());
    closeButton->loadTextures(kArtAppAssetLoc + "close_button.png", kArtAppAssetLoc + "close_buton.png");
    closeButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onCloseColourSelectPressed, this));
    _colourButtonLayout->addChild(closeButton);
    
    _colourSelectButton = ui::Button::create();
    _colourSelectButton->setAnchorPoint(Vec2(1,0));
    _colourSelectButton->setPosition(Vec2(_leftBG->getPosition().x + _leftBG->getContentSize().width/2, visibleOrigin.y));
    _colourSelectButton->loadTextures(kArtAppAssetLoc + "art_button_colour.png", kArtAppAssetLoc + "art_button_colour.png");
    _colourSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onColourSelectPressed,this));
    _colourSelectButton->setColor(Color3B(_drawingCanvas->getSelectedColour()));
    
    this->addChild(_colourSelectButton,MAIN_UI_LAYER);
}

void DrawingCanvasUILayer::addToolSelectButtons(const Size& visibleSize, const Point& visibleOrigin)
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
    _addStickerButton->setAnchorPoint(Vec2(1,0));
    _addStickerButton->setPosition(Vec2(_leftBG->getPositionX() + _leftBG->getContentSize().width, _colourSelectButton->getPosition().y));
    _addStickerButton->loadTextures(kArtAppAssetLoc + "art_button_sticker.png", kArtAppAssetLoc + "art_button_sticker.png");
    _addStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onAddStickerButtonPressed,this));
    
    this->addChild(_addStickerButton,MAIN_UI_LAYER);
    
    _cancelStickerButton = ui::Button::create();
    _cancelStickerButton->setAnchorPoint(Vec2(0,0));
    _cancelStickerButton->setPosition(Vec2(visibleOrigin.x + BUTTON_OFFSET.x, visibleOrigin.y + BUTTON_OFFSET.y));
    _cancelStickerButton->loadTextures(kArtAppAssetLoc + "delete.png", kArtAppAssetLoc + "delete.png");
    _cancelStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onCancelStickerPressed,this));
    _cancelStickerButton->setVisible(false);
    this->addChild(_cancelStickerButton,STICKER_UI_LAYER);
    
    _confirmStickerButton = ui::Button::create();
    _confirmStickerButton->setAnchorPoint(Vec2(1,0));
    _confirmStickerButton->setPosition(Vec2(visibleOrigin.x + visibleSize.width - BUTTON_OFFSET.x, visibleOrigin.y + BUTTON_OFFSET.y));
    _confirmStickerButton->loadTextures(kArtAppAssetLoc + "confirm.png", kArtAppAssetLoc + "confirm.png");
    _confirmStickerButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onConfirmStickerPressed,this));
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
        stickerCatButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onStickerCategoryChangePressed, this,i));
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
    _closeStickerSelectButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onCloseStickerSelectPressed, this));
    _closeStickerSelectButton->setVisible(false);
    
    this->addChild(_closeStickerSelectButton,POPUP_UI_LAYER);
}

void DrawingCanvasUILayer::addBrushRadiusSlider(const Size& visibleSize, const Point& visibleOrigin)
{
    _brushSizeSlider = ui::Slider::create();
    _brushSizeSlider->setTouchEnabled(true);
    _brushSizeSlider->loadBarTexture(kArtAppAssetLoc + "slideBack.png");
    _brushSizeSlider->loadSlidBallTextures(kArtAppAssetLoc + "sliderIcon.png",kArtAppAssetLoc + "sliderIcon.png","");
    _brushSizeSlider->setPercent(50);
    _drawingCanvas->setBrushRadius(INITIAL_RADIUS + _brushSizeSlider->getPercent());
    _brushSizeSlider->setAnchorPoint(Vec2(0.5,0.5));
    _brushSizeSlider->setRotation(-90);
    _brushSizeSlider->setPosition(Vec2(visibleOrigin.x + _brushSizeSlider->getContentSize().height,visibleOrigin.y + visibleSize.height/2));
    _brushSizeSlider->addEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onRadiusSliderInteract, this));
    
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
        _drawCanvasTouchListener->setEnabled(false);
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
        this->removeChild(_drawingStack.back());
        _drawingStack.pop_back();
        if(_drawingStack.size() == 0)
        {
            _undoButton->setEnabled(false);
        }
        
        _actionCounter--;
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
        _selectedColour = Color4F(pressedButton->getColor());
        _selected->retain();
        _selected->removeFromParent();
        pressedButton->addChild(_selected);
        _selected->release();
        pressedButton->setScale(baseScale / 0.85f);
        
        _colourButtonLayout->setVisible(false);
        _colourSelectButton->setColor(Color3B(_selectedColour));
        setUIEnabled(true);
        if(_selectedToolButton->getName() != "eraser")
        {
            _selectedToolButton->setColor(Color3B(_selectedColour));
        }
        _drawCanvasTouchListener->setEnabled(true);
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
        _drawCanvasTouchListener->setEnabled(false);
        
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
        _drawCanvasTouchListener->setEnabled(true);
        
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
        _drawCanvasTouchListener->setEnabled(false);
        
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
        _drawCanvasTouchListener->setEnabled(true);
        
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
        _drawCanvasTouchListener->setEnabled(true);
        
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

void DrawingCanvasUILayer::onRadiusSliderInteract(Ref *pSender, ui::Slider::EventType eEventType)
{
    ui::Slider* slider = static_cast<ui::Slider*>(pSender);
    
    if(eEventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        _brushRadius = INITIAL_RADIUS + slider->getPercent();
    }
}

void DrawingCanvasUILayer::onStickerCategoryChangePressed(Ref *pSender, ui::Widget::TouchEventType eEventType, int index)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    
    if(eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(1.15f);
        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
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
            temp->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onAddStickerPressed, this));
            _stickerScrollView->addChild(temp);
            
            if(i+1 < numStickers)
            {
                ui::Button* temp2 = ui::Button::create();
                temp2->setAnchorPoint(Vec2(0.5,0.5));
                temp2->loadTextures(kStickerLoc + _stickerCats[index].second[i+1],kStickerLoc + _stickerCats[index].second[i+1]);
                temp2->setPosition(Vec2(_stickerScrollView->getInnerContainerSize().width*((i+1.0f)/(numStickers+1)),visibleSize.height/4 - temp2->getContentSize().height/2));
                temp2->addTouchEventListener(CC_CALLBACK_2(DrawingCanvasUILayer::onAddStickerPressed, this));
                _stickerScrollView->addChild(temp2);
            }
            
        }
        
    }
    
    if(eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(1.15f);
    }
}

void DrawingCanvasUILayer::setUIVisible(bool isVisible)
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
