#include "ChildSelectorScene.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Cookie/CookieManager.h>
#include <math.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include "SettingsButton.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "LoginLogicHandler.h"
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include "SceneManagerScene.h"
#include "FlowDataSingleton.h"
#include "ChatNotificationsSingleton.h"
#include <AzoomeeCommon/UI/PrivacyLayer.h>
#include "ContentHistoryManager.h"
#include "ForceUpdateAppLockScene.h"
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/Utils/ActionBuilder.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "HQHistoryManager.h"

#define OOMEE_LAYER_WIDTH 450
#define OOMEE_LAYER_HEIGHT 450
#define OOMEE_LAYER_GAP 100
#define OOMEE_LAYER_GAP_PORTRAIT 150

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string ChildSelectorScene::kSceneName = "ChildSelectScene";
const std::string ChildSelectorScene::kOomeeLayerName = "oomee";

Scene* ChildSelectorScene::createScene()
{
    auto scene = ChildSelectorScene::create();
    scene->setName(kSceneName);
    
    return scene;
}

bool ChildSelectorScene::init()
{
    if ( !Super::init() )
    {
        return false;
    }
    
    AnalyticsSingleton::getInstance()->logoutChildEvent();
    AnalyticsSingleton::getInstance()->setIsUserAnonymous(false);
    
    ContentHistoryManager::getInstance()->setReturnedFromContent(false);
    HQHistoryManager::getInstance()->clearCachedHQ();
    
    AudioMixer::getInstance()->stopBackgroundMusic();
    ChatNotificationsSingleton::getInstance()->stopNotificationsUpdate();
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    
    _isPortrait = _visibleSize.width < _visibleSize.height;
    
    _contentNode = Node::create();
    _contentNode->setContentSize(_visibleSize);
    
    this->addChild(_contentNode, -1, ConfigStorage::kContentNodeName);
    
    _firstTime = true;
    
    addVisualsToScene();
    addScrollViewForProfiles();
    addProfilesToScrollView();
    addPrivacyButton();
    
    _firstTime = false;

    return true;
}

void ChildSelectorScene::onEnterTransitionDidFinish()
{
    OfflineChecker::getInstance()->setDelegate(this);
	
	ForceUpdateSingleton::getInstance()->setDelegate(this);
	ForceUpdateSingleton::getInstance()->doForceUpdateLogic();
	
    if(FlowDataSingleton::getInstance()->hasError())
    {
        MessageBox::createWith(FlowDataSingleton::getInstance()->getErrorCode(), nullptr);
    }
    
    if(ParentManager::getInstance()->isBillingDataAvailable())
    {
        if(ParentManager::getInstance()->isPaidUser())
        {
            setParentButtonVisible(true);
        }
    }
    
    if(ParentManager::getInstance()->getAmountOfAvailableChildren() == 0)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::AddChildFirstTime));
    }
    Super::onEnterTransitionDidFinish();
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------

void ChildSelectorScene::addVisualsToScene()
{
    addBackgroundToScreen();
	
	const bool isIphoneX = ConfigStorage::getInstance()->isDeviceIphoneX();
	
	ui::Layout* titleLayout = ui::Layout::create();
	titleLayout->setContentSize(Size(_visibleSize.width, (isIphoneX && _isPortrait) ? 250 : 150));
	titleLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	titleLayout->setBackGroundColor(Style::Color::skyBlue);
	titleLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
	titleLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_contentNode->addChild(titleLayout);
	
	ui::Text* titleText = ui::Text::create(_("Switch Profile"), Style::Font::Medium(), 91);
	titleText->setNormalizedPosition(isIphoneX ? Vec2(0.5f,0.25f) : Vec2::ANCHOR_MIDDLE);
	titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	titleText->setTextColor(Color4B::WHITE);
	titleLayout->addChild(titleText);
	
	if(_isPortrait)
	{
		auto selectTitle = Label::createWithTTF(_("Select your profile"), Style::Font::Regular(), 65);
		selectTitle->setHorizontalAlignment(TextHAlignment::CENTER);
		selectTitle->setTextColor(Color4B::BLACK);
		selectTitle->setPosition(_visibleSize.width / 2, _visibleSize.height - titleLayout->getContentSize().height * (isIphoneX ? 1.5f : 2.0f));
		selectTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		_contentNode->addChild(selectTitle);
	}
}

