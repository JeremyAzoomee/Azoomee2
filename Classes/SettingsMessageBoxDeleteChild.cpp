//
//  SettingsMessageBoxDeleteChild.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 07/09/2018.
//

#include "SettingsMessageBoxDeleteChild.h"
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void SettingsMessageBoxDeleteChild::onEnter()
{
    Super::onEnter();
    
    _messageBox->setContentSize(Size(_visibleSize.width - 100, 920));
    
    _heading = Label::createWithTTF(_("Are you sure you want to delete this profile?"), Style::Font::Medium(), 91);
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
    
    _deleteButton = SettingsMessageBoxButton::create();
    _deleteButton->loadTextureNormal("res/settings/rounded_button.png");
    _deleteButton->setColor(Style::Color::watermelon);
    _deleteButton->setMessageBoxButtonType(SettingsMessageBoxButtonType::DELETE);
    _deleteButton->setNormalizedPosition(Vec2(0.5,0.12));
    _deleteButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _deleteButton->addTouchEventListener(CC_CALLBACK_2(SettingsMessageBox::onButtonPressed, this));
    _messageBox->addChild(_deleteButton);
    
    Label* deleteLabel = Label::createWithTTF(_("Delete"), Style::Font::Medium(), 75);
    deleteLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    deleteLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _deleteButton->addChild(deleteLabel);
    
}

NS_AZOOMEE_END
