//
//  LanguageSelectScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 01/11/2018.
//

#include "LanguageSelectScene.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include "SceneManagerScene.h"
#include "BackEndCaller.h"
#include "LoginLogicHandler.h"
#include "HQHistoryManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string LanguageSelectScene::kGreekLangID = "gre";
const cocos2d::Size LanguageSelectScene::kBaseButtonSize = Size(255,410);

bool LanguageSelectScene::init()
{
	if(!Super::init())
	{
		return false;
	}
    
    const Color3B& bgColour = Style::Color::darkIndigo;
    
    _titleLayout = ui::Layout::create();
    _titleLayout->setSizeType(ui::Layout::SizeType::PERCENT);
    _titleLayout->setSizePercent(Vec2(0.5f, 1.0f));
    _titleLayout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _titleLayout->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    _titleLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _titleLayout->setBackGroundColor(bgColour);
    addChild(_titleLayout);
    
    _bgPattern = RoundedRectSprite::create();
    _bgPattern->setTexture("res/decoration/pattern_stem_tile.png");
    _bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setColor(Style::Color::macaroniAndCheese);
    _bgPattern->setRoundedCorners(false, false, false, false);
    _bgPattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    _bgPattern->setTileScaleFactor(1.0f);
    _titleLayout->addChild(_bgPattern);
    
    _gradient = LayerGradient::create();
    _gradient->setStartColor(bgColour);
    _gradient->setStartOpacity(255);
    _gradient->setEndColor(bgColour);
    _gradient->setEndOpacity(0);
    _gradient->setVector(Vec2(0,1));
    _gradient->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
    _gradient->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _titleLayout->addChild(_gradient);
    
    _titleText = DynamicText::create(_("Pick your language"), Style::Font::PoppinsBold(), 105);
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _titleText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _titleText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _titleText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _titleText->setTextColor(Color4B::WHITE);
    _titleText->enableShadow(Color4B(0,0,0,125), Size(4,-8));
    _titleLayout->addChild(_titleText);
	
    _languageLayout = ui::Layout::create();
    _languageLayout->setSizeType(ui::Layout::SizeType::PERCENT);
    _languageLayout->setSizePercent(Vec2(0.5f, 1.0f));
    _languageLayout->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _languageLayout->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
    _languageLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _languageLayout->setBackGroundColor(Color3B::WHITE);
    addChild(_languageLayout);
    
    _languageScrollView = ui::ListView::create();
    _languageScrollView->setSizeType(ui::Layout::SizeType::PERCENT);
    _languageScrollView->setSizePercent(Vec2(1.0f,0.86f));
    _languageScrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _languageScrollView->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _languageScrollView->setItemsMargin(150);
    _languageScrollView->setBounceEnabled(true);
    _languageScrollView->setTopPadding(100);
    _languageScrollView->setBottomPadding(100);
    _languageLayout->addChild(_languageScrollView);
    
    createLanguageButtons();
    
	return true;
}

void LanguageSelectScene::onEnter()
{
	Super::onEnter();
}

void LanguageSelectScene::onExit()
{
	Super::onExit();
}

void LanguageSelectScene::onSizeChanged()
{
	Super::onSizeChanged();
	
    const Size& contentSize = getContentSize();
    bool isPortrait = contentSize.width < contentSize.height;
    
    _titleLayout->setSizePercent(isPortrait ? Vec2(1.0f, 0.29f) : Vec2(0.5f, 1.0f));
    const Size& titleSize = Size(contentSize.width * _titleLayout->getSizePercent().x, contentSize.height * _titleLayout->getSizePercent().y);
    _bgPattern->setContentSize(titleSize);
    _gradient->setContentSize(titleSize);
    _gradient->setStartOpacity(isPortrait ? 150 : 255);
    _titleText->setMaxLineWidth(titleSize.width * 0.7f);
	
    _languageLayout->setSizePercent(isPortrait ? Vec2(1.0f, 1.0f - _titleLayout->getSizePercent().y) : Vec2(1.0f - _titleLayout->getSizePercent().x, 1.0f));
}

void LanguageSelectScene::createLanguageButtons()
{
    const auto& langsData = StringMgr::kLanguageParams;
    
	int langsPerRow = 3;
	
	int i = 0;
	while(i < langsData.size())
	{
		ui::Layout* itemRow = ui::Layout::create();
		itemRow->setLayoutType(ui::Layout::Type::ABSOLUTE);
		itemRow->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
		itemRow->setContentSize(Size(langsPerRow * kBaseButtonSize.width + 450 , kBaseButtonSize.height));
        
		for(int column = 0; column < langsPerRow; column++)
		{
			if(i < langsData.size())
			{
				ui::Layout* button = createLanguageButton(langsData.at(i));
				button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				button->setNormalizedPosition(Vec2((column + 0.5) / langsPerRow, 0.5));
				itemRow->addChild(button);
				i++;
			}
		}
		_languageScrollView->pushBackCustomItem(itemRow);
	}
}

cocos2d::ui::Layout* LanguageSelectScene::createLanguageButton(const LanguageParams& params)
{
    ui::Layout* langButton = ui::Layout::create();
    langButton->setContentSize(kBaseButtonSize);
    
    ui::ImageView* flag = ui::ImageView::create(StringUtils::format("res/settings/flag_%s.png",params._identifier.c_str()));
    flag->ignoreContentAdaptWithSize(false);
    flag->setContentSize(Size(kBaseButtonSize.width, kBaseButtonSize.width));
    flag->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    flag->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    langButton->addChild(flag);
    
    ui::ImageView* shadow = ui::ImageView::create("res/languageSelect/shadow.png");
    shadow->ignoreContentAdaptWithSize(false);
    shadow->setScale((kBaseButtonSize.width * 1.2f) / shadow->getContentSize().width);
    shadow->setAnchorPoint(Vec2(0.5f, 0.6f));
    shadow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    flag->addChild(shadow, -1);
    
    DynamicText* text = DynamicText::create(params._name, params._identifier == kGreekLangID ? Style::Font::ArialRegular : Style::Font::poppinsRegular, 50);
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    text->setTextColor(Color4B(Style::Color::brownGrey));
    langButton->addChild(text);
    
    langButton->setTouchEnabled(true);
    langButton->addTouchEventListener([params](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            StringMgr::getInstance()->changeLanguage(params._identifier);
            HQHistoryManager::getInstance()->clearCachedHQ();
            if(!ParentManager::getInstance()->hasParentLoginDataInUserDefaults())
            {
                if(ParentManager::getInstance()->getParent())
                {
                    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
                }
                else
                {
                    BackEndCaller::getInstance()->anonymousDeviceLogin();
                }
            }
            else
            {
                if(ParentManager::getInstance()->getParent())
                {
                    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
                }
                else
                {
                    LoginLogicHandler::getInstance()->doLoginLogic();
                }
            }
        }
    });
    
    return langButton;

	
}

NS_AZOOMEE_END
