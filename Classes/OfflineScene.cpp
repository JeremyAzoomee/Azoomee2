//
//  OfflineScene.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 20/11/2019.
//

#include "OfflineScene.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "GameDataManager.h"
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include "FlowDataSingleton.h"
#include "LoginController.h"
#include "HQHistoryManager.h"
#include "BackEndCaller.h"
#include "ContentOpener.h"
#include "Style.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const Size OfflineScene::kTileSize = Size(570, 430);
const float OfflineScene::kPadding = 20.0f;

bool OfflineScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    ChildManager::getInstance()->setChildLoggedIn(false);
    HQHistoryManager::getInstance()->setIsOffline(true);
    UserDefault* def = UserDefault::getInstance();
    const std::string& lastLoggedInChildId = def->getStringForKey("lastLoggedInChildId");
    def->flush();
    
    if(lastLoggedInChildId != "")
    {
        ChildManager::getInstance()->loginChildOffline(lastLoggedInChildId);
    }
    
    ui::Layout* bgColour = ui::Layout::create();
    bgColour->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    bgColour->setBackGroundColor(Colours::Color_3B::darkIndigo);
    bgColour->setSizeType(ui::Layout::SizeType::PERCENT);
    bgColour->setSizePercent(Vec2(1.0f,1.0f));
    addChild(bgColour);
    
    _bgPattern = RoundedRectSprite::create();
    _bgPattern->setTexture("res/decoration/pattern_stem_tile.png");
    _bgPattern->setColor(Colours::Color_3B::azure);
    _bgPattern->setRoundedCorners(false, false, false, false);
    _bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    addChild(_bgPattern);
    
    _bgGradient = LayerGradient::create();
    _bgGradient->setStartColor(Colours::Color_3B::darkIndigo);
    _bgGradient->setStartOpacity(155);
    _bgGradient->setEndColor(Colours::Color_3B::darkIndigo);
    _bgGradient->setEndOpacity(0);
    _bgGradient->setVector(Vec2(0,1));
    _bgGradient->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
    _bgGradient->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(_bgGradient);
    
    _offlineMsgLayout = ui::Layout::create();
    _offlineMsgLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _offlineMsgLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _offlineMsgLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    addChild(_offlineMsgLayout);
    
    _offlineLogo = ui::ImageView::create("res/offline/offlineIcon.png");
    _offlineLogo->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _offlineMsgLayout->addChild(_offlineLogo);
    
    _offlineText = DynamicText::create(_("You are offline"), Style::Font::PoppinsBold(), 102);
    _offlineText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _offlineText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _offlineText->setTextColor(Color4B::WHITE);
    _offlineText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,kPadding,0,0)));
    _offlineText->setMaxLineWidth(800);
    _offlineMsgLayout->addChild(_offlineText);
    
    _checkConnectionText = DynamicText::create(_("Check your connection and try again"), Style::Font::PoppinsBold(), 50);
    _checkConnectionText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _checkConnectionText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _checkConnectionText->setTextColor(Color4B::WHITE);
    _checkConnectionText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,kPadding,0,kPadding)));
    _checkConnectionText->setMaxLineWidth(650);
    _offlineMsgLayout->addChild(_checkConnectionText);
    
    _retryButton = TextButton::create("res/onboarding/rounded_button.png");
    _retryButton->ignoreContentAdaptWithSize(false);
    _retryButton->setScale9Enabled(true);
    _retryButton->setContentSize(Size(460,140));
    _retryButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _retryButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,kPadding,0,0)));
    _retryButton->setColor(Colours::Color_3B::white);
    _retryButton->setTextColour(Color4B::BLACK);
    _retryButton->setTextFontInfo(Style::Font::PoppinsBold(), 66);
    _retryButton->setTextAreaSizePercent(Vec2(0.9f,0.8f));
    _retryButton->setText(_("Retry"));
    _retryButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            BackEndCaller::getInstance()->offlineCheck();
        }
    });
    _offlineMsgLayout->addChild(_retryButton);
    
    _offlineMsgLayout->setContentSize(Size(0, _offlineLogo->getContentSize().height + _offlineText->getContentSize().height + _checkConnectionText->getContentSize().height + _retryButton->getContentSize().height + (4 * kPadding)));
    
    _gamesLayout = ui::Layout::create();
    _gamesLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _gamesLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _gamesLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    addChild(_gamesLayout);
    
    _gamesSmallHeading = DynamicText::create(_("In the meantime"), Style::Font::PoppinsRegular(), 46);
    _gamesSmallHeading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _gamesSmallHeading->setTextVerticalAlignment(TextVAlignment::CENTER);
    _gamesSmallHeading->setTextColor(Color4B::WHITE);
    _gamesSmallHeading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _gamesSmallHeading->setMaxLineWidth(800);
    _gamesLayout->addChild(_gamesSmallHeading);
    
    _playTheseText = DynamicText::create(_("Play these offline"), Style::Font::PoppinsBold(), 66);
    _playTheseText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _playTheseText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _playTheseText->setTextColor(Color4B::WHITE);
    _playTheseText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,kPadding,0,0)));
    _playTheseText->setMaxLineWidth(800);
    _gamesLayout->addChild(_playTheseText);
    
    _gamesList = ui::ListView::create();
    _gamesList->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,kPadding,0,2 * kPadding)));
    _gamesList->setItemsMargin(35);
    _gamesList->setBounceEnabled(true);
    _gamesLayout->addChild(_gamesList);
    
    return true;
}

