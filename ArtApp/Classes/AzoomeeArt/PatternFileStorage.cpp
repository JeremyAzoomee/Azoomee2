//
//  PatternFileStorage.cpp
//  azoomee2_ArtApp
//
//  Created by Macauley on 19/10/2017.
//
//

#include "PatternFileStorage.h"

NS_AZOOMEE_AA_BEGIN

static std::auto_ptr<PatternFileStorage> sPatternFileStorageSharedInstance;

PatternFileStorage* PatternFileStorage::getInstance()
{
    if(!sPatternFileStorageSharedInstance.get())
    {
        sPatternFileStorageSharedInstance.reset(new PatternFileStorage());
        sPatternFileStorageSharedInstance->init();
    }
    return sPatternFileStorageSharedInstance.get();
}

void PatternFileStorage::init()
{
    _fileStore["gold"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/glitter_gold.png", kArtAppAssetLoc + "patterns/glitter_gold_t.png");
    _fileStore["silver"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/glitter_silver.png", kArtAppAssetLoc + "patterns/glitter_silver_t.png");
    _fileStore["pink"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/glitter_pink.png", kArtAppAssetLoc + "patterns/glitter_pink_t.png");
    _fileStore["blue"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/glitter_blue.png", kArtAppAssetLoc + "patterns/glitter_blue_t.png");
    _fileStore["green"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/glitter_green.png", kArtAppAssetLoc + "patterns/glitter_green_t.png");
}

std::string PatternFileStorage::getNormalFileByName(const std::string &name) const
{
    try{
        const std::string& value = _fileStore.at(name).first;
        return value;
    }
    catch(std::out_of_range){
        return "";
    }
}

std::string PatternFileStorage::getTransparantFileByName(const std::string &name) const
{
    try{
        const std::string& value = _fileStore.at(name).second;
        return value;
    }
    catch(std::out_of_range){
        return "";
    }
}

NS_AZOOMEE_AA_END
