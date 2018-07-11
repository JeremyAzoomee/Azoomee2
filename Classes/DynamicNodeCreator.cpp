//
//  DynamicNodeCreator.cpp
//  azoomee2
//
//  Created by Macauley on 21/09/2017.
//
//

#include "DynamicNodeCreator.h"
#include <AzoomeeCommon/UI/Style.h>
#include "DynamicNodeButtonListener.h"
#include "ButtonActionData.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "HQDataProvider.h"
#include "DynamicNodeTextInput.h"
#include "DynamicNodeButton.h"
#include "DynamicNodeText.h"
#include "DynamicNodeLine.h"
#include "DynamicNodeCheckBox.h"
#include <AzoomeeCommon/UI/Scene.h>
#include "DynamicNodeHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<DynamicNodeCreator> sDynamicNodeCreatorSharedInstance;

const std::string DynamicNodeCreator::kCTAAssetLoc = "res/cta_assets/";
const std::string DynamicNodeCreator::kCTABundleImageLoc = "res/cta_assets/cta_bundle/images/";
const std::string DynamicNodeCreator::kCTADeviceImageCacheLoc = "DCDECache/images/";

const std::string DynamicNodeCreator::kPortraitPrefix = "/p_";

DynamicNodeCreator* DynamicNodeCreator::getInstance()
{
    if(!sDynamicNodeCreatorSharedInstance.get())
    {
        sDynamicNodeCreatorSharedInstance.reset(new DynamicNodeCreator());
    }
    return sDynamicNodeCreatorSharedInstance.get();
}

DynamicNodeCreator::~DynamicNodeCreator(void)
{
    
}

bool DynamicNodeCreator::init(void)
{
    return true;
}

Node* DynamicNodeCreator::createCTAFromFile(const std::string& filepath)
{
    _usingExternalParams = false;
    //Init CTA objects
    initCTANode();
    std::string configFilePath = filepath;
    _portraitMode = false;
    if(Director::getInstance()->getVisibleSize().width < Director::getInstance()->getVisibleSize().height)
    {
        auto stringVec = splitStringToVector(filepath, "/");
        std::string filename = stringVec.back();
        stringVec.pop_back();
        const std::string& path = joinStrings(stringVec, "/");
        if(FileUtils::getInstance()->isFileExist(path + kPortraitPrefix + filename))
        {
            configFilePath = path + kPortraitPrefix + filename;
            _portraitMode = true;
        }
    }
    
    const std::string& jsonString = FileUtils::getInstance()->getStringFromFile(configFilePath);
    
    rapidjson::Document configFile;
    configFile.Parse(jsonString.c_str());
    
    if(configFile.HasParseError())
    {
        return _CTANode;
    }
    
    processFile(configFile);
    
    //return resultant CTA node
    return _CTANode;
    
}

Node* DynamicNodeCreator::createCTAFromFileWithParams(const std::string& filepath, const std::string& params)
{
    _externParams.Parse(params.c_str());
    _usingExternalParams = !_externParams.HasParseError();
    
    //Init CTA objects
    initCTANode();
    
    std::string configFilePath = filepath;
    _portraitMode = false;
    if(Director::getInstance()->getVisibleSize().width < Director::getInstance()->getVisibleSize().height)
    {
        auto stringVec = splitStringToVector(filepath, "/");
        std::string filename = stringVec.back();
        stringVec.pop_back();
        const std::string& path = joinStrings(stringVec, "/");
        if(FileUtils::getInstance()->isFileExist(path + kPortraitPrefix + filename))
        {
            configFilePath = path + kPortraitPrefix + filename;
            _portraitMode = true;
        }
    }
    
    const std::string& jsonString = FileUtils::getInstance()->getStringFromFile(configFilePath);
    
    rapidjson::Document configFile;
    configFile.Parse(jsonString.c_str());
    
    if(configFile.HasParseError())
    {
        return _CTANode;
    }
    
    processFile(configFile);
    
    //return resultant CTA node
    return _CTANode;
    
}

