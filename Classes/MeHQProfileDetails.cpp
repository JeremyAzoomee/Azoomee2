//
//  MeHQProfileDetails.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 14/05/2018.
//

#include "MeHQProfileDetails.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool MeHQProfileDetails::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    bool isPortrait = visibleSize.width < visibleSize.height;
    
    this->setContentSize(Size(visibleSize.width, isPortrait ? 1500 : 1000)); // portrait stacks elements vertically, so 2x height
    
    const Size& contentSize = this->getContentSize();
    
    setLayoutType(isPortrait ? ui::Layout::Type::VERTICAL : ui::Layout::Type::HORIZONTAL);
    
    auto avatarLayout = ui::Layout::create();
    avatarLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    avatarLayout->setSizePercent(isPortrait ? Vec2(1.0,0.66) : Vec2(0.5,1.0));
    this->addChild(avatarLayout);
    
    _avatar = OomeeMaker::OomeeCarouselButton::create();
    _avatar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _avatar->setNormalizedPosition(Vec2(isPortrait ? 0.5 : 0.6,0.5));
    _avatar->setPlaceholderImage("res/oomeeMaker/1_Oomee_Reference.png");
    _avatar->loadPlaceholderImage();
    _avatar->setScale((contentSize.height * (isPortrait ? 0.6 : 0.9)) / _avatar->getContentSize().height);
    _avatar->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(OomeeMakerEntryPointScene));
        }
    });
    _profileImageDownloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
    _profileImageDownloader->downloadImage(this, ChildDataProvider::getInstance()->getLoggedInChildAvatarId());
    
    avatarLayout->addChild(_avatar);
    
    Sprite* editIcon = Sprite::create("res/oomeeMaker/edit_button.png");
    editIcon->setAnchorPoint(Vec2(1.25,1.25));
    editIcon->setPosition(_avatar->getContentSize());
    editIcon->setScale(0.5);
    _avatar->addChild(editIcon);
    
    _labelLayout = ui::Layout::create();
    _labelLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _labelLayout->setSizePercent(isPortrait ? Vec2(1.0,0.34) : Vec2(0.5,1.0));
    this->addChild(_labelLayout);
    
    _nameLabel = Label::createWithTTF(ChildDataProvider::getInstance()->getLoggedInChildName(),Style::Font::Regular , 243);
    _nameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _nameLabel->setNormalizedPosition(Vec2(0.5,isPortrait ? 0.60 : 0.75));
    _nameLabel->setContentSize(Size(contentSize.width /2, contentSize.height / 3.0f));
    
    reduceLabelTextToFitWidth(_nameLabel, contentSize.width * _labelLayout->getSizePercent().x);
    
    _labelLayout->addChild(_nameLabel);
    
    _kidCodeLabel = ui::Text::create("Kid Code: " + ParentDataProvider::getInstance()->getInviteCodeForAnAvailableChild(ChildDataProvider::getInstance()->getLoggedInChildNumber()), Style::Font::Regular, 96);
    _kidCodeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _kidCodeLabel->setNormalizedPosition(Vec2(0.5,isPortrait ? 0.20 : 0.5));
    _kidCodeLabel->setContentSize(Size(contentSize.width /2, contentSize.height / 3.0f));
    _kidCodeLabel->setTextColor(Color4B(Style::Color::greenishTeal));
    _labelLayout->addChild(_kidCodeLabel);
    if(!isPortrait)
    {
        _editOomeeButton = ui::Button::create("res/buttons/MainButton.png");
        _editOomeeButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _editOomeeButton->setNormalizedPosition(Vec2(0.5,0.25));
        _editOomeeButton->setScale9Enabled(true);
        _editOomeeButton->setColor(Style::Color::greenishTeal);
        _editOomeeButton->setCapInsets(Rect(_editOomeeButton->getContentSize().width/2,0,1,_editOomeeButton->getContentSize().height));
        _editOomeeButton->setContentSize(Size(940, _editOomeeButton->getContentSize().height));
        _editOomeeButton->ignoreContentAdaptWithSize(false);
        
        _editOomeeButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                Director::getInstance()->replaceScene(SceneManagerScene::createScene(OomeeMakerEntryPointScene));
            }
        });
        
        Label* editOomeeLabel = Label::createWithTTF("Edit My Oomee", Style::Font::Regular, _editOomeeButton->getContentSize().height * 0.4f);
        editOomeeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        editOomeeLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        _editOomeeButton->addChild(editOomeeLabel);
    
        _labelLayout->addChild(_editOomeeButton);
    }
    
    return true;
}

void MeHQProfileDetails::onEnter()
{
    Super::onEnter();
}

void MeHQProfileDetails::onExit()
{
    _profileImageDownloader->setDelegate(nullptr);
    Super::onExit();
}

void MeHQProfileDetails::onSizeChanged()
{
    Super::onSizeChanged();
}

void MeHQProfileDetails::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _avatar->setMainImage(downloader->getLocalImagePath());
}

void MeHQProfileDetails::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END