void ChildSelectorScene::addBackgroundToScreen()
{
	LayerColor* bgColour = LayerColor::create(Color4B::WHITE);
	this->addChild(bgColour, -2);
	
    auto wireLeft = Sprite::create(StringUtils::format("res/childSelection/wireLeft%s.png", _isPortrait ? "_portrait" : ""));
    wireLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    wireLeft->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    wireLeft->setScale(_visibleSize.height / wireLeft->getContentSize().height);
	wireLeft->setColor(Color3B::BLACK);
	wireLeft->setOpacity(125);
    _contentNode->addChild(wireLeft, -2);
    
    auto wireRight = Sprite::create(StringUtils::format("res/childSelection/wireRight%s.png", _isPortrait ? "_portrait" : ""));
    wireRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    wireRight->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    wireRight->setScale(_visibleSize.height / wireRight->getContentSize().height);
	wireRight->setColor(Color3B::BLACK);
	wireRight->setOpacity(125);
    _contentNode->addChild(wireRight, -2);
}

void ChildSelectorScene::createSettingsButton()
{
    auto settingsButton = SettingsButton::create();
	settingsButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	settingsButton->setAnchorPoint(Vec2(-0.25,1.25));
    _contentNode->addChild(settingsButton);
}

void ChildSelectorScene::addPrivacyButton()
{
    PrivacyLayer* privacyLayer = PrivacyLayer::createWithColor();
    privacyLayer->setCenterPosition(Vec2(_visibleSize.width / 2,privacyLayer->getContentSize().height * 2.0f));
    _contentNode->addChild(privacyLayer);
}

void ChildSelectorScene::addScrollViewForProfiles()
{
    _scrollView = ui::ScrollView::create();
	_scrollView->setContentSize(Size(_visibleSize.width * 0.8, _visibleSize.height * (_isPortrait ? 0.6 : (ConfigStorage::getInstance()->isDevice18x9() ? 0.5 : 0.6))));
	_scrollView->setPosition(Point(_visibleSize.width * 0.1,_visibleSize.height * (_isPortrait ? 0.2 : (ConfigStorage::getInstance()->isDevice18x9() ? 0.3 :  0.25))));
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    _scrollView->setBounceEnabled(true);
    _scrollView->setTouchEnabled(true);
    _scrollView->setInnerContainerSize(getScrollviewInnerSize(_scrollView->getContentSize().width));
    _scrollView->setSwallowTouches(false);
    _scrollView->setScrollBarEnabled(true);
    
    _contentNode->addChild(_scrollView);
    
    Sprite* verticalScrollGradientTop = Sprite::create("res/decoration/TopNavGrad.png");
    verticalScrollGradientTop->setAnchorPoint(Vec2(0.5, 1.0));
    verticalScrollGradientTop->setScaleX(_visibleSize.width * 0.75 / verticalScrollGradientTop->getContentSize().width);
    verticalScrollGradientTop->setColor(Color3B::WHITE);
    verticalScrollGradientTop->setPosition(Vec2(_visibleSize.width / 2, _scrollView->getPosition().y + _scrollView->getContentSize().height));
    _contentNode->addChild(verticalScrollGradientTop,1);
    
    Sprite* verticalScrollGradientBottom = Sprite::create("res/decoration/TopNavGrad.png");
    verticalScrollGradientBottom->setAnchorPoint(Vec2(0.5, 1.0));
    verticalScrollGradientBottom->setScaleX(_visibleSize.width * 0.75 / verticalScrollGradientBottom->getContentSize().width);
    verticalScrollGradientBottom->setColor(Color3B::WHITE);
    verticalScrollGradientBottom->setRotation(180);
    verticalScrollGradientBottom->setPosition(Vec2(_visibleSize.width / 2, _scrollView->getPosition().y));
    _contentNode->addChild(verticalScrollGradientBottom,1);
}

Size ChildSelectorScene::getScrollviewInnerSize(float scrollviewWidth)
{
    int childrenPerRow = _isPortrait ? 2 : 4;
    int numChildren = ParentManager::getInstance()->getAmountOfAvailableChildren();
    
    int numRows = ceil(numChildren / (double)childrenPerRow);
    
    return Size(OOMEE_LAYER_WIDTH * childrenPerRow + OOMEE_LAYER_GAP * (childrenPerRow - 1), (OOMEE_LAYER_HEIGHT * numRows) + ((_isPortrait ? OOMEE_LAYER_GAP_PORTRAIT : OOMEE_LAYER_GAP) * (numRows - 0.5)));
}

