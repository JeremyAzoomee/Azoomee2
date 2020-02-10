#include "ManualGameInputLayer.h"
#include "GameDataManager.h"
#include "ModalMessages.h"
#include "ContentHistoryManager.h"
#include <AzoomeeCommon/Utils/DirUtil.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include "Style.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ManualGameInputLayer::init()
{
    if ( !Super::init() )
    {
        return false;
    }
    
    setName(HQContentItemObject::kContentTypeManual);
    
    _background = ui::Layout::create();
    _background->setSizeType(ui::Layout::SizeType::PERCENT);
    _background->setSizePercent(Vec2(1.0,1.0));
    _background->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _background->setBackGroundColor(Colours::Color_3B::darkIndigo);
    addChild(_background);

    addTitle();
    addButtons();
    addTextBox();
    
    return true;
}

//--------------------- Add Objects To Layer -------------------------

void ManualGameInputLayer::addTitle()
{
    DynamicText* title = DynamicText::create("Add Game URL", Style::Font::PoppinsBold(), 79);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    title->setNormalizedPosition(Vec2(0.5f, 0.9f));
    title->setTextVerticalAlignment(TextVAlignment::CENTER);
    title->setTextHorizontalAlignment(TextHAlignment::CENTER);
    title->setTextColor(Color4B::WHITE);
    addChild(title);
}

void ManualGameInputLayer::addButtons()
{
    _backButton = ui::Button::create("res/login/back_btn.png");
    _backButton->setAnchorPoint(Vec2(-0.25f, 1.25f));
    _backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    _backButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
        }
    });
    addChild(_backButton);
    
    _startGameButton = ui::Button::create("res/login/next_btn.png");
    _startGameButton->setAnchorPoint(Vec2(1.25f, 1.25f));
    _startGameButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    _startGameButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            UserDefault* def = UserDefault::getInstance();
            def->setStringForKey("GameURI", _uriTextInput->getText());
            def->flush();
            
            if(_streamGameCheckbox->isSelected())
            {
                if(_uriTextInput->getText().size() > 4)
                {
                    MutableHQContentItemObjectRef contentItem = MutableHQContentItemObject::create();
                    contentItem->setContentItemId(GameDataManager::kManualGameId);
                    contentItem->setType(HQContentItemObject::kContentTypeManual);
                    ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
                    Director::getInstance()->replaceScene(SceneManagerScene::createWebview(Director::getInstance()->getVisibleSize().width > Director::getInstance()->getVisibleSize().height ? Landscape : Portrait, _uriTextInput->getText()));
                }
            }
            else
            {
                ModalMessages::getInstance()->startLoading();
                
                const std::string& manualGamePath = DirUtil::getCachesPath() + GameDataManager::kGameCacheFolder + GameDataManager::kManualGameId;
                
                if(FileUtils::getInstance()->isDirectoryExist(manualGamePath))
                {
                    FileUtils::getInstance()->removeDirectory(manualGamePath);
                }

                FileUtils::getInstance()->createDirectory(manualGamePath);
                MutableHQContentItemObjectRef contentItem = MutableHQContentItemObject::create();
                contentItem->setContentItemId(GameDataManager::kManualGameId);
                contentItem->setType(HQContentItemObject::kContentTypeManual);
                ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
                
                GameDataManager::getInstance()->getJSONGameData(_uriTextInput->getText().c_str(), GameDataManager::kManualGameId);
            }
        }
    });
    addChild(_startGameButton);
    
    _streamGameCheckbox = ui::CheckBox::create("res/buttons/check-box-empty-white.png", "res/buttons/correct-symbol-white.png");
    _streamGameCheckbox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _streamGameCheckbox->setNormalizedPosition(Vec2(0.5f, 0.33f));
    addChild(_streamGameCheckbox);
    
    Label* checkboxLabel = Label::createWithTTF("Stream Game?", Style::Font::Regular(), 72);
    checkboxLabel->setTextColor(Color4B::WHITE);
    checkboxLabel->setAnchorPoint(Vec2(0.5, -0.5));
    checkboxLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _streamGameCheckbox->addChild(checkboxLabel);
}

void ManualGameInputLayer::addTextBox()
{
    UserDefault* def = UserDefault::getInstance();
    
    _uriTextInput = TextInputLayer::createWithSize(Size(getContentSize().width * 0.75f,500), -1);
    _uriTextInput->setText(def->getStringForKey("GameURI", ""));
    _uriTextInput->setCenterPosition(Vec2(getContentSize().width * 0.5f, getContentSize().height * 0.7f));
    addChild(_uriTextInput);
    def->flush();
}

NS_AZOOMEE_END
