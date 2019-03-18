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

//void OomeeMakerDataHandler::parseOomeeColourData()
//{
//   const std::vector<std::string>& colourConfigs = getConfigFilesForType("colourConfigs");
//    for(const std::string& filename : colourConfigs)
//    {
//        rapidjson::Document itemConfig;
//        itemConfig.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile(getAssetDir() + filename).c_str());
//        if(!itemConfig.HasParseError())
//        {
//            if(itemConfig.HasMember("items") && itemConfig["items"].IsArray())
//            {
//                for (int i = 0; i < itemConfig["items"].Size(); i++)
//                {
//                    const OomeeColourRef& item = OomeeColour::createWithData(itemConfig["items"][i]);
//                    _dataStorage->addColour(item);
//                }
//            }
//
//        }
//   }
//}

std::vector<std::string> OomeeMakerDataHandler::getConfigFilesForType(const std::string& listType) const
{
    rapidjson::Document configFilesList;
    configFilesList.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile(getAssetDir() + "configs/configFileLists.json").c_str());
    if(configFilesList.HasParseError())
    {
        return std::vector<std::string>();
    }
    
    return getStringArrayFromJson(configFilesList[listType.c_str()]);
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
		ModalMessages::getInstance()->stopLoading();
		_dataStorage->clearAllData();
		parseOomeeData();
		parseCategoryData();
		parseOomeeItemData();
		_dataStorage->_initialised = true;
		sendCallback(true);
	}
	else
	{
		unzipBundledAssets();
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
