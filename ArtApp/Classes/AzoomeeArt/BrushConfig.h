//
//  BrushConfig.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/10/2017.
//
//

#ifndef BrushConfig_h
#define BrushConfig_h

#include "AzoomeeArtApp.h"

USING_NS_TZ
NS_AZ_AA_BEGIN

class BrushConfig;

typedef std::shared_ptr<BrushConfig> BrushConfigRef;

class BrushConfig
{
private:
    float _brushRadius;
    cocos2d::Color4F _selectedColour;
    std::string _selectedPattern;
    std::string _selectedPatternTransparant;
    std::string _bgImageFilename;
    
public:
    static BrushConfigRef create();
    BrushConfig();
    
    void setBrushRadius(float brushRadius);
    float getBrushRadius() const;
    void setSelectedColour(cocos2d::Color4F colour);
    cocos2d::Color4F getSelectedColour() const;
    void setSelectedPattern(const std::string& pattern);
    std::string getSelectedPattern() const;
    void setSelectedPatternTansparant(const std::string& pattern);
    std::string getSelectedPatternTransparant() const;
    void setBgImageFilename(const std::string& bgImageFilename);
    std::string getBgImageFilename() const;
    
};

NS_AZ_AA_END

#endif /* BrushConfig_h */

