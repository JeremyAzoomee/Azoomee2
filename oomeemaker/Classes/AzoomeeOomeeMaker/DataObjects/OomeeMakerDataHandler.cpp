//
//  OomeeMakerDataHandler.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 15/02/2018.
//

#include "OomeeMakerDataHandler.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include "../UI/OomeeSelectScene.h"

using namespace cocos2d;

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
    if(!_dataStorage->getInstance()->_initialised)
    {
        ModalMessages::getInstance()->startLoading();
        removeExistingAssets();
        unzipBundledAssets();
    }
}

void OomeeMakerDataHandler::removeExistingAssets()
{
    if(FileUtils::getInstance()->isDirectoryExist(getAssetDir() + "assets"))
    {
        FileUtils::getInstance()->removeDirectory(getAssetDir() + "assets");
    }
    
    if(FileUtils::getInstance()->isDirectoryExist(getAssetDir() + "configs"))
    {
        FileUtils::getInstance()->removeDirectory(getAssetDir() + "configs");
    }
}

void OomeeMakerDataHandler::getLatestData(const OnCompleteCallback& callback)
{
#ifdef STANDALONE_APP
	getConfigFilesIfNeeded();
	return;
#endif
	if(callback)
	{
		_callback = callback;
	}
	ModalMessages::getInstance()->startLoading();
	HttpRequestCreator* request = API::GetOomeeMakerAssets(ChildDataProvider::getInstance()->getParentOrChildId(), this);
	request->execute();
}

void OomeeMakerDataHandler::parseOomeeData()
{
	const std::vector<std::string>& oomeeConfigs = DirectorySearcher::getInstance()->getJsonFilesInDirectory(getAssetDir() + "configs/oomees/");
    for(const std::string& filename : oomeeConfigs)
    {
        rapidjson::Document oomeeConfig;
        oomeeConfig.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile(getAssetDir() + "configs/oomees/" + filename).c_str());
        if(!oomeeConfig.HasParseError())
        {
			
			if(oomeeConfig.HasMember("oomees") && oomeeConfig["oomees"].IsArray())
			{
				for (int i = 0; i < oomeeConfig["oomees"].Size(); i++)
				{
					const OomeeRef& oomee = Oomee::createWithData(oomeeConfig["oomees"][i]);
					_dataStorage->addOomee(oomee);
					if(getBoolFromJson("default", oomeeConfig["oomees"][i]))
					{
						_dataStorage->setDefaultOomeeId(oomee->getId());
					}
				}
			}
        }
    }
}

void OomeeMakerDataHandler::parseCategoryData()
{
    const std::vector<std::string>& catConfigs = DirectorySearcher::getInstance()->getJsonFilesInDirectory(getAssetDir() + "configs/categories/");
    for(const std::string& filename : catConfigs)
    {
        rapidjson::Document catConfig;
        catConfig.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile(getAssetDir() + "configs/categories/" + filename).c_str());
        if(!catConfig.HasParseError())
        {
			if(catConfig.HasMember("categories") && catConfig["categories"].IsArray())
			{
				for (int i = 0; i < catConfig["categories"].Size(); i++)
				{
					const ItemCategoryRef& category = ItemCategory::createWithData(catConfig["categories"][i]);
					_dataStorage->addItemCategory(category);
					if(getBoolFromJson("default", catConfig["categories"][i]))
					{
						_dataStorage->setDefaultCategoryId(category->getId());
					}
				}
			}
        }
    }
}

