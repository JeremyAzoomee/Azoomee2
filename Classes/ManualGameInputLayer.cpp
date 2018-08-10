#include "ManualGameInputLayer.h"
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "GameDataManager.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "ContentHistoryManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ManualGameInputLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    setName(ConfigStorage::kContentTypeManual);
    createBackgroundLayer();
    addSideWiresToScreen(this, 0, 2);
    addTitle();
    addButtons();
    addTextBox();
    
    return true;
}

//---------------------- Create Background Layer -----------------------------

void ManualGameInputLayer::createBackgroundLayer()
{
    backgroundLayer = LayerColor::create(Color4B::BLACK,visibleSize.width, visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void ManualGameInputLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) 
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

//--------------------- Add Objects To Layer -------------------------

void ManualGameInputLayer::addTitle()
{
    auto title = createLabelHeader("Add Game URL");
    title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.9f));
    backgroundLayer->addChild(title);
}

void ManualGameInputLayer::addButtons()
{
    backButton = ElectricDreamsButton::createBackButton();
    backButton->setCenterPosition(Vec2(backButton->getContentSize().width*.7, visibleSize.height - backButton->getContentSize().height*.7));
    backButton->setDelegate(this);
    backButton->setMixPanelButtonName("ManualGameInputBackButton");
    backgroundLayer->addChild(backButton);
    
    startGameButton = ElectricDreamsButton::createNextButton();
    startGameButton->setCenterPosition(Vec2(visibleSize.width -startGameButton->getContentSize().width*.7, visibleSize.height - startGameButton->getContentSize().height*.7));
    startGameButton->setDelegate(this);
    startGameButton->setMixPanelButtonName("ManualGameInputStartGameButton");
    backgroundLayer->addChild(startGameButton);
}

void ManualGameInputLayer::addTextBox()
{
    UserDefault* def = UserDefault::getInstance();
    
    uriTextInput = TextInputLayer::createWithSize(Size(visibleSize.width * 0.75f,500), -1);
    uriTextInput->setText(def->getStringForKey("GameURI", ""));
    uriTextInput->setCenterPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.70f));
    backgroundLayer->addChild(uriTextInput);
    def->flush();
}

//---------------------- Actions -----------------

void ManualGameInputLayer::removeSelf()
{
    this->removeChild(backgroundLayer);
    this->removeFromParent();
}

//----------------------- Delegate Functions ----------------------------

void ManualGameInputLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == backButton)
    {
        removeSelf();
    }
    else if(button == startGameButton)
    {
        ModalMessages::getInstance()->startLoading();
        
        UserDefault* def = UserDefault::getInstance();
        def->setStringForKey("GameURI", uriTextInput->getText());
        def->flush();
        
        std::string manualGamePath = ConfigStorage::getInstance()->getGameCachePath() + GameDataManager::kManualGameId;
        
        if(FileUtils::getInstance()->isDirectoryExist(manualGamePath))
            FileUtils::getInstance()->removeDirectory(manualGamePath);

        FileUtils::getInstance()->createDirectory(manualGamePath);
        HQContentItemObjectRef contentItem = HQContentItemObject::create();
        contentItem->setContentItemId(GameDataManager::kManualGameId);
        ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
        GameDataManager::getInstance()->getJSONGameData(uriTextInput->getText().c_str(), GameDataManager::kManualGameId);
    }
}

NS_AZOOMEE_END