void DynamicNodeCreator::processFile(const rapidjson::Document& configFile)
{
    /*
     values not used:
     nodeId, groupId
     these are specific for asset gathering and storing purposes, not needed in theis viewer
     */
    //config node size
    if(configFile.HasMember("nodeSize"))
    {
        const rapidjson::Value& sizePercentages = configFile["nodeSize"];
        configNodeSize(sizePercentages);
    }
    
    //config close button
    if(configFile.HasMember("closeButton"))
    {
        const rapidjson::Value& closeButtonToggle = configFile["closeButton"];
        configCloseButton(closeButtonToggle);
    }
    
    //config background colour
    if(configFile.HasMember("backgroundColour"))
    {
        const rapidjson::Value& backgroundColour = configFile["backgroundColour"];
        configBackgroundColour(backgroundColour);
    }
    
    //config background image
    if(configFile.HasMember("backgroundImage"))
    {
        const rapidjson::Value& backgroundImageData = configFile["backgroundImage"];
        configBackgroundImage(backgroundImageData);
    }
    
    //config text
    if(configFile.HasMember("nodeText"))
    {
        const rapidjson::Value& textData = configFile["nodeText"];
        configText(textData);
    }
    
    //config buttons
    
    if(configFile.HasMember("buttons"))
    {
        const rapidjson::Value& buttonsList = configFile["buttons"];
        configButtons(buttonsList);
    }
    
    //config extra images
    if(configFile.HasMember("images"))
    {
        const rapidjson::Value& imageList = configFile["images"];
        configExtraImages(imageList);
    }
    
    //config text input fields
    if(configFile.HasMember("textInputFields"))
    {
        const rapidjson::Value& textInputList = configFile["textInputFields"];
        configTextInput(textInputList);
    }
    
    //config line drawing
    if(configFile.HasMember("lines"))
    {
        const rapidjson::Value& lineList = configFile["lines"];
        configLines(lineList);
    }
    
    //config check boxes drawing
    if(configFile.HasMember("checkBoxes"))
    {
        const rapidjson::Value& checkBoxList = configFile["checkBoxes"];
        configCheckBoxes(checkBoxList);
    }
}

void DynamicNodeCreator::initCTANode()
{
    
    _windowSize = Director::getInstance()->getWinSize();
    
    if(_CTANode)
    {
        _CTANode->removeFromParent();
    }
    
    _CTANode = Node::create();
    
    _CTANode->setOnExitCallback([=](){
        _CTANode = nullptr;
    });
    
    LayerColor* overlay = LayerColor::create(Style::Color_4B::semiTransparentOverlayDark, _windowSize.width, _windowSize.height);
    overlay->setName("overlay");
    _CTANode->addChild(overlay);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true; //block touches
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), overlay);
    
    if(dynamic_cast<Azoomee::Scene*>(Director::getInstance()->getRunningScene()))
    {
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        _CTANode->setPosition(-origin);
    }
    
    _stencil = ui::Scale9Sprite::create(kCTAAssetLoc + "deep_free_pop_over.png");
    _stencil->setContentSize(Size(_windowSize.width*0.75,_windowSize.height*0.67));
    
    _clippingNode = ClippingNode::create(_stencil);
    
    _bgColour = LayerColor::create(Color4B::WHITE, _windowSize.width, _windowSize.height);
    _bgColour->setPosition(Vec2(_windowSize*-0.5f));
    _clippingNode->addChild(_bgColour);
    
    _clippingNode->setAlphaThreshold(0.5f);
    _clippingNode->setPosition(_windowSize/2);
    _CTANode->addChild(_clippingNode);
    
    _popupImages = Node::create();
    _popupImages->setContentSize(_stencil->getContentSize());
    _popupImages->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _popupImages->setPosition(_windowSize/2);
    _CTANode->addChild(_popupImages);
    
    _linesLayer = Node::create();
    _linesLayer->setContentSize(_stencil->getContentSize());
    _linesLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _linesLayer->setPosition(_windowSize/2);
    _CTANode->addChild(_linesLayer);
    
    _popupButtonsLayer = Node::create();
    _popupButtonsLayer->setContentSize(_stencil->getContentSize());
    _popupButtonsLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _popupButtonsLayer->setPosition(_windowSize/2);
    _CTANode->addChild(_popupButtonsLayer);
    
    _textInputLayer = Node::create();
    _textInputLayer->setContentSize(_stencil->getContentSize());
    _textInputLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _textInputLayer->setPosition(_windowSize/2);
    _CTANode->addChild(_textInputLayer);
    
    _checkBoxLayer = Node::create();
    _checkBoxLayer->setContentSize(_stencil->getContentSize());
    _checkBoxLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _checkBoxLayer->setPosition(_windowSize/2);
    _CTANode->addChild(_checkBoxLayer);
    
    _textLayer = Node::create();
    _textLayer->setContentSize(_windowSize);
    _textLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _textLayer->setPosition(_windowSize/2);
    _CTANode->addChild(_textLayer);
    
    _popupFrame = ui::Scale9Sprite::create(kCTAAssetLoc + "deep_free_pop_over_trans.png");
    _popupFrame->setPosition(_windowSize/2);
    _popupFrame->setAnchorPoint(Vec2(0.5,0.5));
    _popupFrame->setContentSize(Size(_windowSize.width*0.75,_windowSize.height*0.67));
    _CTANode->addChild(_popupFrame);
    
    _closeButton = ui::Button::create();
    _closeButton->loadTextures(kCTAAssetLoc + "close.png", kCTAAssetLoc + "close.png");
    _closeButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _closeButton->setPosition(_popupFrame->getPosition() + _popupFrame->getContentSize()/2 - _closeButton->getContentSize()*0.75);
    _closeButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            AnalyticsSingleton::getInstance()->ctaButtonPressed("close");
            this->resetCTAPopup();
        }
    });
    _CTANode->addChild(_closeButton);
    
}

