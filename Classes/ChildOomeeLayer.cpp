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
     bool is18x9 = ConfigStorage::getInstance()->isDevice18x9();
    const float offset[2] = {is18x9 ? 50.0f : 100.0f, 200.0f};
    const Size& contentSize = this->getContentSize();
    
    bool isPortrait = contentSize.width < contentSize.height;
   
    
    Label* mainTitle = Label::createWithTTF(StringUtils::format("Here is %s’s Oomee",_childCreator->getName().c_str()), Style::Font::Regular, 120);
    mainTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    mainTitle->setPosition(Vec2(contentSize.width / 2, contentSize.height - (mainTitle->getContentSize().height * ((is18x9 && !isPortrait) ? 0.5f : 1.0f))));
    mainTitle->setColor(Style::Color::white);
    mainTitle->enableGlow(Color4B(Style::Color::telish));
    mainTitle->setMaxLineWidth(contentSize.width * 0.9);
    mainTitle->setHorizontalAlignment(TextHAlignment::CENTER);
    this->addChild(mainTitle);
    
    float oomeeHeight = contentSize.height * 0.3;
    
    _oomee = OomeeMaker::OomeeCarouselButton::create();
    _oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _oomee->setPosition(mainTitle->getPosition() - Vec2(0,mainTitle->getContentSize().height + offset[isPortrait]));
    _oomee->setPlaceholderImage("res/oomeeMaker/1_Oomee_Reference.png");
    _oomee->loadPlaceholderImage();
    //_oomee->setScale(oomeeHeight / _oomee->getContentSize().height);
    _oomee->setContentSize(_oomee->getContentSize() * (oomeeHeight / _oomee->getContentSize().height));
    _oomee->ignoreContentAdaptWithSize(false);
    this->addChild(_oomee);
    
    _oomeeDownloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
    _oomeeDownloader->downloadImage(this, ConfigStorage::getInstance()->getUrlForOomee(_childCreator->getOomeeNum()));
    
    Label* subTitle = Label::createWithTTF(StringUtils::format("Don’t worry if they don’t like it,\nthey can change it anytime%sin the Oomee Maker.",isPortrait ? "\n" : " "), Style::Font::Regular, (is18x9 && !isPortrait) ? 75 : 90);
    subTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    subTitle->setPosition(_oomee->getPosition() - Vec2(0,oomeeHeight + offset[isPortrait]));
    subTitle->setColor(Color3B::WHITE);
    subTitle->setHorizontalAlignment(TextHAlignment::CENTER);
    this->addChild(subTitle);
    
    ui::Button* doneButton = ui::Button::create("res/buttons/MainButton.png");
    doneButton->setColor(Style::Color::telish);
    doneButton->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE : Vec2::ANCHOR_MIDDLE_RIGHT);
    doneButton->setPosition(Vec2(contentSize.width * (isPortrait ? 0.5 : 0.7325f), contentSize.height * (isPortrait ? 0.125 : 0.2)));
    doneButton->ignoreContentAdaptWithSize(false);
    doneButton->setScale9Enabled(true);
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
    doneButton->setContentSize(Size(doneButtonText->getContentSize().width + 160, doneButton->getContentSize().height));
    
    ui::Button* addAnotherButton = ui::Button::create("res/buttons/MainButton.png");
    addAnotherButton->setColor(Style::Color::telish);
    addAnotherButton->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE : Vec2::ANCHOR_MIDDLE_LEFT);
    addAnotherButton->setPosition(Vec2(contentSize.width * (isPortrait ? 0.5 : 0.2675f), contentSize.height * (isPortrait ? 0.225 : 0.2)));
    addAnotherButton->ignoreContentAdaptWithSize(false);
    addAnotherButton->setScale9Enabled(true);
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
    addAnotherButton->setContentSize(Size(addAnotherButtonText->getContentSize().width + 160, addAnotherButton->getContentSize().height));
    
    Sprite* progressIcon = Sprite::create("res/decoration/progress3.png");
    progressIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    progressIcon->setPosition(Vec2(contentSize.width / 2.0f, progressIcon->getContentSize().height));
    this->addChild(progressIcon);
    
    Super::onEnter();
}

void ChildOomeeLayer::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _oomee->setMainImage(downloader->getLocalImagePath());
    float oomeeHeight = this->getContentSize().height * 0.3;
    _oomee->setContentSize(_oomee->getContentSize() * (oomeeHeight / _oomee->getContentSize().height));
}
void ChildOomeeLayer::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END



