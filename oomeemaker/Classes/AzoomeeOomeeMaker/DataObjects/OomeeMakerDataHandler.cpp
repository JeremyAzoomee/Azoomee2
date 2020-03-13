//
//  OomeeMakerDataHandler.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 15/02/2018.
//

#include "OomeeMakerDataHandler.h"
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Utils/DirUtil.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "../UI/OomeeSelectScene.h"

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

const std::string OomeeMakerDataHandler::kSaveNewOomeeEventName = "azoomee.oomeemaker.savenewoomee";
const std::string OomeeMakerDataHandler::kOomeeFileExtension = ".oomee";

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
    // make sure asset dir for OM assets is created
    getAssetDir();
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
    if(_gettingDataAsync)
    {
        _gettingDataAsync = false;
    }
    else
    {
        HttpRequestCreator* request = API::GetOomeeMakerAssets(ChildManager::getInstance()->getLoggedInChild()->getId(), this);
        request->execute();
    }
}

void OomeeMakerDataHandler::getLatestDataAsync(const OnCompleteCallback& callback)
{
#ifdef STANDALONE_APP
    getConfigFilesIfNeeded();
    return;
#endif
    if(callback)
    {
        _callback = callback;
    }
    if(!_gettingDataAsync)
    {
        _gettingDataAsync = true;
        HttpRequestCreator* request = API::GetOomeeMakerAssets(ChildManager::getInstance()->getLoggedInChild()->getId(), this);
        request->execute();
    }
}

void OomeeMakerDataHandler::getOomeesForChild(const std::string &childId, bool getOnlySelected, const OnCompleteCallback& callback)
{
	if(callback)
	{
		_callback = callback;
	}
    
	_targetChildId = childId;
    const std::string& uploadLocalKey = "uploadedLocalOomees-" + childId;
    if(UserDefault::getInstance()->getBoolForKey(uploadLocalKey.c_str(), false))
    {
        ModalMessages::getInstance()->startLoading();
        HttpRequestCreator* request = API::GetChildOomees(childId, getOnlySelected, this);
        request->execute();
    }
    else
    {
        UserDefault::getInstance()->setBoolForKey(uploadLocalKey.c_str(), true);
        uploadExistingOomeesToBE(childId);
    }
}

void OomeeMakerDataHandler::getAllOomees(const OnCompleteCallback& callback)
{
	if(callback)
	{
		_callback = callback;
	}
    _targetChildId = "";
	ModalMessages::getInstance()->startLoading();
	HttpRequestCreator* request = API::GetAllOomees(UserAccountManager::getInstance()->getLoggedInParentId(), false, this);
	request->execute();
}

void OomeeMakerDataHandler::saveOomee(const OomeeFigureDataRef& oomee, bool setAsAvatar, const std::string& childId, const OnCompleteCallback& callback)
{
	if(callback)
	{
		_callback = callback;
	}
	_targetChildId = childId;
	ModalMessages::getInstance()->startLoading();
    _savingNewOomee = oomee->getId() == "";
	if(_savingNewOomee)
	{
		HttpRequestCreator* request = API::SaveNewOomee(childId, UserAccountManager::getInstance()->getLoggedInParentId(), oomee->getOomeeId(), oomee->getAccessoryIds(), setAsAvatar, this);
		request->execute();
	}
	else
	{
		HttpRequestCreator* request = API::UpdateChildOomee(childId, oomee->getId(), UserAccountManager::getInstance()->getLoggedInParentId(), oomee->getOomeeId(), oomee->getAccessoryIds(), setAsAvatar, this);
		request->execute();
	}
}

void OomeeMakerDataHandler::deleteOomee(const OomeeFigureDataRef &oomee, const std::string &childId, const OnCompleteCallback& callback)
{
	if(callback)
	{
		_callback = callback;
	}
	_targetChildId = childId;
	ModalMessages::getInstance()->startLoading();
	HttpRequestCreator* request = API::DeleteChildOomee(childId, oomee->getId(), this);
	request->execute();
}

void OomeeMakerDataHandler::uploadLocalOomeesToBE(const std::string &childId, const OnCompleteCallback& callback)
{
	if(callback)
	{
		_callback = callback;
	}
	_targetChildId = childId;
	uploadExistingOomeesToBE(childId);

}

void OomeeMakerDataHandler::parseOomeeData()
{
	const std::vector<std::string>& oomeeConfigs = DirUtil::getJsonFilesInDirectory(getAssetDir() + "configs/oomees/");
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
					if(getBoolFromJson("isDefault", oomeeConfig["oomees"][i]))
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
    const std::vector<std::string>& catConfigs = DirUtil::getJsonFilesInDirectory(getAssetDir() + "configs/categories/");
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
					if(getBoolFromJson("isDefault", catConfig["categories"][i]))
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
    const std::vector<std::string>& itemConfigs = DirUtil::getJsonFilesInDirectory(getAssetDir() + "configs/items/");
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
    const std::string& searchDir =  assetDir + ChildManager::getInstance()->getLoggedInChild()->getId() + "/";
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
    return kBaseFolderName + ChildManager::getInstance()->getLoggedInChild()->getId() + "/";
#endif
}