void DynamicNodeCreator::configNodeSize(const rapidjson::Value &sizePercentages)
{
    if(sizePercentages.Size() == 2)
    {
        if(sizePercentages[0].IsFloat() && sizePercentages[1].IsFloat())
        {
            Size visibleSizeSafe = Director::getInstance()->getVisibleSize() - Size(50,50);
            if(ConfigStorage::getInstance()->isDeviceIphoneX())
            {
                visibleSizeSafe = visibleSizeSafe - (visibleSizeSafe.width > visibleSizeSafe.height ? Size(200,0) : Size(0,200));
            }
            
            float width = sizePercentages[0].GetFloat()/100.0f;
            float height = sizePercentages[1].GetFloat()/100.0f;
            Size newSize = Size(2736*width,2048*height);
            if(_portraitMode)
            {
                newSize = Size(2048*width,2736*height);
            }
            
            if(newSize.width > visibleSizeSafe.width || newSize.height > visibleSizeSafe.height)
            {
                float widthMod = visibleSizeSafe.width/newSize.width;
                float heightMod = visibleSizeSafe.height/newSize.height;
                _sizeMod = MIN(widthMod, heightMod);
                newSize = newSize * _sizeMod;
            }
            
            _stencil->setContentSize(newSize);
            _popupFrame->setContentSize(newSize);
            _popupButtonsLayer->setContentSize(newSize);
            _popupImages->setContentSize(newSize);
            _textLayer->setContentSize(newSize);
            _textInputLayer->setContentSize(newSize);
            _linesLayer->setContentSize(newSize);
            _checkBoxLayer->setContentSize(newSize);
        }
    }
}

void DynamicNodeCreator::configCloseButton(const rapidjson::Value &closeButtonToggle)
{
    if(closeButtonToggle.IsBool())
    {
        bool showCloseButton = closeButtonToggle.GetBool();
        
        _closeButton->setVisible(showCloseButton);
        _closeButton->setPosition(_popupFrame->getPosition() + _popupFrame->getContentSize()/2 - _closeButton->getContentSize()*0.75);
    }
}

void DynamicNodeCreator::configBackgroundColour(const rapidjson::Value &backgroundColour)
{
    if(backgroundColour.Size() == 3 && backgroundColour[0].IsInt() && backgroundColour[1].IsInt() && backgroundColour[2].IsInt())
    {
        const Color3B& newColour = Color3B(backgroundColour[0].GetInt(), backgroundColour[1].GetInt(), backgroundColour[2].GetInt());
        _bgColour->setColor(newColour);
    }
}

void DynamicNodeCreator::configBackgroundImage(const rapidjson::Value &backgroundImageData)
{
    _maskedBGImage = DynamicNodeImage::create();
    if(_maskedBGImage->initWithParamsAsBGImage(backgroundImageData, _stencil->getContentSize(), _usingExternalParams))
    {
        _clippingNode->addChild(_maskedBGImage);
    }
}

void DynamicNodeCreator::configButtons(const rapidjson::Value &buttonsList)
{
    if(buttonsList.IsArray())
    {
        for (int i = 0; i < buttonsList.Size(); i++)
        {
            DynamicNodeButton* button = DynamicNodeButton::create();
            if(button->initWithParams(buttonsList[i], _popupButtonsLayer->getContentSize(), _usingExternalParams))
            {
                _popupButtonsLayer->addChild(button);
            }
        }
    }
}