void ChildSelectorScene::addProfilesToScrollView()
{
    AnalyticsSingleton::getInstance()->registerNoOfChildren(ParentManager::getInstance()->getAmountOfAvailableChildren());
    
    for(int i = 0; i < ParentManager::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        auto profileLayer = createChildProfileButton(ParentManager::getInstance()->getChild(i)->getProfileName(), i);
        profileLayer->setTag(i);
        profileLayer->setPosition(positionElementOnScrollView(profileLayer));
        _scrollView->addChild(profileLayer);
    }
    
    _parentButton = createParentProfileButton();
	if(ParentManager::getInstance()->isLoggedInParentAnonymous())
	{
    	setParentButtonVisible(false);
	}
	_parentButton->setPosition(Vec2(_visibleSize.width * 0.5f, _parentButton->getContentSize().height * (_isPortrait ? 1.7f : 1.2)));
    _contentNode->addChild(_parentButton);
    
}

ui::Button *ChildSelectorScene::createChildProfileButton(const std::string& profileName, int childNum)
{
	const Size& oomeeSize = Size(370,370);
	
    auto button = ui::Button::create();
    button->setContentSize(Size(OOMEE_LAYER_WIDTH,OOMEE_LAYER_HEIGHT));
    button->ignoreContentAdaptWithSize(false);
    button->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    button->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::BEGAN)
        {
            auto button = dynamic_cast<Node*>(pSender);
            if(button)
            {
                auto oomeeLayer = button->getChildByName(kOomeeLayerName);
                if(oomeeLayer)
                {
                    oomeeLayer->setScale(1.25f);
                }
            }
        }
        else if(eType == ui::Widget::TouchEventType::ENDED)
        {
            AudioMixer::getInstance()->playEffect(SELECT_OOMEE_AUDIO_EFFECT);
            _parentIconSelected = false;
            int childNumber = ((Node*)pSender)->getTag();
            AnalyticsSingleton::getInstance()->registerChildGenderAndAge(ParentManager::getInstance()->getChild(childNumber));
            BackEndCaller::getInstance()->childLogin(childNumber);
        }
        else if(eType == ui::Widget::TouchEventType::CANCELED)
        {
            auto button = dynamic_cast<Node*>(pSender);
            if(button)
            {
                auto oomeeLayer = button->getChildByName(kOomeeLayerName);
                if(oomeeLayer)
                {
                    oomeeLayer->setScale(1.0f);
                }
            }
        }
    });
	
	Sprite* bgCircle1 = Sprite::create("res/oomeeMaker/circle_0.png");
	bgCircle1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bgCircle1->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	bgCircle1->setScale(0);
	bgCircle1->setOpacity(25);
	bgCircle1->setRotation(RandomHelper::random_real(0.0,M_PI));
	bgCircle1->setColor(Style::Color::darkTeal);
	button->addChild(bgCircle1, -1);
	
	auto popIn1 = EaseBackOut::create(ScaleTo::create(0.5, ((oomeeSize.height * 0.85) / bgCircle1->getContentSize().height)));
	auto rotate1 = RepeatForever::create(RotateBy::create(30 + CCRANDOM_0_1() * 30, 360));
	
	bgCircle1->runAction(popIn1);
	bgCircle1->runAction(rotate1);
	
	Sprite* bgCircle2 = Sprite::create("res/oomeeMaker/circle_1.png");
	bgCircle2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bgCircle2->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	bgCircle2->setScale(0);
	bgCircle2->setOpacity(25);
	bgCircle2->setRotation(RandomHelper::random_real(0.0,M_PI));
	bgCircle2->setColor(Style::Color::darkTeal);
	button->addChild(bgCircle2, -1);
	
	auto popIn2 = EaseBackOut::create(ScaleTo::create(0.5, ((oomeeSize.height * 1.15f) / bgCircle2->getContentSize().height)));
	auto rotate2 = RepeatForever::create(RotateBy::create(30 +  CCRANDOM_0_1() * 30, -360));
	
	bgCircle2->runAction(popIn2);
	bgCircle2->runAction(rotate2);
	
    auto oomee = RemoteImageSprite::create();
    oomee->initWithUrlAndSizeWithoutPlaceholder(ParentManager::getInstance()->getChild(childNum)->getAvatar(), oomeeSize);
    oomee->setKeepAspectRatio(true);
    oomee->setAnchorPoint(Vec2(0.5,0.4));
    oomee->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    oomee->setName(kOomeeLayerName);
    button->addChild(oomee);
    
    float delayTime = CCRANDOM_0_1() * 0.5;
    if(_firstTime)
    {
        oomee->setOpacity(0);
        oomee->runAction(createBlinkEffect(delayTime, 0.1));
    }
	
    auto profileLabel = createLabelChildName(profileName);
    profileLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    profileLabel->setNormalizedPosition(Vec2(0.5,0));
	profileLabel->setTextColor(Color4B::BLACK);
    reduceLabelTextToFitWidth(profileLabel,OOMEE_LAYER_WIDTH);
    button->addChild(profileLabel);
    
    if(_firstTime)
    {
        profileLabel->setOpacity(0);
        profileLabel->runAction(createBlinkEffect(delayTime, 0.1));
    }

    return button;
}

