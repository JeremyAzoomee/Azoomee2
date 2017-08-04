//
//  ArtAppImageConverter.cpp
//  azoomee2
//
//  Created by Macauley on 03/08/2017.
//
//

#include "ArtAppImageConverter.h"

USING_NS_CC;

NS_AZOOMEE_BEGIN

ArtAppImageConverter::ArtAppImageConverter(std::string filename)
{
    this->filename = filename;
}

bool ArtAppImageConverter::convertImage()
{
    Sprite* imageSprite = Sprite::create();
    Image *img = new Image();
    std::string imageData = FileUtils::getInstance()->getStringFromFile(filename);
    if(imageData.length() <= 22) return false;
    
    imageData = imageData.substr(22);
    
    int len = 0;
    unsigned char *buffer;
    len = base64Decode((const unsigned char*)imageData.c_str(), (unsigned int)imageData.length(), &buffer);
    
    img->initWithImageData(buffer, len);
    Texture2D *texture = new Texture2D();
    texture->initWithImage(img);
    
    imageSprite->initWithTexture(texture);
    
    delete img;
    
    //Size imageSize = imageSprite->getContentSize();
    //Size windowSize = Director::getInstance()->getWinSize();
    
    //imageSprite->setScale(windowSize.width/imageSize.width);
    imageSprite->setAnchorPoint(Vec2(0.5,0.5));
    imageSprite->setPosition(imageSprite->getContentSize()/2);
    
    std::string saveFile = filename.substr(0,filename.length()-4);
    saveFile = saveFile.substr(saveFile.find("artCache/"));
    saveFile = saveFile.append("png");
    
    RenderTexture* renderTex = RenderTexture::create(imageSprite->getContentSize().width, imageSprite->getContentSize().height);
    //renderTex->setAnchorPoint(Vec2(0.5,0.5));
    //renderTex->setPosition(Vec2(0,0));
    renderTex->beginWithClear(1, 1, 1, 1);
    imageSprite->visit();
    renderTex->end();
    renderTex->saveToFile(saveFile, Image::Format::PNG);
    Director::getInstance()->getRenderer()->render();
    
    FileUtils::getInstance()->removeFile(filename);
    
    return true;
    
}

NS_AZOOMEE_END
