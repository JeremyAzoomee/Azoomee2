#include "ChildSelectorScene.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Cookie/CookieDataParser.h>
#include <math.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include "SettingsButton.h"
#include "OfflineHubScene.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "LoginLogicHandler.h"
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include "SceneManagerScene.h"
#include "FlowDataSingleton.h"
#include "ChatNotificationsSingleton.h"
#include <AzoomeeCommon/UI/PrivacyLayer.h>
#include "ContentHistoryManager.h"
#include "DynamicNodeHandler.h"
#include "ForceUpdateAppLockScene.h"
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/Utils/ActionBuilder.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>

#define OOMEE_LAYER_WIDTH 400
#define OOMEE_LAYER_HEIGHT 400
#define OOMEE_LAYER_GAP 100
#define OOMEE_LAYER_GAP_PORTRAIT 100

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
    
    AudioMixer::getInstance()->stopBackgroundMusic();
    ChatNotificationsSingleton::getInstance()->stopNotificationsUpdate();
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    
    _isPortrait = _visibleSize.width < _visibleSize.height;
    
    _contentNode = Node::create();
    _contentNode->setContentSize(_visibleSize);
    
    this->addChild(_contentNode, -1, ConfigStorage::kContentNodeName);
    
    _firstTime = true;
    
    addVisualsToScene();
    createSettingsButton();
    addScrollViewForProfiles();
    addProfilesToScrollView();
    addPrivacyButton();
    
    _firstTime = false;
    
    _addChildButton= createNewProfileButton();
    _addChildButton->setPosition(Vec2(_visibleSize.width * 0.5, _addChildButton->getContentSize().height * (_isPortrait ? 3.0f : 1.5f)));
    _contentNode->addChild(_addChildButton);

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
    
    if(ParentDataProvider::getInstance()->isBillingDataAvailable())
    {
        if(ParentDataProvider::getInstance()->isPaidUser())
        {
            setParentButtonVisible(true);
        }
    }
    
    if(ParentDataProvider::getInstance()->getAmountOfAvailableChildren() == 0)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(AddChildFirstTime));
    }
    Super::onEnterTransitionDidFinish();
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------

void ChildSelectorScene::addVisualsToScene()
{
    addBackgroundToScreen();
    
    auto selectTitle = createLabelHeader(_("Select your profile"));
    selectTitle->setNormalizedPosition(Vec2(0.5,0.9));
    _contentNode->addChild(selectTitle);
}

void ChildSelectorScene::addBackgroundToScreen()
{
    auto wireLeft = Sprite::create(StringUtils::format("res/childSelection/wireLeft%s.png", _isPortrait ? "_portrait" : ""));
    wireLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    wireLeft->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    wireLeft->setScale(_visibleSize.height / wireLeft->getContentSize().height);
    _contentNode->addChild(wireLeft);
    
    auto wireRight = Sprite::create(StringUtils::format("res/childSelection/wireRight%s.png", _isPortrait ? "_portrait" : ""));
    wireRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    wireRight->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    wireRight->setScale(_visibleSize.height / wireRight->getContentSize().height);
    _contentNode->addChild(wireRight);
}

void ChildSelectorScene::createSettingsButton()
{
    auto settingsButton = SettingsButton::createSettingsButton(0.0f);
    settingsButton->setCenterPosition(Vec2(_visibleSize.width - settingsButton->getContentSize().width,_visibleSize.height - settingsButton->getContentSize().height));
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
    _scrollView->setContentSize(Size(_visibleSize.width * 0.75, _visibleSize.height * (_isPortrait ?  0.65 : 0.6)));
    _scrollView->setPosition(Point(_visibleSize.width * 0.125,_visibleSize.height * (_isPortrait ?  0.2 : 0.25)));
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
    verticalScrollGradientTop->setColor(Color3B::BLACK);
    verticalScrollGradientTop->setPosition(Vec2(_visibleSize.width / 2, _scrollView->getPosition().y + _scrollView->getContentSize().height));
    _contentNode->addChild(verticalScrollGradientTop,1);
    
    Sprite* verticalScrollGradientBottom = Sprite::create("res/decoration/TopNavGrad.png");
    verticalScrollGradientBottom->setAnchorPoint(Vec2(0.5, 1.0));
    verticalScrollGradientBottom->setScaleX(_visibleSize.width * 0.75 / verticalScrollGradientBottom->getContentSize().width);
    verticalScrollGradientBottom->setColor(Color3B::BLACK);
    verticalScrollGradientBottom->setRotation(180);
    verticalScrollGradientBottom->setPosition(Vec2(_visibleSize.width / 2, _scrollView->getPosition().y));
    _contentNode->addChild(verticalScrollGradientBottom,1);
}

