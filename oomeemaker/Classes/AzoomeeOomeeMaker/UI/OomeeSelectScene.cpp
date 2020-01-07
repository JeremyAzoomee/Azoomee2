//
//  OomeeSelectScene.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 21/02/2018.
//

#include "OomeeSelectScene.h"
#include "OomeeMakerScene.h"
#include "SimpleAudioEngine.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Utils/DirUtil.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/Utils/TimeFunctions.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

void OomeeSelectScene::newOomee()
{
    AnalyticsSingleton::getInstance()->newOomee();
    OomeeMakerScene* makerScene = OomeeMakerScene::create();
    
    makerScene->setIsNewOomee(true);
    Director::getInstance()->replaceScene(makerScene);
	
}

bool OomeeSelectScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    addSideWiresToScreen(this, 0.2, 1.0f, 0.3f);
    
    _contentLayer = Layer::create();
    _contentLayer->setContentSize(Director::getInstance()->getVisibleSize());
    _contentLayer->setPosition(Director::getInstance()->getVisibleOrigin());
    this->addChild(_contentLayer);
   
    _oomeeCarousel = OomeeCarousel::create();
    _oomeeCarousel->setButtonDelegate(this);
    _oomeeCarousel->setContentSize(_contentLayer->getContentSize());
    _oomeeCarousel->setPosition(_contentLayer->getContentSize() / 2);
    _contentLayer->addChild(_oomeeCarousel);
    
    _exitButton = ui::Button::create();
    _exitButton->loadTextureNormal("res/oomeeMaker/close_button.png");
    _exitButton->setAnchorPoint(Vec2(-0.25,1.25));
    _exitButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    _exitButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Undo_Exit_Buttons.mp3");
            if(delegate)
            {
                delegate->onOomeeMakerNavigationBack();
            }
        }
    });
    _contentLayer->addChild(_exitButton);
    
    _newOomeeButton = ui::Button::create();
    _newOomeeButton->loadTextureNormal("res/oomeeMaker/new_oomee_button.png");
    _newOomeeButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _newOomeeButton->setNormalizedPosition(Vec2(0.5,0.15));
    _newOomeeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Edit_Button.wav");
            newOomee();
        }
    });
    _contentLayer->addChild(_newOomeeButton);
    
    ui::Button* leftArrow = ui::Button::create("res/oomeeMaker/arrow_button.png");
    leftArrow->setRotation(180);
    leftArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    leftArrow->setNormalizedPosition(Vec2(0.1,0.5));
    leftArrow->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            _oomeeCarousel->moveCarouselLeft();
        }
    });
    _contentLayer->addChild(leftArrow);
    
    ui::Button* rightArrow = ui::Button::create("res/oomeeMaker/arrow_button.png");
    rightArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rightArrow->setNormalizedPosition(Vec2(0.9,0.5));
    rightArrow->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            _oomeeCarousel->moveCarouselRight();
        }
    });
    _contentLayer->addChild(rightArrow);
    
    return true;
}

void OomeeSelectScene::onEnter()
{
	TutorialController::getInstance()->registerDelegate(this);
	if(TutorialController::getInstance()->isTutorialActive())
	{
		onTutorialStateChanged(TutorialController::getInstance()->getCurrentState());
	}
	
	OomeeMakerDataHandler::getInstance()->getLatestData([this](bool success){
		const auto& getOomeesCallback = [this](bool success){
			this->setCarouselData();
			if(TutorialController::getInstance()->isTutorialActive() && TutorialController::getInstance()->getCurrentState() == TutorialController::kConfirmOomee)
			{
				OomeeSelectScene::newOomee();
			}
			else if(delegate->_newAccessoryId != "")
			{
				if(_oomeeCarousel->getOomeeData().size() > 0)
				{
					editOomee(_oomeeCarousel->getOomeeData().at(0));
				}
				else
				{
					OomeeSelectScene::newOomee();
				}
			}
		};

        OomeeMakerDataHandler::getInstance()->getOomeesForChild(ChildManager::getInstance()->getLoggedInChild()->getId(), false, getOomeesCallback);

	});
	Super::onEnter();
}

void OomeeSelectScene::onEnterTransitionDidFinish()
{
    Super::onEnterTransitionDidFinish();
}

void OomeeSelectScene::onExit()
{
	TutorialController::getInstance()->unRegisterDelegate(this);
	Super::onExit();
}

void OomeeSelectScene::setCarouselData()
{
    const std::string& fileExtention = ".png";
    const std::vector<std::string>& createdOomeeFiles = DirUtil::getFilesInDirectoryWithExtention(OomeeMakerDataHandler::getInstance()->getFullSaveDir(), fileExtention);
    AnalyticsSingleton::getInstance()->reportNumberOfOomees((int)createdOomeeFiles.size());
    std::vector<std::string> trimmedFilenames;
    for(std::string filename : createdOomeeFiles)
    {
        trimmedFilenames.push_back(filename.substr(0,filename.length() - fileExtention.size()));
    }
    _oomeeCarousel->setVisibleRange(3);
    _oomeeCarousel->setOomeeData(trimmedFilenames);
}

