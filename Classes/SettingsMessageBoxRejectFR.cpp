//
//  SettingsMessageBoxRejectFR.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 06/09/2018.
//

#include "SettingsMessageBoxRejectFR.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void SettingsMessageBoxRejectFR::onEnter()
{
    Super::onEnter();
    
    _messageBox->setContentSize(Size(_visibleSize.width - 100, 920));
    
    _heading = Label::createWithTTF(_("Are you sure you want to reject this profile?"), Style::Font::Medium(), 91);
    _heading->setTextColor(Color4B::BLACK);
    _heading->setHorizontalAlignment(TextHAlignment::CENTER);
    _heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _heading->setNormalizedPosition(Vec2(0.5,0.88));
	_heading->setWidth((_visibleSize.width - 100) * 0.75f);
    _messageBox->addChild(_heading);
    
    _cancelButton = SettingsMessageBoxButton::create();
    _cancelButton->loadTextureNormal("res/settings/rounded_button.png");
    _cancelButton->setColor(Style::Color::skyBlue);
    _cancelButton->setMessageBoxButtonType(SettingsMessageBoxButtonType::CANCEL);
    _cancelButton->setNormalizedPosition(Vec2(0.5,0.3));
    _cancelButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _cancelButton->addTouchEventListener(CC_CALLBACK_2(SettingsMessageBox::onButtonPressed, this));
    _messageBox->addChild(_cancelButton);
    
    Label* cancelLabel = Label::createWithTTF(_("Cancel"), Style::Font::Medium(), 75);
    cancelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    cancelLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _cancelButton->addChild(cancelLabel);
    
    _rejectButton = SettingsMessageBoxButton::create();
    _rejectButton->loadTextureNormal("res/settings/rounded_button.png");
    _rejectButton->setColor(Style::Color::watermelon);
    _rejectButton->setMessageBoxButtonType(SettingsMessageBoxButtonType::REJECT);
    _rejectButton->setNormalizedPosition(Vec2(0.5,0.12));
    _rejectButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _rejectButton->addTouchEventListener(CC_CALLBACK_2(SettingsMessageBox::onButtonPressed, this));
    _messageBox->addChild(_rejectButton);
    
    Label* rejectLabel = Label::createWithTTF(_("Reject"), Style::Font::Medium(), 75);
    rejectLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rejectLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _rejectButton->addChild(rejectLabel);
    
}

NS_AZOOMEE_END