Point ChildSelectorScene::positionElementOnScrollView(Node *layerToBeAdded)
{
    Vector<Node*> scrollViewChildren = _scrollView->getChildren();
    
    int numberOfChildren = ParentManager::getInstance()->getAmountOfAvailableChildren();
    if(numberOfChildren > _isPortrait ? 2 : 4)
    {
        numberOfChildren = _isPortrait ? 2 : 4;
    }
    
    float layerMultiplier = numberOfChildren * 0.5;
    float gapMultiplier = (numberOfChildren - 1) * 0.5;
    
    float startPosX = _scrollView->getInnerContainerSize().width / 2 - (gapMultiplier * OOMEE_LAYER_GAP) - (layerMultiplier * OOMEE_LAYER_WIDTH);
    
    if(scrollViewChildren.size() == 0)
    {
        return Point(startPosX, _scrollView->getInnerContainerSize().height - OOMEE_LAYER_GAP / 2 - layerToBeAdded->getContentSize().height);
    }
    
    Node *lastChild = scrollViewChildren.at(scrollViewChildren.size() - 1);
    Point lastPos = lastChild->getPosition();
    
    Point newPos = Point(lastPos.x + lastChild->getContentSize().width + OOMEE_LAYER_GAP, lastPos.y);
    
    if(newPos.x + layerToBeAdded->getContentSize().width > (startPosX + getScrollviewInnerSize(_scrollView->getContentSize().width).width)) // position relative to expected size
    {
        newPos = Point(startPosX, newPos.y - (_isPortrait ? OOMEE_LAYER_GAP_PORTRAIT : OOMEE_LAYER_GAP) / 2 - layerToBeAdded->getContentSize().height);
    }
    
    return newPos;
}

ui::Button* ChildSelectorScene::createParentProfileButton()
{
	Sprite* icon = Sprite::create("res/settings/parents_area_icon.png");
	icon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	icon->setAnchorPoint(Vec2(-0.125,0.5));
	
	auto parentButton = ui::Button::create("res/settings/rounded_button.png");
	
	float scale = (icon->getContentSize().height * 1.25) / parentButton->getContentSize().height;
	
	parentButton->setContentSize(parentButton->getContentSize() * scale);
	parentButton->ignoreContentAdaptWithSize(false);
	parentButton->setColor(Style::Color::skyBlue);
	parentButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
	parentButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	parentButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::SettingsFromChildSelect));
		}
	});
	
	parentButton->addChild(icon);
	
	Label* parentAreaText = Label::createWithTTF(_("Parents' Zone"), Style::Font::Regular(), 70);
	parentAreaText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	parentAreaText->setColor(Color3B::WHITE);
	parentAreaText->setHorizontalAlignment(TextHAlignment::CENTER);
	parentAreaText->setVerticalAlignment(TextVAlignment::CENTER);
	parentAreaText->setOverflow(Label::Overflow::SHRINK);
	parentAreaText->setDimensions(parentButton->getContentSize().width * 0.6f, parentButton->getContentSize().height * 0.6f);
	parentAreaText->setNormalizedPosition(Vec2(0.6,0.5));
	parentButton->addChild(parentAreaText);
	
    return parentButton;
}

