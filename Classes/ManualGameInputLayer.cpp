#include "ManualGameInputLayer.h"
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "GameDataManager.h"
#include <AzoomeeCommon/UI/ModalMessages.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ManualGameInputLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

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
    backgroundLayer = LayerColor::create(Color4B::BLACK,origin.x + visibleSize.width,origin.y + visibleSize.height);
    
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
    backgroundLayer->addChild(title);
}

void ManualGameInputLayer::addButtons()
{
    backButton = ElectricDreamsButton::createBackButton();
    backButton->setCenterPosition(Vec2(origin.x +backButton->getContentSize().width*.7, origin.y + visibleSize.height - backButton->getContentSize().height*.7));
    backButton->setDelegate(this);
    backButton->setMixPanelButtonName("ManualGameInputBackButton");
    backgroundLayer->addChild(backButton);
    
    startGameButton = ElectricDreamsButton::createNextButton();
    startGameButton->setCenterPosition(Vec2(origin.x + visibleSize.width -startGameButton->getContentSize().width*.7, origin.y+ visibleSize.height - startGameButton->getContentSize().height*.7));
    startGameButton->setDelegate(this);
    startGameButton->setMixPanelButtonName("ManualGameInputStartGameButton");
    backgroundLayer->addChild(startGameButton);
}

void ManualGameInputLayer::addTextBox()
{
    UserDefault* def = UserDefault::getInstance();
    
    uriTextInput = TextInputLayer::createWithSize(Size(1500,500), -1);
    uriTextInput->setText(def->getStringForKey("GameURI", ""));
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
        
        std::string manualGamePath = FileUtils::getInstance()->getWritablePath() + "gameCache/" + GameDataManager::kManualGameId;
        
        if(FileUtils::getInstance()->isDirectoryExist(manualGamePath))
            FileUtils::getInstance()->removeDirectory(manualGamePath);

        FileUtils::getInstance()->createDirectory(manualGamePath);
        
        GameDataManager::getInstance()->getJSONGameData(uriTextInput->getText().c_str(), GameDataManager::kManualGameId);
    }
}

NS_AZOOMEE_END