void DynamicNodeCreator::configExtraImages(const rapidjson::Value &imageList)
{
    if(imageList.IsArray())
    {
        for (int i = 0; i < imageList.Size(); i++)
        {
            DynamicNodeImage* image = DynamicNodeImage::create();
            if(image->initWithParams(imageList[i], _popupImages->getContentSize(), _usingExternalParams))
            {
                _popupImages->addChild(image);
            }
        }
    }
}

void DynamicNodeCreator::configText(const rapidjson::Value& textConfig)
{
    if(textConfig.HasMember("titleText"))
    {
        const rapidjson::Value& titleText = textConfig["titleText"];
        DynamicNodeText* text = DynamicNodeText::create();
        if(text->initWithParams(DynamicNodeText::kDefaultTitleFontSize, Style::Color_4B::ctaNodeText, titleText, _textLayer->getContentSize(), _usingExternalParams))
        {
            _textLayer->addChild(text);
        }
    }
    
    if(textConfig.HasMember("bodyText"))
    {
        for(int i = 0; i < textConfig["bodyText"].Size(); i++)
        {
            const rapidjson::Value& bodyText = textConfig["bodyText"][i];
            DynamicNodeText* text = DynamicNodeText::create();
            if(text->initWithParams(DynamicNodeText::kDefaultBodyFontSize, Style::Color_4B::ctaNodeText, bodyText, _textLayer->getContentSize(), _usingExternalParams))
            {
                _textLayer->addChild(text);
            }
        }
    }
    
    if(textConfig.HasMember("footerText"))
    {
        const rapidjson::Value& footerText = textConfig["footerText"];
        DynamicNodeText* text = DynamicNodeText::create();
        if(text->initWithParams(DynamicNodeText::kDefaultFooterFontSize, Style::Color_4B::ctaNodeText, footerText, _textLayer->getContentSize(), _usingExternalParams))
        {
            _textLayer->addChild(text);
        }
        
    }
}

void DynamicNodeCreator::configTextInput(const rapidjson::Value &textInputConfig)
{
    if(textInputConfig.IsArray())
    {
        for (int i = 0; i < textInputConfig.Size(); i++)
        {
            DynamicNodeTextInput* textInput = DynamicNodeTextInput::create();
            textInput->initWithParams(textInputConfig[i], _textInputLayer->getContentSize(), _usingExternalParams);
            _textInputLayer->addChild(textInput);
        }
    }
}

void DynamicNodeCreator::configLines(const rapidjson::Value &linesConfig)
{
    if(linesConfig.IsArray())
    {
        for (int i = 0; i < linesConfig.Size(); i++)
        {
            DynamicNodeLine* line = DynamicNodeLine::create();
            line->initWithParams(linesConfig[i], _linesLayer->getContentSize());
            _linesLayer->addChild(line);
        }
    }
}

void DynamicNodeCreator::configCheckBoxes(const rapidjson::Value &checkboxConfig)
{
    if(checkboxConfig.IsArray())
    {
        for (int i = 0; i < checkboxConfig.Size(); i++)
        {
            DynamicNodeCheckBox* checkBox = DynamicNodeCheckBox::create();
            checkBox->initWithParams(checkboxConfig[i], _checkBoxLayer->getContentSize(),_usingExternalParams);
            _checkBoxLayer->addChild(checkBox);
        }
    }
}

std::string DynamicNodeCreator::addExternalParamsToString(const std::string& str)
{
    std::string sourceString = str;
    std::string result = "";
    long i = sourceString.find("<");
    
    while (i < sourceString.npos)
    {
        result += sourceString.substr(0,i);
        const std::string& paramName = sourceString.substr(i+1,sourceString.find(">") - (i+1));
        sourceString = sourceString.substr(sourceString.find(">") + 1);
        result += getStringFromJson(paramName, DynamicNodeCreator::getInstance()->_externParams);
        i = sourceString.find("<");
    }
    
    result += sourceString;
    
    
    return result;
}

void DynamicNodeCreator::resetCTAPopup()
{
    AnalyticsSingleton::getInstance()->registerCTASource("","","");
    if(_CTANode)
    {
        _CTANode->removeFromParent();
        _CTANode = nullptr;
        DynamicNodeHandler::getInstance()->_currentCTAFile = "";
    }
}

NS_AZOOMEE_END
