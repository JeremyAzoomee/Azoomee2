//
//  MeHQ.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/03/2018.
//

#include "MeHQ.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

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
    
    _artScrollView = HQSceneArtsApp::create();
    _artScrollView->setShowPrivacyButton(false);
    _artScrollView->setOriginPosition(Point(0,this->getContentSize().height * 0.5f));
    _artScrollView->setSize(Size(this->getContentSize().width * 0.75f, 0));
    this->addChild(_artScrollView);
    
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
