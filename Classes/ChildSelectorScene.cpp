#include "ChildSelectorScene.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "LoginController.h"
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include "SceneManagerScene.h"
#include "FlowDataSingleton.h"
#include "ContentHistoryManager.h"
#include "ForceUpdateAppLockScene.h"
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/Utils/ActionBuilder.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/UI/Style.h>
#include "HQHistoryManager.h"
#include "PopupMessageBox.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const Size ChildSelectorScene::kBaseButtonSize = Size(560,710);

bool ChildSelectorScene::init()
{
    if ( !Super::init() )
    {
        return false;
    }
    
    AnalyticsSingleton::getInstance()->logoutChildEvent();
    AnalyticsSingleton::getInstance()->setIsUserAnonymous(false);
    UserAccountManager::getInstance()->logoutChild();
    ContentHistoryManager::getInstance()->setReturnedFromContent(false);
    HQHistoryManager::getInstance()->clearCachedHQData();
    
    AudioMixer::getInstance()->stopBackgroundMusic();
    
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
    
    _titleText = DynamicText::create(_("Tap to access"), Style::Font::PoppinsBold(), 105);
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _titleText->setNormalizedPosition(Vec2(0.5f, 0.52f));
    _titleText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _titleText->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    _titleText->setTextColor(Color4B::WHITE);
    _titleText->enableShadow(Color4B(0,0,0,125), Size(4,-8));
    _titleLayout->addChild(_titleText);
    
    _settingsButton = TextButton::create("res/onboarding/rounded_button.png");
    _settingsButton->ignoreContentAdaptWithSize(false);
    _settingsButton->setScale9Enabled(true);
    _settingsButton->setContentSize(Size(700,140));
    _settingsButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _settingsButton->setNormalizedPosition(Vec2(0.5f, 0.48f));
    _settingsButton->setColor(Style::Color::white);
    _settingsButton->setTextColour(Color4B::BLACK);
    _settingsButton->setTextFontInfo(Style::Font::PoppinsBold(), 66);
    _settingsButton->setTextAreaSizePercent(Vec2(0.9f,0.8f));
    _settingsButton->setText(_("Account settings"));
    _settingsButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
           Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::SettingsFromChildSelect));
        }
    });
    _titleLayout->addChild(_settingsButton);
    
    _childLayout = ui::Layout::create();
    _childLayout->setSizeType(ui::Layout::SizeType::PERCENT);
    _childLayout->setSizePercent(Vec2(0.5f, 1.0f));
    _childLayout->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _childLayout->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
    _childLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _childLayout->setBackGroundColor(Color3B::WHITE);
    addChild(_childLayout);
    
    _switchProfileText = DynamicText::create(_("Switch profile"), Style::Font::PoppinsBold(), 66);
    _switchProfileText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _switchProfileText->setNormalizedPosition(Vec2(0.5f, 0.92f));
    _switchProfileText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _switchProfileText->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    _switchProfileText->setTextColor(Color4B(Style::Color::brownGrey));
    _childLayout->addChild(_switchProfileText);
    
    _childPageView = ui::PageView::create();
    _childPageView->setDirection(ui::PageView::Direction::HORIZONTAL);
    _childPageView->setIndicatorEnabled(true);
    _childPageView->setIndicatorPositionAsAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _childPageView->setIndicatorIndexNodesColor(Style::Color::brownGrey);
    _childPageView->setIndicatorIndexNodesOpacity(125);
    _childPageView->setIndicatorSelectedIndexColor(Style::Color::brownGrey);
    _childPageView->setIndicatorSelectedIndexOpacity(255);
    _childPageView->setIndicatorSpaceBetweenIndexNodes(20);
    _childPageView->setSizeType(ui::Layout::SizeType::PERCENT);
    _childPageView->setSizePercent(Vec2(1.0f,0.85f));
    _childPageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _childPageView->setNormalizedPosition(Vec2(0.5f, 0.9));
    _childPageView->setItemsMargin(80);
    _childPageView->setBounceEnabled(true);
    _childLayout->addChild(_childPageView);
    
    createChildButtons();
    
    return true;
}

void ChildSelectorScene::onEnter()
{
    OfflineChecker::getInstance()->setDelegate(this);
	
	ForceUpdateSingleton::getInstance()->setDelegate(this);
	ForceUpdateSingleton::getInstance()->doForceUpdateLogic();
	
    if(FlowDataSingleton::getInstance()->hasError())
    {
        const long errorCode = FlowDataSingleton::getInstance()->getErrorCode();
        const auto& errorMessageText = LocaleManager::getInstance()->getErrorMessageWithCode(errorCode);
        
        PopupMessageBox* messageBox = PopupMessageBox::create();
        messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
        messageBox->setBody(errorMessageText.at(ERROR_BODY));
        messageBox->setButtonText(_("Back"));
        messageBox->setButtonColour(Style::Color::darkIndigo);
        messageBox->setPatternColour(Style::Color::azure);
        messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
            pSender->removeFromParent();
        });
        this->addChild(messageBox, 1);
    }
    
    if(UserAccountManager::getInstance()->getAmountOfAvailableChildren() == 0)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::AddChild));
    }
    
    Super::onEnter();
}

void ChildSelectorScene::onExit()
{
    OfflineChecker::getInstance()->setDelegate(nullptr);
    ForceUpdateSingleton::getInstance()->setDelegate(nullptr);

    Super::onExit();
}