void OomeeSelectScene::toggleMakeAvatarHiglight()
{
    auto centerButton = _oomeeCarousel->getCenterButton();
    if(centerButton)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Avatar_Messagedrop.mp3");
        this->runAction(Sequence::create(
        CallFunc::create([=](){
            centerButton->enableHighlight(true);
        }), DelayTime::create(4.0f),
        CallFunc::create([=](){
            centerButton->enableHighlight(false);
        }), NULL));
        
        auto banner = ui::Scale9Sprite::create("res/oomeeMaker/popup_window.png");
        banner->setAnchorPoint(Vec2(0.0,1.25));
        banner->setPosition(Vec2(_contentLayer->getContentSize().width, _contentLayer->getContentSize().height));
        banner->setContentSize(Size(_contentLayer->getContentSize().width * 0.315 , 166));
        banner->setColor(Style::Color::white);
        banner->runAction(Sequence::create(MoveBy::create(0.5, Vec2(-banner->getContentSize().width,0)), DelayTime::create(3.0f),MoveBy::create(0.5, Vec2(banner->getContentSize().width,0)),CallFunc::create([=](){
            banner->removeFromParent();
        }),NULL));
        _contentLayer->addChild(banner,10);
        
        auto bannerLabel = Label::createWithTTF(_("This is your new Oomee!"), Style::Font::Regular(), 73);
        bannerLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bannerLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        bannerLabel->setColor(Color3B::BLACK);
		bannerLabel->setHorizontalAlignment(TextHAlignment::CENTER);
		bannerLabel->setVerticalAlignment(TextVAlignment::CENTER);
		bannerLabel->setOverflow(Label::Overflow::SHRINK);
		bannerLabel->setDimensions(banner->getContentSize().width * 0.9f, banner->getContentSize().height * 0.7f);
        banner->addChild(bannerLabel);
        
    }
}

void OomeeSelectScene::setCarouselCenterTarget(const std::string &oomeeFilename)
{
    _oomeeCarousel->centerOnOomee(oomeeFilename);
}

// delegate functions

void OomeeSelectScene::editOomee(const std::string& oomeeFileName)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Edit_Button.wav");
    AnalyticsSingleton::getInstance()->editOomee();
    OomeeMakerScene* makerScene = OomeeMakerScene::create();
    makerScene->setFilename(oomeeFileName);
    Director::getInstance()->replaceScene(makerScene);
}

void OomeeSelectScene::deleteOomee(const std::string &oomeeFilename)
{
    ConfirmCancelMessageBox* messagebox = ConfirmCancelMessageBox::createWithParams(_("Delete?"), "res/buttons/confirm_bin.png", "res/buttons/confirm_x_2.png");
    messagebox->setDelegate(this);
    messagebox->setName(oomeeFilename);
    _contentLayer->addChild(messagebox);
}

void OomeeSelectScene::makeAvatar(const std::string &oomeeFilename)
{
    if(Azoomee::OomeeMaker::delegate)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Make_Avatar_Button.mp3");
        rapidjson::Document data;
        data.Parse(FileUtils::getInstance()->getStringFromFile(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + oomeeFilename + OomeeMakerDataHandler::kOomeeFileExtension).c_str());
        if(data.HasParseError())
        {
            return;
        }
        OomeeFigureDataRef oomee = OomeeFigureData::createWithData(data);
        OomeeMakerDataHandler::getInstance()->saveOomee(oomee, true, ChildManager::getInstance()->getLoggedInChild()->getId(), [this, oomeeFilename](bool success){
            if(delegate && success)
            {
                delegate->onOomeeMakerUpdateAvatar(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + oomeeFilename + ".png");
            }
        });
    }
}

void OomeeSelectScene::shareOomee(const std::string &oomeeFilename)
{
    if(delegate)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Share_Button.mp3");
        delegate->onOomeeMakerShareOomee(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + oomeeFilename + ".png");
    }
}

void OomeeSelectScene::onConfirmPressed(Azoomee::ConfirmCancelMessageBox *pSender)
{
	OomeeFigureDataRef oomee = OomeeFigureData::create();
	oomee->setId(pSender->getName());
	OomeeMakerDataHandler::getInstance()->deleteOomee(oomee, ChildManager::getInstance()->getLoggedInChild()->getId(), [this](bool success){
		if(success)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Undo_Exit_Buttons.mp3");
			AnalyticsSingleton::getInstance()->deleteOomee();
			stopAllActions();
			setCarouselData();
			this->getScheduler()->schedule([this](float deltaT){
				_oomeeCarousel->centerButtons();
			}, this, 0, 0, 1.0, 0, "centerButtons");
		}
	});
	
    pSender->removeFromParent();
}

void OomeeSelectScene::onCancelPressed(Azoomee::ConfirmCancelMessageBox *pSender)
{
    pSender->removeFromParent();
}

void OomeeSelectScene::onTutorialStateChanged(const std::string &stateId)
{
	if(stateId == TutorialController::kCreateOomee)
	{
		_exitButton->setVisible(false);
	}
	else
	{
		_exitButton->setVisible(true);
	}
}

NS_AZOOMEE_OM_END