std::string OomeeMakerDataHandler::getAssetDir() const
{
    const std::string& assetDir = DirUtil::getCachesPath() + kBaseFolderName;
    
    if(!FileUtils::getInstance()->isDirectoryExist(assetDir))
    {
        FileUtils::getInstance()->createDirectory(assetDir);
    }
    
    return assetDir;
}

bool OomeeMakerDataHandler::deleteOomee(const std::string& oomeeName)
{
    const std::string& fullOomeeName = getFullSaveDir() + oomeeName + kOomeeFileExtension;
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
		
        if(_gettingDataAsync)
        {
            _gettingDataAsync = false;
        }
        else
        {
            ModalMessages::getInstance()->stopLoading();
        }
		
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
	
	const auto& dirFolders = DirUtil::getFoldersInDirectory(getAssetDir());
	for(const auto& folder : dirFolders)
	{
		const auto& oomeeFiles = DirUtil::getFilesInDirectoryWithExtention(getAssetDir() + folder, kOomeeFileExtension);
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

void OomeeMakerDataHandler::uploadExistingOomeesToBE(const std::string& childId)
{
    const std::string& parentId = UserAccountManager::getInstance()->getLoggedInParentId();
    const std::string& oomeeDir = getAssetDir() + childId;
	const auto& oomeeFiles = DirUtil::getFilesInDirectoryWithExtention(oomeeDir, kOomeeFileExtension);
	for(const auto& file : oomeeFiles)
	{
		rapidjson::Document data;
		data.Parse(FileUtils::getInstance()->getStringFromFile(oomeeDir + "/" + file).c_str());
		if(data.HasParseError())
		{
			continue;
		}
		OomeeFigureDataRef oomee = OomeeFigureData::createWithData(data);
		if(oomee->getId() == "")
		{
			HttpRequestCreator* request = API::SaveNewOomee(childId, parentId, oomee->getOomeeId(), oomee->getAccessoryIds(), (_pendingLocalOomeeUploads.size() == 0), this);
            request->_requestTag = API::TagSaveLocalOomee;
			_pendingLocalOomeeUploads.push_back(request);
			deleteOomee(file.substr(0,file.size() - kOomeeFileExtension.length()));
		}
	}
	if(_pendingLocalOomeeUploads.size() > 0)
	{
        ModalMessages::getInstance()->startLoading();
		HttpRequestCreator* request = _pendingLocalOomeeUploads.back();
		_pendingLocalOomeeUploads.pop_back();
		request->execute();
	}
	else
	{
		HttpRequestCreator* request = API::GetChildOomees(childId, false, this);
		request->execute();
	}
}

void OomeeMakerDataHandler::writeOomeeFiles(const rapidjson::Value& data)
{
    std::vector<std::string> oomeeIds;
    std::vector<std::string> childIds;
    for(int i = 0; i < data.Size(); i++)
    {
        const rapidjson::Value& figureData = data[i];
        OomeeFigureDataRef figure = OomeeFigureData::createWithData(figureData);
        if(figure->getId() == "")
        {
            continue;
        }
        const std::string& childId = getStringFromJson("childId", figureData);
        if(figure->isSelected())
        {
            UserAccountManager::getInstance()->setAvatarColourForChild(childId, getColor4BFromJson("colour", figureData));
        }
        oomeeIds.push_back(figure->getId());
        if(std::find(childIds.begin(), childIds.end(), childId) == childIds.end())
        {
            childIds.push_back(childId);
        }
        
        const std::string& filePath = getAssetDir() + childId + "/" + figure->getId() + kOomeeFileExtension;
        if(FileUtils::getInstance()->isDirectoryExist(getAssetDir() + childId))
        {
            if(FileUtils::getInstance()->isFileExist(filePath))
            {
                rapidjson::Document oldData;
                oldData.Parse(FileUtils::getInstance()->getStringFromFile(filePath).c_str());
                if(!oldData.HasParseError())
                {
                    OomeeFigureDataRef oldFigure = OomeeFigureData::createWithData(oldData);
                    if(figure->isEqual(oldFigure)  && FileUtils::getInstance()->isFileExist(getAssetDir() + childId + "/" + figure->getId() + ".png"))
                    {
                        continue;
                    }
                }
            }
        }
        else
        {
            FileUtils::getInstance()->createDirectory(getAssetDir() + childId);
        }
        
        std::string savedFileContent = "{";
        savedFileContent += StringUtils::format("\"id\":\"%s\",", figure->getId().c_str());
        savedFileContent += StringUtils::format("\"oomeeId\":\"%s\",", figure->getOomeeId().c_str());
        savedFileContent += StringUtils::format("\"selected\":%s,", figure->isSelected() ? "true" : "false");
        savedFileContent += "\"oomeeItems\":[";
        const std::vector<std::string>& accIds = figure->getAccessoryIds();
        for(int i = 0; i < accIds.size(); i++)
        {
            savedFileContent += StringUtils::format("\"%s\"",accIds.at(i).c_str());
            if(i < accIds.size() - 1)
            {
                savedFileContent += ",";
            }
        }
        savedFileContent += "]}";
        
        FileUtils::getInstance()->writeStringToFile(savedFileContent, filePath);
        if(_dataStorage->_initialised)
        {
            OomeeFigure* oomee = OomeeFigure::create();
            oomee->initWithOomeeFigureData(figure);
            oomee->saveSnapshotImage(kBaseFolderName + childId + "/" + figure->getId() + ".png");
        }
    }
    // trim out any deleted oomees
    for(const std::string& childId : childIds)
    {
        auto childOomees = DirUtil::getFilesInDirectoryWithExtention(getAssetDir() + childId , kOomeeFileExtension);
        for(const std::string& oomeeId : childOomees)
        {
            const std::string& truncId = oomeeId.substr(0, oomeeId.length() - kOomeeFileExtension.length());
            if(std::find(oomeeIds.begin(), oomeeIds.end(), truncId) == oomeeIds.end())
            {
                deleteOomee(truncId);
            }
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
        _dataStorage->_initialised = false;
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
            if(_gettingDataAsync)
            {
                _gettingDataAsync = false;
            }
            else
            {
                ModalMessages::getInstance()->stopLoading();
            }
			sendCallback(false);
		}
	}
}

void OomeeMakerDataHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagGetOomeeMakerAssets)
	{
		rapidjson::Document result;
		result.Parse(body.c_str());
		if(result.HasParseError())
		{
			if(!_dataStorage->_initialised)
			{
				loadLocalData();
			}
			else
			{
                if(_gettingDataAsync)
                {
                    _gettingDataAsync = false;
                }
                else
                {
                    ModalMessages::getInstance()->stopLoading();
                }
				sendCallback(false);
			}
			return;
		}
		
		const std::string& zipUrl = getStringFromJson("configMetaUrl", result);
		
		downloadFile(zipUrl);
	}
	else if(requestTag == API::TagGetChildOomees || requestTag == API::TagGetAllOomees)
	{
		ModalMessages::getInstance()->stopLoading();
		rapidjson::Document result;
		result.Parse(body.c_str());
		if(result.HasParseError() || !result.IsArray())
		{
			sendCallback(false);
			return;
		}
		
		// write files to cache - temp solution, maybe
        if(result.Size() > 0 || _targetChildId == "")
        {
            writeOomeeFiles(result);
            sendCallback(true);
        }
        else
        {
            rapidjson::Document defaultOomeeData;
            defaultOomeeData.Parse(FileUtils::getInstance()->getStringFromFile("res/defaultOomees/oomee_11.oomee").c_str());
            OomeeFigureDataRef defaultOomee = OomeeFigureData::createWithData(defaultOomeeData);
            saveOomee(defaultOomee, true, _targetChildId);
        }
	}
	else if(requestTag == API::TagSaveNewOomee || requestTag == API::TagUpdateChildOomee)
	{
        if(_savingNewOomee)
        {
            _savingNewOomee = false;
            auto locationHeader = StringFunctions::getValueFromHttpResponseHeaderForKey("Location", headers);
            std::string oomeeId = StringFunctions::splitStringToVector(locationHeader, "/").back();
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(kSaveNewOomeeEventName, &oomeeId);
        }
        
		HttpRequestCreator* request = API::GetChildOomees(_targetChildId, false, this);
		request->execute();
	}
	else if(requestTag == API::TagDeleteChildOomee)
	{
		HttpRequestCreator* request = API::GetChildOomees(_targetChildId, false, this);
		request->execute();
	}
    else if(requestTag == API::TagSaveLocalOomee)
	{
		if(_pendingLocalOomeeUploads.size() > 0)
		{
			HttpRequestCreator* request = _pendingLocalOomeeUploads.back();
			_pendingLocalOomeeUploads.pop_back();
			request->execute();
		}
		else
		{
			HttpRequestCreator* request = API::GetChildOomees(_targetChildId, false, this);
			request->execute();
		}
	}
}
void OomeeMakerDataHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	if(requestTag == API::TagGetOomeeMakerAssets && !_dataStorage->_initialised)
	{
		loadLocalData();
	}
    else if(requestTag == API::TagSaveLocalOomee)
	{
		if(_pendingLocalOomeeUploads.size() > 0)
		{
			HttpRequestCreator* request = _pendingLocalOomeeUploads.back();
			_pendingLocalOomeeUploads.pop_back();
			request->execute();
		}
		else
		{
			HttpRequestCreator* request = API::GetChildOomees(_targetChildId, false, this);
			request->execute();
		}
	}
	else
	{
		ModalMessages::getInstance()->stopLoading();
		sendCallback(false);
	}
}

NS_AZOOMEE_OM_END
