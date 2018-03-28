//
//  MeHQ.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/03/2018.
//

#include "MeHQ.h"
#include "HQScene2.h"
#include "HQSceneElement.h"
#include "SceneManagerScene.h"
#include "RecentlyPlayedManager.h"
#include "HQScene2ElementPositioner.h"
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool MeHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setContentSize(Director::getInstance()->getVisibleSize());
    this->setPosition(Director::getInstance()->getVisibleOrigin());
    
    _contentScrollView = cocos2d::ui::ScrollView::create();
    _contentScrollView->setContentSize(Size(this->getContentSize().width * 0.75f, this->getContentSize().height * 0.8));
    _contentScrollView->setPosition(Point(0,0));
    _contentScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    _contentScrollView->setTouchEnabled(true);
    _contentScrollView->setBounceEnabled(true);
    _contentScrollView->setScrollBarEnabled(false);
    _contentScrollView->setSwallowTouches(false);
    _contentScrollView->setInnerContainerSize(Size(this->getContentSize().width * 0.75f, 0));
    this->addChild(_contentScrollView);
    
    float totalHeight = 0;
    
    _artScrollView = HQSceneArtsApp::create();
    _artScrollView->setShowPrivacyButton(false);
    _artScrollView->setSize(Size(_contentScrollView->getContentSize().width * 0.75f, 0));
    //_artScrollView->setOriginPosition(Vec2(0,_contentScrollView->getContentSize().height));
    _contentScrollView->addChild(_artScrollView);
    _contentScrollView->setInnerContainerSize(_contentScrollView->getInnerContainerSize() + Size(0,_artScrollView->getContentSize().height));
    
    totalHeight += _artScrollView->getContentSize().height;
    
    auto recentContent = RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(ConfigStorage::kMixHQName);
    
    if(recentContent.size() > 0)
    {
        Layer* contentLayer = Layer::create();
        contentLayer->setContentSize(Size(_contentScrollView->getContentSize().width, 0));
        
        float lowestElementYPosition = 0;
        
        for(const auto& content : recentContent)
        {
            auto hqSceneElement = HQSceneElement::create();
            hqSceneElement->setCategory(ConfigStorage::kGameHQName);
            hqSceneElement->setItemData(content);
            hqSceneElement->setElementRow(0);
            hqSceneElement->setElementIndex(0);
            hqSceneElement->setMargin(HQScene2::kContentItemMargin);
            hqSceneElement->setManualSizeMultiplier(1); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
            hqSceneElement->addHQSceneElement();
            
            HQScene2ElementPositioner hqScene2ElementPositioner;
            hqScene2ElementPositioner.setElement(hqSceneElement);
            hqScene2ElementPositioner.setCarouselLayer(contentLayer);
            hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
            hqScene2ElementPositioner.setBaseUnitSize(Size(590,442));
            
            Point position = hqScene2ElementPositioner.positionHQSceneElement();
            
            hqSceneElement->setPosition(position);
            contentLayer->addChild(hqSceneElement);
            
            if(position.y < lowestElementYPosition)
            {
                lowestElementYPosition = position.y;
            }
        }
        
        contentLayer->setPosition(Vec2(0,_contentScrollView->getContentSize().height - 300));
        contentLayer->setContentSize(Size(contentLayer->getContentSize().width, -lowestElementYPosition));
        //_contentScrollView->setInnerContainerSize(_contentScrollView->getInnerContainerSize() + Size(0,contentLayer->getContentSize().height));
        _contentScrollView->addChild(contentLayer);
        _artScrollView->setOriginPosition(Vec2(0,_contentScrollView->getContentSize().height + _artScrollView->getContentSize().height));
    }
    
    
    
    _oomeeMakerButton = ui::Button::create("res/artapp/popup_bg.png");
    _oomeeMakerButton->setScale9Enabled(true);
    _oomeeMakerButton->setZoomScale(0);
    _oomeeMakerButton->setContentSize(Size(this->getContentSize().width * 0.2f, this->getContentSize().height * 0.5f));
    _oomeeMakerButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _oomeeMakerButton->setPosition(Vec2(this->getContentSize().width * 0.9f, this->getContentSize().height * 0.5f));
    _oomeeMakerButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(OomeeMakerEntryPointScene));
        }
    });
    this->addChild(_oomeeMakerButton);
    
    RemoteImageSprite* profileImage = RemoteImageSprite::create();
    profileImage->initWithUrlAndSizeWithoutPlaceholder(ChildDataProvider::getInstance()->getLoggedInChildAvatarId(), Size(this->getContentSize().width * 0.2f, this->getContentSize().height * 0.5f), true);
    profileImage->setPosition(_oomeeMakerButton->getContentSize() / 2);
    profileImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _oomeeMakerButton->addChild(profileImage);
    
    
    return true;
}

void MeHQ::onEnter()
{
    Super::onEnter();
}

NS_AZOOMEE_END
