//
//  BrushConfig.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 30/10/2017.
//
//

#include "BrushConfig.h"

using namespace cocos2d;

NS_AZOOMEE_AA_BEGIN

BrushConfigRef BrushConfig::create()
{
    return std::make_shared<BrushConfig>();
}

BrushConfig::BrushConfig()
{
    _brushRadius = 0;
    _selectedColour = Color4F::BLACK;
    _selectedPattern = "";
    _selectedPatternTransparant = "";
    _bgImageFilename = kArtAppAssetLoc + "white_bg.png";
}

void BrushConfig::setBrushRadius(float brushRadius)
{
    _brushRadius = brushRadius;
}

float BrushConfig::getBrushRadius() const
{
    return _brushRadius;
}

void BrushConfig::setSelectedColour(cocos2d::Color4F colour)
{
    _selectedColour = colour;
}

Color4F BrushConfig::getSelectedColour() const
{
    return _selectedColour;
}

void BrushConfig::setSelectedPattern(const std::string& pattern)
{
    _selectedPattern = pattern;
}

std::string BrushConfig::getSelectedPattern() const
{
    return _selectedPattern;
}

void BrushConfig::setSelectedPatternTansparant(const std::string &pattern)
{
    _selectedPatternTransparant = pattern;
}

std::string BrushConfig::getSelectedPatternTransparant() const
{
    return _selectedPatternTransparant;
}

void BrushConfig::setBgImageFilename(const std::string &bgImageFilename)
{
    _bgImageFilename = bgImageFilename;
}

std::string BrushConfig::getBgImageFilename() const
{
    return _bgImageFilename;
}

NS_AZOOMEE_AA_END