Size ChildSelectorScene::getScrollviewInnerSize(float scrollviewWidth)
{
    int childrenPerRow = _isPortrait ? 2 : 4;
    int numChildren = ParentDataProvider::getInstance()->getAmountOfAvailableChildren();
    
    int numRows = ceil(numChildren / (double)childrenPerRow);
    
    return Size(OOMEE_LAYER_WIDTH * childrenPerRow + OOMEE_LAYER_GAP * (childrenPerRow - 1), (OOMEE_LAYER_HEIGHT * numRows) + ((_isPortrait ? OOMEE_LAYER_GAP_PORTRAIT : OOMEE_LAYER_GAP) * (numRows - 0.5)));
}

void ChildSelectorScene::addProfilesToScrollView()
{
    AnalyticsSingleton::getInstance()->registerNoOfChildren(ParentDataProvider::getInstance()->getAmountOfAvailableChildren());
    
    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        auto profileLayer = createChildProfileButton(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(i), i);
        profileLayer->setTag(i);
        profileLayer->setPosition(positionElementOnScrollView(profileLayer));
        _scrollView->addChild(profileLayer);
    }
    
    _parentButton = createParentProfileButton();
    setParentButtonVisible(false);
    _parentButton->setPosition(Vec2(_visibleSize.width * (_isPortrait ? 0.5f : 0.35f), _parentButton->getContentSize().height * 1.7f));
    _contentNode->addChild(_parentButton);
    
}

ui::Button *ChildSelectorScene::createChildProfileButton(const std::string& profileName, int childNum)
{
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
            AnalyticsSingleton::getInstance()->registerChildGenderAndAge(childNumber);
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
    
    auto oomee = RemoteImageSprite::create();
    oomee->initWithUrlAndSizeWithoutPlaceholder(ParentDataProvider::getInstance()->getAvatarForAnAvailableChild(childNum), Size(320, 320));
    oomee->setKeepAspectRatio(true);
    oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    oomee->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    oomee->setName(kOomeeLayerName);
    button->addChild(oomee);
    
    float delayTime = CCRANDOM_0_1() * 0.5;
    if(_firstTime)
    {
        oomee->setOpacity(0);
        oomee->runAction(createBlinkEffect(delayTime, 0.1));
    }
    
    auto glow = Sprite::create("res/childSelection/glow.png");
    glow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    oomee->addChild(glow, -1);
    
    if(_firstTime)
    {
        glow->setOpacity(0);
        glow->runAction(createBlinkEffect(delayTime, 0.1));
    }
    
    auto profileLabel = createLabelChildName(profileName);
    profileLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    profileLabel->setNormalizedPosition(Vec2(0.5,0));
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
    
    int numberOfChildren = ParentDataProvider::getInstance()->getAmountOfAvailableChildren();
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

ui::Button* ChildSelectorScene::createNewProfileButton()
{
    ui::Button* addChildButton = ui::Button::create("res/childSelection/button.png");
    addChildButton->setName("addChildButton");
    addChildButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
			AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChildSelector - AddChild");
            
            createAdultPinLayerWithDelegate();
            
            AnalyticsSingleton::getInstance()->childProfileStartEvent();
        }
    });
    
    float delayTime = CCRANDOM_0_1();
    if(_firstTime)
    {
        addChildButton->setOpacity(0);
        addChildButton->runAction(createBlinkEffect(delayTime, 0.1));
    }
	
	Label* buttonText = Label::createWithTTF(_("Add a profile"), Style::Font::Regular(), addChildButton->getContentSize().height * 0.4f);
	buttonText->setTextColor(Color4B(Style::Color::brightAqua));
	buttonText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	buttonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	buttonText->setHorizontalAlignment(TextHAlignment::CENTER);
	buttonText->setVerticalAlignment(TextVAlignment::CENTER);
	buttonText->setOverflow(Label::Overflow::SHRINK);
	buttonText->setDimensions(addChildButton->getContentSize().width * 0.7f, addChildButton->getContentSize().height * 0.7f);
	addChildButton->addChild(buttonText);
	
    return addChildButton;
}

