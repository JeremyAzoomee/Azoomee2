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
    
    float oomeeHeight = contentSize.height * 0.3f;
    
    _oomee = OomeeMaker::OomeeCarouselButton::create();
    _oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _oomee->setPosition(mainTitle->getPosition() - Vec2(0,mainTitle->getContentSize().height + offset[isPortrait]));
    _oomee->setPlaceholderImage(ConfigStorage::getInstance()->getLocalImageForOomee(_childCreator->getOomeeNum()));
    _oomee->loadPlaceholderImage();
    //_oomee->setScale(oomeeHeight / _oomee->getContentSize().height);
    _oomee->setContentSize(_oomee->getContentSize() * (oomeeHeight / _oomee->getContentSize().height));
    _oomee->ignoreContentAdaptWithSize(false);
    this->addChild(_oomee);
    
    saveDefaultOomeeToOomeeMakerFiles();
    
    Label* subTitle = Label::createWithTTF(StringUtils::format("Don’t worry if they don’t like it,\nthey can change it anytime%sin the Oomee Maker.",isPortrait ? "\n" : " "), Style::Font::Regular, (is18x9 && !isPortrait) ? 75 : 90);
    subTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    subTitle->setPosition(_oomee->getPosition() - Vec2(0,oomeeHeight + offset[isPortrait]));
    subTitle->setColor(Color3B::WHITE);
    subTitle->setHorizontalAlignment(TextHAlignment::CENTER);
    this->addChild(subTitle);
    
    ui::Button* doneButton = ui::Button::create("res/buttons/MainButton.png");
    doneButton->setColor(Style::Color::telish);
    doneButton->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE : Vec2::ANCHOR_MIDDLE_RIGHT);
    doneButton->setPosition(Vec2(contentSize.width * (isPortrait ? 0.5f : 0.7325f), contentSize.height * (isPortrait ? 0.125f : 0.2f)));
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
    addAnotherButton->setPosition(Vec2(contentSize.width * (isPortrait ? 0.5f : 0.2675f), contentSize.height * (isPortrait ? 0.225f : 0.2f)));
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

void ChildOomeeLayer::saveDefaultOomeeToOomeeMakerFiles()
{
    const std::string& imageData = FileUtils::getInstance()->getStringFromFile(ConfigStorage::getInstance()->getLocalImageForOomee(_childCreator->getOomeeNum()));
    const std::string& oomeeConfig = FileUtils::getInstance()->getStringFromFile(ConfigStorage::getInstance()->getLocalConfigForOomee(_childCreator->getOomeeNum()));
    std::string filePath = FileUtils::getInstance()->getWritablePath() + "oomeeMaker/";
    if(!FileUtils::getInstance()->isDirectoryExist(filePath))
    {
        FileUtils::getInstance()->createDirectory(filePath);
    }
    filePath += _childCreator->getCreatedChildId() + "/";
    if(!FileUtils::getInstance()->isDirectoryExist(filePath))
    {
        FileUtils::getInstance()->createDirectory(filePath);
    }
    
    FileUtils::getInstance()->writeStringToFile(imageData, filePath + "default_oomee.png");
    FileUtils::getInstance()->writeStringToFile(oomeeConfig, filePath + "default_oomee.oomee");
}

NS_AZOOMEE_END