void ChildSelectorScene::onSizeChanged()
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
    
    _childLayout->setSizePercent(isPortrait ? Vec2(1.0f, 1.0f - _titleLayout->getSizePercent().y) : Vec2(1.0f - _titleLayout->getSizePercent().x, 1.0f));
    
    _childPageView->scrollToPage(_childPageView->getCurrentPageIndex());
}

void ChildSelectorScene::createChildButtons()
{
    const int numChildren = UserAccountManager::getInstance()->getAmountOfAvailableChildren();
    
    const int rowsPerPage = 2;
    const int colsPerPage = 2;
    const int pages = ceil(numChildren / (float)(rowsPerPage * colsPerPage));
    
    const float buttonPadding = 80.0f;
    
    const Size& pageSize = Size((kBaseButtonSize.width + buttonPadding) * colsPerPage, (kBaseButtonSize.height + buttonPadding) * rowsPerPage);
    const Size& rowSize = Size((kBaseButtonSize.width + buttonPadding) * colsPerPage , kBaseButtonSize.height);
    
    int childNum = 0;
    for(int pageNum = 0; pageNum < pages; pageNum++)
    {
        ui::Layout* page = ui::Layout::create();
        page->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        page->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
        page->setLayoutType(ui::Layout::Type::VERTICAL);
        page->setContentSize(pageSize);
        _childPageView->addPage(page);
        
        for(int row = 0; row < rowsPerPage; row++)
        {
            ui::Layout* itemRow = ui::Layout::create();
            itemRow->setLayoutType(ui::Layout::Type::ABSOLUTE);
            itemRow->setContentSize(rowSize);
            itemRow->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,buttonPadding)));
            page->addChild(itemRow);
            
            for(int col = 0; col < colsPerPage; col++)
            {
                
                if(childNum < numChildren)
                {
                    const ChildRef& child = UserAccountManager::getInstance()->getChild(childNum);
                    if(child)
                    {
                        ui::Layout* button = createChildButton(child, childNum);
                        button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                        button->setNormalizedPosition(Vec2((col + 0.5) / colsPerPage, 0.5));
                        itemRow->addChild(button);
                    }
                    childNum++;
                }
            }
        }
    }
    
}

cocos2d::ui::Layout* ChildSelectorScene::createChildButton(const ChildRef& child, int childNum)
{
    ui::Layout* childButton = ui::Layout::create();
    childButton->setContentSize(kBaseButtonSize);
    
    RemoteImageSprite* oomee = RemoteImageSprite::create();
    oomee->initWithUrlAndSize(child->getAvatar(), Size(kBaseButtonSize.width, kBaseButtonSize.width));
    oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    oomee->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    childButton->addChild(oomee);
    
    ui::ImageView* shadow = ui::ImageView::create("res/childSelection/shadow.png");
    shadow->ignoreContentAdaptWithSize(false);
    shadow->setScale((kBaseButtonSize.width * 0.8f) / shadow->getContentSize().width);
    shadow->setAnchorPoint(Vec2(0.5f, 0.4f));
    shadow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    oomee->addChild(shadow, -1);
    
    DynamicText* text = DynamicText::create(child->getProfileName(), Style::Font::PoppinsRegular(), 50);
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    text->setTextColor(Color4B(Style::Color::brownGrey));
    childButton->addChild(text);
    
    childButton->setTouchEnabled(true);
    childButton->addTouchEventListener([childNum](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            AudioMixer::getInstance()->playEffect(SELECT_OOMEE_AUDIO_EFFECT);
            AnalyticsSingleton::getInstance()->registerChildGenderAndAge(UserAccountManager::getInstance()->getChild(childNum));
            LoginController::getInstance()->childLogin(UserAccountManager::getInstance()->getChild(childNum)->getProfileName());
        }
    });
    
    return childButton;

}

//----------------------- Delegate Functions ----------------------------

void ChildSelectorScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{

}

void ChildSelectorScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    //do nothing
}

void ChildSelectorScene::connectivityStateChanged(bool online)
{
    if(!online)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineHub));
    }
}

void ChildSelectorScene::onForceUpdateCheckFinished(const ForceUpdateResult& result)
{
	switch (result) {
		case ForceUpdateResult::DO_NOTHING:
		{
			break;
		}
		case ForceUpdateResult::NOTIFY:
		{
            PopupMessageBox* messageBox = PopupMessageBox::create();
            messageBox->setTitle(_("Update recommended"));
            messageBox->setBody(_("You should update to the latest version of Azoomee. Ask a grown-up to help you."));
            messageBox->setPatternColour(Style::Color::azure);
            
            messageBox->setButtonText(_("Update"));
            messageBox->setButtonColour(Style::Color::strongPink);
            messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
                pSender->removeFromParent();
                Application::getInstance()->openURL(ForceUpdateSingleton::getInstance()->getUpdateUrlFromFile());
            });
            
            messageBox->setSecondButtonText(_("Back"));
            messageBox->setSecondButtonColour(Style::Color::darkIndigo);
            messageBox->setSecondButtonPressedCallback([this](PopupMessageBox* pSender){
                pSender->removeFromParent();
            });
            this->addChild(messageBox, 1);
			break;
		}
		case ForceUpdateResult::LOCK:
		{
			Director::getInstance()->replaceScene(ForceUpdateAppLockScene::create());
		}
		default:
			break;
	}
}
NS_AZOOMEE_END
