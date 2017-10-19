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
    _fileStore = std::map<std::string, std::pair<std::string,std::string>>();
    _fileStore["gold"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/Glitter_gold_1.png", kArtAppAssetLoc + "patterns/Glitter_gold_1t.png");
    _fileStore["silver"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/Glitter_silver_1.png", kArtAppAssetLoc + "patterns/Glitter_silver_1t.png");
    _fileStore["pink"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/Glitter_pink_1.png", kArtAppAssetLoc + "patterns/Glitter_pink_1t.png");
    _fileStore["blue"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/Glitter_blue_1.png", kArtAppAssetLoc + "patterns/Glitter_blue_1t.png");
    _fileStore["gold2"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/Glitter_gold_2.png", kArtAppAssetLoc + "patterns/Glitter_gold_2t.png");
    _fileStore["silver2"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/Glitter_silver_2.png", kArtAppAssetLoc + "patterns/Glitter_silver_2t.png");
    _fileStore["pink2"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/Glitter_pink_2.png", kArtAppAssetLoc + "patterns/Glitter_pink_2t.png");
    _fileStore["blue2"] = std::pair<std::string, std::string>(kArtAppAssetLoc + "patterns/Glitter_blue_2.png", kArtAppAssetLoc + "patterns/Glitter_blue_2t.png");
}

std::string PatternFileStorage::getNormalFileByName(const std::string &name)
{
    try{
        std::string value = _fileStore.at(name).first;
        return value;
    }
    catch(std::out_of_range){
        return "";
    }
}

std::string PatternFileStorage::getTransparantFileByName(const std::string &name)
{
    try{
        std::string value = _fileStore.at(name).second;
        return value;
    }
    catch(std::out_of_range){
        return "";
    }
}

NS_AZOOMEE_AA_END
