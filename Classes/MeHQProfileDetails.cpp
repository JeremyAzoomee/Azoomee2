//
//  MeHQProfileDetails.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 14/05/2018.
//

#include "MeHQProfileDetails.h"
#include "SceneManagerScene.h"
#include "SimpleAudioEngine.h"
#include "HQDataProvider.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Strings.h>
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
    bool is3x4Device = false;
    if(isPortrait && (visibleSize.height / visibleSize.width) < HQDataProvider::getInstance()->k16x10LowerBound)
    {
        isPortrait = false;
        is3x4Device = true;
    }
    
    this->setContentSize(Size(visibleSize.width, isPortrait ? 1500 : 1000)); // portrait stacks elements vertically, so 1.5x height
    
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
    _avatar->ignoreContentAdaptWithSize(false);
    _avatar->setScale((contentSize.height * (isPortrait ? 0.6 : 0.9)) / _avatar->getContentSize().height);
    _avatar->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
			if(TutorialController::getInstance()->isTutorialActive() && TutorialController::getInstance()->getCurrentState() == TutorialController::kCreateOomee)
			{
				TutorialController::getInstance()->nextStep();
			}
			
            AnalyticsSingleton::getInstance()->contentItemSelectedEvent(ConfigStorage::kOomeeMakerURI);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Edit_Button.wav");
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OomeeMakerEntryPointScene));
        }
    });
	
	_profileImageDownloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
	_profileImageDownloader->downloadImage(this, ChildDataProvider::getInstance()->getParentOrChildAvatarId());
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
    
    _nameLabel = Label::createWithTTF(ChildDataProvider::getInstance()->getParentOrChildName(),Style::Font::Regular() , is3x4Device ? 96 : 140);
    _nameLabel->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE : Vec2::ANCHOR_MIDDLE_BOTTOM);
    _nameLabel->setNormalizedPosition(Vec2(0.5,isPortrait ? 0.60 : 0.5));
    _nameLabel->setContentSize(Size(contentSize.width /2, contentSize.height / 3.0f));
    
    reduceLabelTextToFitWidth(_nameLabel, contentSize.width * _labelLayout->getSizePercent().x);
    
    _labelLayout->addChild(_nameLabel);
    
	_kidCodeLabel = ui::Text::create(_("Kid Code:") + " " + ChildDataProvider::getInstance()->getLoggedInChild()->getInviteCode(), Style::Font::Regular(), is3x4Device ? 67 : 80);
    _kidCodeLabel->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE : Vec2::ANCHOR_MIDDLE_TOP);
    _kidCodeLabel->setNormalizedPosition(Vec2(0.5,isPortrait ? 0.25 : 0.45));
    _kidCodeLabel->setContentSize(Size(contentSize.width /2, contentSize.height / 3.0f));
    _kidCodeLabel->setTextColor(Color4B(Style::Color::greenishTeal));
    _labelLayout->addChild(_kidCodeLabel);
    
    return true;
}

void MeHQProfileDetails::onEnter()
{
    Super::onEnter();
	TutorialController::getInstance()->registerDelegate(this);
	if(TutorialController::getInstance()->isTutorialActive())
	{
		onTutorialStateChanged(TutorialController::getInstance()->getCurrentState());
	}
}

void MeHQProfileDetails::onExit()
{
	TutorialController::getInstance()->unRegisterDelegate(this);
	if(_profileImageDownloader)
	{
    	_profileImageDownloader->setDelegate(nullptr);
	}
    Super::onExit();
}

void MeHQProfileDetails::onSizeChanged()
{
    Super::onSizeChanged();
}

void MeHQProfileDetails::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    Size prevSize = _avatar->getContentSize();
    _avatar->setMainImage(downloader->getLocalImagePath());
    _avatar->setContentSize(prevSize);
}

void MeHQProfileDetails::onImageDownloadFailed()
{
    
}

void MeHQProfileDetails::onTutorialStateChanged(const std::string& stateId)
{
	_avatar->enableTutorialHighlight(stateId == TutorialController::kCreateOomee);
}


NS_AZOOMEE_END
