//
//  ArtStudioButton.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 11/10/2019.
//

#include "ArtStudioButton.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ArtStudioButton::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    ignoreContentAdaptWithSize(false);
    addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ArtAppEntryPointScene));
        }
    });
    
    _frame = RoundedRectSprite::create();
    _frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _frame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _frame->setColor(Style::Color::purplyPink);
    _frame->setTexture("res/decoration/white_1px.png");
    _frame->setCornerRadius(27);
    addChild(_frame);
    
    _background = RoundedRectSprite::create();
    _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _background->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _background->setTexture("res/OomeeHQ/ArtStudio/background.png");
    _background->setCornerRadius(27);
    _frame->addChild(_background);
    
    _tools = Sprite::create("res/OomeeHQ/ArtStudio/tools.png");
    _tools->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _tools->setNormalizedPosition(Vec2(0.35f, 0.0f));
    _background->addChild(_tools);
    
    _logo = Sprite::create("res/OomeeHQ/ArtStudio/art_studio_logo.png");
    _logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _logo->setNormalizedPosition(Vec2(0.75f, 0.5f));
    _background->addChild(_logo);
    
    _paint = RoundedRectSprite::create();
    _paint->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _paint->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    _paint->setTexture("res/OomeeHQ/ArtStudio/paint_drop_red.png");
    _paint->setCornerRadius(27);
    _paint->setRoundedCorners(false, false, false, true);
    _background->addChild(_paint);
    
    return true;
}

void ArtStudioButton::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    _frame->setContentSize(contentSize - Size(0,25));
    _background->setContentSize(contentSize - Size(10,35));
    _tools->setScale((contentSize.height - 5) / _tools->getContentSize().height);
    _logo->setScale((contentSize.height * 0.5f) / _logo->getContentSize().height);
    _paint->setContentSize(_paint->getTexture()->getContentSize() * _tools->getScale());
    
}


NS_AZOOMEE_END