void OomeeMakerDataHandler::parseOomeeItemData()
{
    const std::vector<std::string>& itemConfigs = DirectorySearcher::getInstance()->getJsonFilesInDirectory(getAssetDir() + "configs/items/");
    for(const std::string& filename : itemConfigs)
    {
        rapidjson::Document itemConfig;
        itemConfig.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile(getAssetDir() + "configs/items/" + filename).c_str());
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

void OomeeMakerDataHandler::unzipBundledAssets()
{
    const std::string& basePath = getAssetDir();
    const std::string& targetPath = "res/oomeeMaker/oomeeMakerAssets.zip";
    const std::string& destinationPath = basePath + "oomeeMakerAssets.zip";
    const std::string& zipFile = FileUtils::getInstance()->getStringFromFile(targetPath);
    
    FileUtils::getInstance()->writeStringToFile(zipFile, destinationPath);
    
    FileZipUtil::getInstance()->asyncUnzip(destinationPath,basePath, "", this);
}

std::string OomeeMakerDataHandler::getFullSaveDir() const
{
    const std::string& assetDir = getAssetDir();
#ifdef STANDALONE_APP
    const std::string& searchDir =  assetDir + "user/";
#else
    const std::string& searchDir =  assetDir + ChildDataProvider::getInstance()->getParentOrChildId() + "/";
#endif
    
    if(!FileUtils::getInstance()->isDirectoryExist(searchDir))
    {
        FileUtils::getInstance()->createDirectory(searchDir);
    }
    
    return searchDir;
}

std::string OomeeMakerDataHandler::getLocalSaveDir() const
{
#ifdef STANDALONE_APP
    return kBaseFolderName + "user/";
#else
    return kBaseFolderName + ChildDataProvider::getInstance()->getParentOrChildId() + "/";
#endif
}

std::string OomeeMakerDataHandler::getAssetDir() const
{
    const std::string& assetDir = FileUtils::getInstance()->getWritablePath() + kBaseFolderName;
    
    if(!FileUtils::getInstance()->isDirectoryExist(assetDir))
    {
        FileUtils::getInstance()->createDirectory(assetDir);
    }
    
    return assetDir;
}

bool OomeeMakerDataHandler::deleteOomee(const std::string& oomeeName)
{
    const std::string& fullOomeeName = getFullSaveDir() + oomeeName + ".oomee";
    const std::string& fullImageName = getFullSaveDir() + oomeeName + ".png";
    
    if(FileUtils::getInstance()->isFileExist(fullOomeeName))
    {
        FileUtils::getInstance()->removeFile(fullOomeeName);
        FileUtils::getInstance()->removeFile(fullImageName);
        return true;
    }
    return false;
}

std::string OomeeMakerDataHandler::getCachePath() const
{
	return kBaseFolderName;
}

void OomeeMakerDataHandler::loadLocalData()
{
	if(FileUtils::getInstance()->isDirectoryExist(getAssetDir() + "configs"))
	{
		_dataStorage->clearAllData();
		parseOomeeData();
		parseCategoryData();
		parseOomeeItemData();
		_dataStorage->_initialised = true;
		
		if(!UserDefault::getInstance()->getBoolForKey("updatedExitsingOomeeFile", false))
		{
			updateExistingOomeeFilesToNewIds();
			UserDefault::getInstance()->setBoolForKey("updatedExitsingOomeeFile", true);
		}
		
		ModalMessages::getInstance()->stopLoading();
		
		sendCallback(true);
	}
	else
	{
		unzipBundledAssets();
	}
}

void OomeeMakerDataHandler::updateExistingOomeeFilesToNewIds()
{
	const std::string& lookupFilename = "res/oomeeMaker/idConvLookup.json";
	rapidjson::Document lookupJson;
	lookupJson.Parse(FileUtils::getInstance()->getStringFromFile(lookupFilename).c_str());
	if(lookupJson.HasParseError())
	{
		return;
	}
	const std::map<std::string, std::string> lookupMap = getStringMapFromJson(lookupJson);
	
	const auto& dirFolders = DirectorySearcher::getInstance()->getFoldersInDirectory(getAssetDir());
	for(const auto& folder : dirFolders)
	{
		const auto& oomeeFiles = DirectorySearcher::getInstance()->getFilesInDirectoryWithExtention(getAssetDir() + folder, ".oomee");
		for(const auto& file : oomeeFiles)
		{
			rapidjson::Document data;
			data.Parse(FileUtils::getInstance()->getStringFromFile(getAssetDir() + folder + "/" + file).c_str());
			if(data.HasParseError())
			{
				continue;
			}
			
			const auto& lookupFunc = [lookupMap](const std::string& key){
				if(lookupMap.find(key) != lookupMap.end())
				{
					return lookupMap.at(key);
				}
				return key;
			};
			
			const std::string& oomeeKey = getStringFromJson("oomee", data);
			const std::string& newOomeeKey = lookupFunc(oomeeKey);
			
			std::vector<std::string> newItemKeys;
			if(data.HasMember("oomeeItems"))
			{
				const std::vector<std::string>& itemKeys = getStringArrayFromJson(data["oomeeItems"]);
				for(std::string key : itemKeys)
				{
					newItemKeys.push_back(lookupFunc(key));
				}
			}
			
			std::string savedFileContent = "{";
			savedFileContent += StringUtils::format("\"oomee\":\"%s\",", newOomeeKey.c_str());
			savedFileContent += "\"oomeeItems\":[";
			for(int i = 0; i < newItemKeys.size(); i++)
			{
				savedFileContent += StringUtils::format("\"%s\"",newItemKeys.at(i).c_str());
				if(i < newItemKeys.size() - 1)
				{
					savedFileContent += ",";
				}
			}
			savedFileContent += "]}";
			
			FileUtils::getInstance()->writeStringToFile(savedFileContent, getAssetDir() + folder + "/" + file);
		}
	}
}

// Delegate functions
void OomeeMakerDataHandler::onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath)
{
    FileUtils::getInstance()->removeFile(zipPath);
	loadLocalData();
}

void OomeeMakerDataHandler::onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode)
{
	if(responseCode == 200)
	{
		setLocalEtag(_fileDownloader->getEtag());
		_fileDownloader = nullptr;
		const std::string& dirPath = getAssetDir();
		const std::string& zipPath = dirPath + "oomeeMakerAssets.zip";
		cocos2d::FileUtils::getInstance()->writeStringToFile(fileString, zipPath);
		removeExistingAssets();
		FileZipUtil::getInstance()->asyncUnzip(zipPath, dirPath, "", this);
	}
	else
	{
		if(!_dataStorage->_initialised)
		{
			loadLocalData();
		}
		else
		{
			ModalMessages::getInstance()->stopLoading();
			sendCallback(false);
		}
	}
}

void OomeeMakerDataHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	rapidjson::Document result;
	result.Parse(body.c_str());
	if(result.HasParseError())
	{
		return;
	}
	
	const std::string& zipUrl = getStringFromJson("configMetaUrl", result);
	
	downloadFile(zipUrl);
}
void OomeeMakerDataHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	if(!_dataStorage->_initialised)
	{
		loadLocalData();
	}
	else
	{
		ModalMessages::getInstance()->stopLoading();
		sendCallback(false);
	}
}

NS_AZOOMEE_OM_END
