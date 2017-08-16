//
//  ImageConverterLoadingLayer.h
//  azoomee2
//
//  Created by Macauley on 03/08/2017.
//
//

#ifndef ImageConverterLoadingLayer_h
#define ImageConverterLoadingLayer_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "ArtAppImageConverter.h"

NS_AZOOMEE_BEGIN

class ImageConverterLoadingLayer : public cocos2d::Layer
{
private:
    std::vector<std::string> filenames;
    int totalImages;
    int currentImage;
    cocos2d::Label* text;
public:
    CREATE_FUNC(ImageConverterLoadingLayer);
    
    void setFileNames(std::vector<std::string> filenames);
    
    void onEnter() override;
    void update(float dt) override;
    void onExit() override;
    
    bool finished;
};

NS_AZOOMEE_END

#endif /* ImageConverterLoadingLayer_h */