ui::Button* ChildSelectorScene::createParentProfileButton()
{
    auto parentButton = ui::Button::create("res/childSelection/button.png");
    
    float delayTime = CCRANDOM_0_1() * 0.5;
    if(_firstTime)
    {
        parentButton->setOpacity(0);
        parentButton->runAction(createBlinkEffect(delayTime,0.1));
    }
    
    parentButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
			AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
            AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChildSelector - ParentOomee");
            _parentIconSelected = true;
            
            createAdultPinLayerWithDelegate();
        }
    });
	
	Label* buttonText = Label::createWithTTF(_("Parent inbox"), Style::Font::Regular(), parentButton->getContentSize().height * 0.4f);
	buttonText->setTextColor(Color4B(Style::Color::brightAqua));
	buttonText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	buttonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	buttonText->setHorizontalAlignment(TextHAlignment::CENTER);
	buttonText->setVerticalAlignment(TextVAlignment::CENTER);
	buttonText->setOverflow(Label::Overflow::SHRINK);
	buttonText->setDimensions(parentButton->getContentSize().width * 0.7f, parentButton->getContentSize().height * 0.7f);
	parentButton->addChild(buttonText);
	
    return parentButton;
}

void ChildSelectorScene::setParentButtonVisible(bool visible)
{
    if(_parentButton)
    {
        _parentButton->setVisible(visible);
        if(visible && !_isPortrait && _addChildButton)
        {
            _addChildButton->setPosition(Vec2(_visibleSize.width * 0.65, _addChildButton->getContentSize().height * (_isPortrait ? 3.2f : 1.7f)));
        }
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
    const std::string& userId = ParentDataProvider::getInstance()->getLoggedInParentId();
    const std::string& sessionId = ParentDataProvider::getInstance()->getLoggedInParentCdnSessionId();
    
    HttpRequestCreator* request = API::GetGordenRequest(userId, sessionId, this);
    request->execute();
}

void ChildSelectorScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagCookieRefresh)
    {
        ParentDataParser::getInstance()->parseParentSessionData(body);
        getParentCookiesRequest();
        return;
    }
    
    if(CookieDataParser::getInstance()->parseDownloadCookies(headers))
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
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
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(OfflineHub));
    }
}

void ChildSelectorScene::callDelegateFunction(float dt)
{
    if(ParentDataProvider::getInstance()->getAmountOfAvailableChildren() == 0)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(AddChildFirstTime));
    }
    else
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(AddChild));
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
        ParentDataParser::getInstance()->logoutChild();
        
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
    createSettingsButton();
    addScrollViewForProfiles();
    addProfilesToScrollView();
    addPrivacyButton();
    
    _addChildButton = createNewProfileButton();
    _addChildButton->setPosition(Vec2(_visibleSize.width * 0.5, _addChildButton->getContentSize().height * (_isPortrait ? 3.2f : 1.7f)));
    _contentNode->addChild(_addChildButton);
    
    setParentButtonVisible(parentButtonVisible);
    
    DynamicNodeHandler::getInstance()->rebuildCurrentCTA();
}

NS_AZOOMEE_END
