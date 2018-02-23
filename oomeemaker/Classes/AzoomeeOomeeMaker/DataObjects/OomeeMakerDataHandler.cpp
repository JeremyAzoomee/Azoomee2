//
//  OomeeMakerDataHandler.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 15/02/2018.
//

#include "OomeeMakerDataHandler.h"

NS_AZOOMEE_OM_BEGIN

static std::auto_ptr<OomeeMakerDataHandler> sOomeeMakerDataHandlerSharedInstance;

OomeeMakerDataHandler* OomeeMakerDataHandler::getInstance()
{
    if(!sOomeeMakerDataHandlerSharedInstance.get())
    {
        sOomeeMakerDataHandlerSharedInstance.reset(new OomeeMakerDataHandler());
        sOomeeMakerDataHandlerSharedInstance->init();
    }
    return sOomeeMakerDataHandlerSharedInstance.get();
}

OomeeMakerDataHandler::~OomeeMakerDataHandler(void)
{
    
}

void OomeeMakerDataHandler::init()
{
    _dataStorage = OomeeMakerDataStorage::getInstance();
}

void OomeeMakerDataHandler::getConfigFilesIfNeeded()
{
    OomeeMakerDataStorage::getInstance()->clearAllData();
    parseOomeeData();
    parseCategoryData();
    parseOomeeItemData();
}

void OomeeMakerDataHandler::getPackageJson()
{
    
}

void OomeeMakerDataHandler::getConfigDataZip()
{
    
}


void OomeeMakerDataHandler::parseOomeeData()
{
    const std::vector<std::string>& oomeeConfigs = getConfigFilesForType("oomeeConfigs");
    for(const std::string& filename : oomeeConfigs)
    {
        rapidjson::Document oomeeConfig;
        oomeeConfig.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile(filename).c_str());
        if(!oomeeConfig.HasParseError())
        {
            const OomeeRef& oomee = Oomee::createWithData(oomeeConfig);
            _dataStorage->addOomee(oomee);
        }
    }
}

void OomeeMakerDataHandler::parseCategoryData()
{
    const std::vector<std::string>& catConfigs = getConfigFilesForType("categoryConfigs");
    for(const std::string& filename : catConfigs)
    {
        rapidjson::Document catConfig;
        catConfig.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile(filename).c_str());
        if(!catConfig.HasParseError())
        {
            const ItemCategoryRef& category = ItemCategory::createWithData(catConfig);
            _dataStorage->addItemCategory(category);
        }
    }
}

void OomeeMakerDataHandler::parseOomeeItemData()
{
    const std::vector<std::string>& itemConfigs = getConfigFilesForType("itemConfigs");
    for(const std::string& filename : itemConfigs)
    {
        rapidjson::Document itemConfig;
        itemConfig.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile(filename).c_str());
        if(!itemConfig.HasParseError())
        {
            if(itemConfig.HasMember("items") && itemConfig["items"].IsArray())
            {
                for (int i = 0; i < itemConfig["items"].Size(); i++)
                {
                    const OomeeItemRef& item = OomeeItem::createWithData(itemConfig["items"][i]);
                    _dataStorage->addOomeeItem(item);
                }
            }
            
        }
    }
}

std::vector<std::string> OomeeMakerDataHandler::getConfigFilesForType(const std::string& listType) const
{
    rapidjson::Document configFilesList;
    configFilesList.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile("res/oomeeMaker/configFileLists.json").c_str());
    if(configFilesList.HasParseError())
    {
        return std::vector<std::string>();
    }
    
    return getStringArrayFromJson(configFilesList[listType.c_str()]);
}

// Delegate functions
void OomeeMakerDataHandler::onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath)
{
    
}

void OomeeMakerDataHandler::onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode)
{
    
}

NS_AZOOMEE_OM_END
