//
//  ChildOomeeLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 05/07/2018.
//

#include "ChildOomeeLayer.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChildOomeeLayer::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    return true;
}

void ChildOomeeLayer::onEnter()
{
    const Size& contentSize = this->getContentSize();
    
    Label* mainTitle = Label::createWithTTF(StringUtils::format("Here is %s’s Oomee",_childCreator->getName().c_str()), Style::Font::Bold, 120);
    mainTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    mainTitle->setPosition(Vec2(contentSize.width / 2, contentSize.height - mainTitle->getContentSize().height));
    mainTitle->setColor(Style::Color::telish);
    mainTitle->enableGlow(Color4B(Style::Color::telish));
    this->addChild(mainTitle);
    
    _oomee = OomeeMaker::OomeeCarouselButton::create();
    _oomee = OomeeMaker::OomeeCarouselButton::create();
    _oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _oomee->setNormalizedPosition(Vec2(0.5,0.5));
    _oomee->setPlaceholderImage("res/oomeeMaker/1_Oomee_Reference.png");
    _oomee->loadPlaceholderImage();
    
    _oomee->setScale((this->getContentSize().height * 0.4f) / _oomee->getContentSize().height);
    
    _oomeeDownloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
    _oomeeDownloader->downloadImage(this, ConfigStorage::getInstance()->getUrlForOomee(0));
    
    Label* subTitle = Label::createWithTTF("Don’t worry if they don’t like it,\nthey can change it anytime in the Oomee Maker.", Style::Font::Bold, 90);
    subTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    subTitle->setPosition(mainTitle->getPosition() - Vec2(0, mainTitle->getContentSize().height * 1.5f));
    subTitle->setColor(Color3B::WHITE);
    this->addChild(subTitle);
    
    ui::Button* doneButton = ui::Button::create("res/buttons/MainButton.png");
    doneButton->setColor(Style::Color::greenishTeal);
    doneButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    doneButton->setPosition(Vec2(contentSize.width * 0.66f, contentSize.height * 0.3));
    doneButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_delegate)
            {
                _delegate->nextLayer();
            }
        }
    });
    this->addChild(doneButton);
    
    Label* doneButtonText = Label::createWithTTF("Done", Style::Font::Regular, doneButton->getContentSize().height * 0.4f);
    doneButtonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    doneButtonText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    doneButtonText->setTextColor(Color4B::BLACK);
    doneButton->addChild(doneButtonText);
    
    ui::Button* addAnotherButton = ui::Button::create("res/buttons/MainButton.png");
    addAnotherButton->setColor(Style::Color::greenishTeal);
    addAnotherButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addAnotherButton->setPosition(Vec2(contentSize.width * 0.66f, contentSize.height * 0.3));
    addAnotherButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_delegate)
            {
                _delegate->prevLayer();
            }
        }
    });
    this->addChild(addAnotherButton);
    
    Label* addAnotherButtonText = Label::createWithTTF("Add another child", Style::Font::Regular, addAnotherButton->getContentSize().height * 0.4f);
    addAnotherButtonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addAnotherButtonText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    addAnotherButtonText->setTextColor(Color4B::BLACK);
    addAnotherButton->addChild(addAnotherButtonText);
    
    Sprite* progressIcon = Sprite::create("res/decoration/progress3.png");
    progressIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    progressIcon->setPosition(Vec2(contentSize.width / 2.0f, progressIcon->getContentSize().height));
    this->addChild(progressIcon);
    
    Super::onEnter();
}

void ChildOomeeLayer::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _oomee->setMainImage(downloader->getLocalImagePath());
}
void ChildOomeeLayer::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END