void ChildSelectorScene::setParentButtonVisible(bool visible)
{
    if(_parentButton)
    {
        _parentButton->setVisible(visible);
    }
}

//----------------------- Delegate Functions ----------------------------
void ChildSelectorScene::AdultPinCancelled(RequestAdultPinLayer* layer)
{
    removeAdultPinLayerDelegate();
}

void ChildSelectorScene::AdultPinAccepted(RequestAdultPinLayer* layer)
{
    removeAdultPinLayerDelegate();
    
    if(_parentIconSelected)
    {
        refreshParentCookiesRequest();
        return;
    }
    
    callDelegateFunction(0);
}

void ChildSelectorScene::createAdultPinLayerWithDelegate()
{
    _awaitingAdultPinLayer = RequestAdultPinLayer::create();
    _awaitingAdultPinLayer->setDelegate(this);
}

void ChildSelectorScene::removeAdultPinLayerDelegate()
{
    if(_awaitingAdultPinLayer == nullptr)
    {
        return;
    }
    
    _awaitingAdultPinLayer->setDelegate(nullptr);
    _awaitingAdultPinLayer = nullptr;
}

void ChildSelectorScene::refreshParentCookiesRequest()
{
    HttpRequestCreator* request = API::RefreshParentCookiesRequest(this);
    request->execute();
}

void ChildSelectorScene::getParentCookiesRequest()
{
    const std::string& userId = ParentManager::getInstance()->getLoggedInParentId();
    const std::string& sessionId = ParentManager::getInstance()->getLoggedInParentCdnSessionId();
    
    HttpRequestCreator* request = API::GetGordenRequest(userId, sessionId, this);
    request->execute();
}

void ChildSelectorScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagCookieRefresh)
    {
        ParentManager::getInstance()->parseParentSessionData(body);
        getParentCookiesRequest();
        return;
    }
    
    if(CookieManager::getInstance()->parseDownloadCookies(headers))
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChatEntryPointScene));
    }
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

void ChildSelectorScene::callDelegateFunction(float dt)
{
    if(ParentManager::getInstance()->getAmountOfAvailableChildren() == 0)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::AddChildFirstTime));
    }
    else
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::AddChild));
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
			std::vector<std::string> buttonNames = {_("OK"), _("Update")};
			MessageBox::createWith(_("Update recommended"), _("You should update to the latest version of Azoomee. Ask a grown-up to help you."), buttonNames, this);
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

void ChildSelectorScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(messageBoxTitle == StringMgr::getInstance()->getErrorMessageWithCode(ERROR_CODE_EMAIL_VARIFICATION_REQUIRED)[ERROR_TITLE] && buttonTitle == StringMgr::getInstance()->getErrorMessageWithCode(ERROR_CODE_EMAIL_VARIFICATION_REQUIRED)[ERROR_BUTTON])
    {
        AnalyticsSingleton::getInstance()->logoutParentEvent();
        ParentManager::getInstance()->logoutChild();
        
        LoginLogicHandler::getInstance()->forceNewLogin();
    }
	if(messageBoxTitle == _("Update recommended"))
	{
		if(buttonTitle == _("Update"))
		{
			Application::getInstance()->openURL(ForceUpdateSingleton::getInstance()->getUpdateUrlFromFile());
		}
	}
	
}

void ChildSelectorScene::onExit()
{
    OfflineChecker::getInstance()->setDelegate(nullptr);
	ForceUpdateSingleton::getInstance()->setDelegate(nullptr);
    removeAdultPinLayerDelegate();

    Super::onExit();
}

void ChildSelectorScene::onSizeChanged()
{
    Super::onSizeChanged();
    _isPortrait = Director::getInstance()->getVisibleSize().width < Director::getInstance()->getVisibleSize().height;
    bool parentButtonVisible = false;
    if(_parentButton)
    {
        parentButtonVisible = _parentButton->isVisible();
    }
    if(_contentNode)
    {
        _contentNode->removeFromParent();
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    
    _contentNode = Node::create();
    _contentNode->setContentSize(_visibleSize);
    
    this->addChild(_contentNode, -1, ConfigStorage::kContentNodeName);
    
    addVisualsToScene();
    addScrollViewForProfiles();
    addProfilesToScrollView();
    addPrivacyButton();
    
    setParentButtonVisible(parentButtonVisible);
}

NS_AZOOMEE_END
