//
//  ImageConverterLoadingLayer.cpp
//  azoomee2
//
//  Created by Macauley on 03/08/2017.
//
//

#include "ImageConverterLoadingLayer.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/UI/Style.h>
#include "HQSceneArtsApp.h"

USING_NS_CC;

NS_AZOOMEE_BEGIN

void ImageConverterLoadingLayer::setFileNames(std::vector<std::string> filenames)
{
    this->filenames = filenames;
    currentImage = 1;
    totalImages = (int)filenames.size();
    //this->scheduleUpdate();
}

void ImageConverterLoadingLayer::onEnter()
{
    finished = false;
    ModalMessages::getInstance()->startLoading();
    std::string str = StringUtils::format("Converting Image %i of %i",currentImage,totalImages);
    text = Label::createWithTTF(str, Style::Font::Regular, 128);
    text->setColor(Style::Color::white);
    text->setPosition(Director::getInstance()->getVisibleSize()/2);
    text->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(text,10);
    //have to force it to call update because it hates me for some reason :'(
    Director::getInstance()->getScheduler()->schedule([&](float dt){this->update(dt);}, this, 0.5f, false, "update");
}

void ImageConverterLoadingLayer::update(float dt)
{
    if(filenames.size() > 0)
    {
        ArtAppImageConverter converter = ArtAppImageConverter(filenames.back());
        converter.convertImage();
        filenames.pop_back();
        if(filenames.size() > 0)
            currentImage++;
        std::string str = StringUtils::format("Converting Image %i of %i",currentImage,totalImages);
        text->setString(str);
    }
    else
    {
        Director::getInstance()->getScheduler()->unschedule("update", this);
        finished = true;
        auto parent = this->getParent();
        auto offlineArtsAppScrollView = HQSceneArtsApp::create();
        offlineArtsAppScrollView->setName("ARTS APP");
        parent->addChild(offlineArtsAppScrollView);
        this->removeFromParent();
    }
}

void ImageConverterLoadingLayer::onExit()
{
    ModalMessages::getInstance()->stopLoading();
}

NS_AZOOMEE_END

