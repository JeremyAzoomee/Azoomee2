//
//  OomeeMakerButton.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 11/10/2019.
//

#include "OomeeMakerButton.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool OomeeMakerButton::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    ignoreContentAdaptWithSize(false);
    addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OomeeMakerEntryPointScene));
        }
    });
    
    _frame = RoundedRectSprite::create();
    _frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _frame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _frame->setColor(Style::Color::macaroniAndCheese);
    _frame->setTexture("res/decoration/white_1px.png");
    _frame->setCornerRadius(27);
    addChild(_frame);
    
    _background = RoundedRectSprite::create();
    _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _background->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _background->setTexture("res/OomeeHQ/OomeeMaker/background.png");
    _background->setCornerRadius(27);
    _frame->addChild(_background);
    
    _oomees = RoundedRectSprite::create();
    _oomees->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _oomees->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _oomees->setTexture("res/OomeeHQ/OomeeMaker/oomees.png");
    _oomees->setCornerRadius(27);
    _background->addChild(_oomees);
    
    return true;
}

void OomeeMakerButton::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    _frame->setContentSize(contentSize - Size(0,80));
    _background->setContentSize(contentSize - Size(10,90));
    _oomees->setContentSize(Size(MIN(_oomees->getTexture()->getPixelsWide() * (contentSize.height / _oomees->getTexture()->getPixelsHigh()), contentSize.width) - 10,contentSize.height - 5));
}

NS_AZOOMEE_END
