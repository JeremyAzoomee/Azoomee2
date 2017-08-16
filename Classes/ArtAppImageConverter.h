//
//  ArtAppImageConverter.h
//  azoomee2
//
//  Created by Macauley on 03/08/2017.
//
//

#ifndef ArtAppImageConverter_h
#define ArtAppImageConverter_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

class ArtAppImageConverter
{
private:
    std::string filename;
    
public:
    ArtAppImageConverter(std::string filename);
    
    bool convertImage();
    
};

NS_AZOOMEE_END

#endif /* ArtAppImageConverter_h */