void OfflineScene::onEnter()
{
    OfflineChecker::getInstance()->setDelegate(this);
    _gamesLayout->setVisible(ChildManager::getInstance()->isChildLoggedIn());
    Super::onEnter();
}

void OfflineScene::onExit()
{
    OfflineChecker::getInstance()->setDelegate(nullptr);
    Super::onExit();
}

void OfflineScene::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    
    _bgPattern->setContentSize(contentSize);
    _bgGradient->setContentSize(contentSize);
    
    populateOfflineGamesList();
    
    _gamesLayout->setContentSize(Size(0,_gamesSmallHeading->getContentSize().height + _playTheseText->getContentSize().height + _gamesList->getContentSize().height + (4 * kPadding)));
    _offlineMsgLayout->setNormalizedPosition(Vec2(0.5f, contentSize.width < contentSize.height ? 0.6f : 0.5f));
}

void OfflineScene::populateOfflineGamesList()
{
    _gamesList->removeAllItems();
    auto games = GameDataManager::getInstance()->getOfflineGameList();
    MutableHQContentItemObjectRef artContent = MutableHQContentItemObject::create();
    artContent->setUri(HQConsts::kArtAppURI);
    artContent->setType(HQContentItemObject::kContentTypeInternal);
    artContent->setEntitled(true);
    games.insert(games.begin(), artContent);
    const float tileSpacing = 35.0f;
    
    const int numCols = MAX(2, floor((getContentSize().width * 0.9f) / (kTileSize.width + tileSpacing)));
    const Size& tileSize = kTileSize * MIN(1.0f, (getContentSize().width * 0.45f) - tileSpacing);
    const int visibleRows = _contentSize.width < _contentSize.height ? 2 : 1;
    const int rows = ceil(games.size() / (float)numCols);
    
    _gamesList->setContentSize(Size((tileSize.width + tileSpacing) * numCols, (tileSize.height + tileSpacing) * visibleRows));
    
    for(int row = 0; row < rows; row++)
    {
        ui::Layout* rowContainer = ui::Layout::create();
        rowContainer->setContentSize(Size(_gamesList->getContentSize().width, tileSize.height));
        rowContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        rowContainer->setNormalizedPosition(Vec2(0.5f,1.0 - ((row + 0.5f) / rows)));
        _gamesList->pushBackCustomItem(rowContainer);
        
        for(int col = 0; col < numCols; col++)
        {
            if((row * numCols) + col < games.size())
            {
                const auto& content = games.at((row * numCols) + col);
                FeaturedTile* tile = FeaturedTile::create();
                tile->setPlaceholderFilename(content->getUri() == HQConsts::kArtAppURI ? "res/contentPlaceholders/artStudio.jpg" : "res/contentPlaceholders/Games1X1.png");
                tile->setContentSize(tileSize);
                tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                tile->setNormalizedPosition(Vec2((col + 0.5) / numCols,0.5f));
                tile->setImageShape(Vec2(1,1));
                tile->setContentItemData(content);
                tile->setContentSelectedCallback([this](HQContentItemObjectRef content){
                    ContentOpener::getInstance()->openContentObject(content);
                });
                tile->setTouchEnabled(true);
                rowContainer->addChild(tile);
            }
        }
    }
    
}

void OfflineScene::connectivityStateChanged(bool online)
{
    if(online)
    {
        OfflineChecker::getInstance()->setDelegate(nullptr);
        ChildManager::getInstance()->setChildLoggedIn(false);
        HQHistoryManager::getInstance()->setIsOffline(false);
        FlowDataSingleton::getInstance()->clearData();
        LoginController::getInstance()->doLoginLogic();
    }
}

NS_AZOOMEE_END

