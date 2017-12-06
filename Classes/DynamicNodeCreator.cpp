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
#include "HQDataProvider.h"
#include "DynamicNodeTextInput.h"
#include "DynamicNodeButton.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<DynamicNodeCreator> sDynamicNodeCreatorSharedInstance;

const std::string DynamicNodeCreator::kCTAAssetLoc = "res/cta_assets/";
const std::string DynamicNodeCreator::kCTABundleImageLoc = "res/cta_assets/cta_bundle/images/";
const std::string DynamicNodeCreator::kCTADeviceImageCacheLoc = "DCDECache/images/";

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

    const std::string& jsonString = FileUtils::getInstance()->getStringFromFile(filepath);
    
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
    
    const std::string& jsonString = FileUtils::getInstance()->getStringFromFile(filepath);
    
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
    
    _stencil = ui::Scale9Sprite::create(kCTAAssetLoc + "deep_free_pop_over.png");
    _stencil->setContentSize(Size(_windowSize.width*0.75,_windowSize.height*0.67));
    
    _clippingNode = ClippingNode::create(_stencil);
    
    _bgColour = LayerColor::create(Color4B::WHITE, _windowSize.width, _windowSize.height);
    _bgColour->setPosition(Vec2(_windowSize*-0.5f));
    _clippingNode->addChild(_bgColour);
    
    _maskedBGImage = Sprite::create(kCTAAssetLoc + "deep_free_pop_over_trans.png");
    _maskedBGImage->setScale(_stencil->getContentSize().width/_maskedBGImage->getContentSize().width, _stencil->getContentSize().height/_maskedBGImage->getContentSize().height);
    _maskedBGImage->setVisible(false);
    _clippingNode->addChild(_maskedBGImage);
    
    _clippingNode->setAlphaThreshold(0.5f);
    _clippingNode->setPosition(_windowSize/2);
    _CTANode->addChild(_clippingNode);
    
    _textLayer = Node::create();
    _textLayer->setContentSize(_windowSize);
    _textLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _textLayer->setPosition(_windowSize/2);
    _CTANode->addChild(_textLayer);
    
    _popupButtonsLayer = Node::create();
    _popupButtonsLayer->setContentSize(_stencil->getContentSize());
    _popupButtonsLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _popupButtonsLayer->setPosition(_windowSize/2);
    _CTANode->addChild(_popupButtonsLayer);
    
    _popupImages = Node::create();
    _popupImages->setContentSize(_stencil->getContentSize());
    _popupImages->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _popupImages->setPosition(_windowSize/2);
    _CTANode->addChild(_popupImages);
    
    _textInputLayer = Node::create();
    _textInputLayer->setContentSize(_stencil->getContentSize());
    _textInputLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _textInputLayer->setPosition(_windowSize/2);
    _CTANode->addChild(_textInputLayer);
    
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
            float width = sizePercentages[0].GetFloat()/100.0f;
            float height = sizePercentages[1].GetFloat()/100.0f;
            const Size& newSize = Size(_windowSize.width*width,_windowSize.height*height);
            
            _stencil->setContentSize(newSize);
            _maskedBGImage->setScale(_stencil->getContentSize().width/_maskedBGImage->getContentSize().width, _stencil->getContentSize().height/_maskedBGImage->getContentSize().height);
            _popupFrame->setContentSize(newSize);
            _popupButtonsLayer->setContentSize(newSize);
            _popupImages->setContentSize(newSize);
            _textLayer->setContentSize(newSize);
            _textInputLayer->setContentSize(newSize);
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
    _maskedBGImage->setVisible(true);
    const std::string& filename = getStringFromJson("file", backgroundImageData);
    bool imagefound = false;
    
    if(filename != "")
    {
        
        if(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + kCTADeviceImageCacheLoc + filename))
        {
            _maskedBGImage->initWithFile(FileUtils::getInstance()->getWritablePath() + kCTADeviceImageCacheLoc + filename);
            imagefound = true;
        }
        else
        {
            if(FileUtils::getInstance()->isFileExist(kCTABundleImageLoc + filename))
            {
                _maskedBGImage->initWithFile(kCTABundleImageLoc + filename);
                imagefound = true;
            }
        }
    }
    
    if(imagefound)
    {
        std::string displaymode = getStringFromJson("displayMode", backgroundImageData);
        if(displaymode == "fill")
        {
            _maskedBGImage->setScale(_stencil->getContentSize().width/_maskedBGImage->getContentSize().width, _stencil->getContentSize().height/_maskedBGImage->getContentSize().height);
        }
        else if(displaymode == "fit")
        {
            float widthScale = _stencil->getContentSize().width/_maskedBGImage->getContentSize().width;
            float heightScale =   _stencil->getContentSize().height/_maskedBGImage->getContentSize().height;
            _maskedBGImage->setScale(MIN(widthScale, heightScale));
            int yPos = getIntFromJson("position",backgroundImageData);
            if(yPos != INT_MAX)
            {
                _maskedBGImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                _maskedBGImage->setPosition(Vec2(0, _stencil->getContentSize().height * ((yPos-50)/100.0f)));
            }
        }
    }
    
}

void DynamicNodeCreator::configButtons(const rapidjson::Value &buttonsList)
{
    if(buttonsList.IsArray())
    {
        for (int i = 0; i < buttonsList.Size(); i++)
        {
            DynamicNodeButton* button = DynamicNodeButton::create();
            if(_usingExternalParams)
            {
                if(button->initWithParams(buttonsList[i], _popupButtonsLayer->getContentSize(), _externParams))
                {
                    _popupButtonsLayer->addChild(button);
                }
            }
            else
            {
                if(button->initWithParams(buttonsList[i], _popupButtonsLayer->getContentSize()))
                {
                    _popupButtonsLayer->addChild(button);
                }
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
            Vec2 pos;
            Vec2 size;
            int opacity;
            bool usingRemoteImage;
            
            pos = getVec2FromJson("position",imageList[i]);
            
            if(pos.x != 0 && pos.y != 0)
            {
                pos = pos/100.0f;
            }
            else
            {
                continue;
            }
            
            size = getVec2FromJson("size",imageList[i]);
            
            if(size.x != 0 && size.y != 0)
            {
                size = size/100.0f;
            }
            else
            {
                continue;
            }
            
            opacity = getIntFromJson("opacity", imageList[i]);
            
            if(opacity == INT_MAX)
            {
                opacity = 255;
            }
            
            usingRemoteImage = getBoolFromJson("usingRemoteImage", imageList[i]);
            
            if(!usingRemoteImage)
            {
                const std::string& filename = getStringFromJson("file", imageList[i]);
            
                if(filename != "")
                {
                    if(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + kCTADeviceImageCacheLoc + filename))
                    {
                        addImageWithParams(size, pos, opacity, FileUtils::getInstance()->getWritablePath() + kCTADeviceImageCacheLoc + filename);
                    }
                    else
                    {
                        if(FileUtils::getInstance()->isFileExist(kCTAAssetLoc + kCTABundleImageLoc + filename))
                        {
                            addImageWithParams(size, pos, opacity,kCTAAssetLoc +  kCTABundleImageLoc + filename);
                        }
                    }
                }
            }
            else
            {
                std::string fileurl = getStringFromJson("file", imageList[i]);
                fileurl = addExternalParamsToString(fileurl); // file url will be the id of the HQ item object
                fileurl = HQDataProvider::getInstance()->getImageUrlForItem(fileurl, Vec2(1,1)); //get actual url of image from HQDataProvider
                addRemoteImageWithParams(size, pos, opacity, fileurl);
            }
        }
    }

    
}

void DynamicNodeCreator::configText(const rapidjson::Value& textConfig)
{
    if(textConfig.HasMember("titleText"))
    {
        const rapidjson::Value& titleText = textConfig["titleText"];
        addTextWithParams(94, Style::Color_4B::ctaNodeText, titleText);
    }
    
    if(textConfig.HasMember("bodyText"))
    {
        for(int i = 0; i < textConfig["bodyText"].Size(); i++)
        {
            const rapidjson::Value& bodyText = textConfig["bodyText"][i];
            addTextWithParams(63, Color4B(Style::Color::black), bodyText);
        }
    }
    
    if(textConfig.HasMember("footerText"))
    {
        const rapidjson::Value& footerText = textConfig["footerText"];
        addTextWithParams(42, Style::Color_4B::ctaNodeText, footerText);
    }
}

void DynamicNodeCreator::configTextInput(const rapidjson::Value &textInputConfig)
{
    if(textInputConfig.IsArray())
    {
        for (int i = 0; i < textInputConfig.Size(); i++)
        {
            DynamicNodeTextInput* textInput = DynamicNodeTextInput::create();
            textInput->initWithParams(textInputConfig[i], _textInputLayer->getContentSize());
            _textInputLayer->addChild(textInput);
        }
    }
}

void DynamicNodeCreator::addImageWithParams(const Vec2& size, const Vec2& pos, int opacity, const std::string& filename)
{
    Sprite* image = Sprite::create(filename);
    if(size.x > 0 && size.y > 0)
    {
        image->setScale((_popupImages->getContentSize().width*size.x)/image->getContentSize().width, (_popupImages->getContentSize().height*size.y)/image->getContentSize().height);
    }
    else if(size.x <= 0 && size.y <= 0)
    {
        image->setScale(1);
    }
    else if(size.x <= 0)
    {
        image->setScale((_popupImages->getContentSize().height*size.y)/image->getContentSize().height);
    }
    else
    {
        image->setScale((_popupImages->getContentSize().width*size.x)/image->getContentSize().width);
    }
    
    image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    image->setNormalizedPosition(pos);
    image->setOpacity(opacity);
    _popupImages->addChild(image);
}

void DynamicNodeCreator::addRemoteImageWithParams(const Vec2& size, const Vec2& pos, int opacity, const std::string& url)
{
    Sprite* tempSizeSprite = Sprite::create("res/contentPlaceholders/Games1X1.png");
    RemoteImageSprite* image = RemoteImageSprite::create();
    Size imageSize;
    if(size.x > 0 && size.y > 0)
    {
        imageSize = Vec2((_popupImages->getContentSize().width*size.x), (_popupImages->getContentSize().height*size.y));
    }
    else if(size.x <= 0 && size.y <= 0)
    {
        imageSize = tempSizeSprite->getContentSize();
    }
    else if(size.x <= 0)
    {
        float scaleMod = (_popupImages->getContentSize().height*size.y)/tempSizeSprite->getContentSize().height;
        imageSize = tempSizeSprite->getContentSize() * scaleMod;
        
    }
    else
    {
        float scaleMod = (_popupImages->getContentSize().width*size.x)/tempSizeSprite->getContentSize().width;
        imageSize = tempSizeSprite->getContentSize() * scaleMod;
    }
    image->initWithUrlAndSizeWithoutPlaceholder(url, imageSize);
    image->setNormalizedPosition(pos);
    image->setOpacity(opacity);
    _popupImages->addChild(image);
}

void DynamicNodeCreator::addTextWithParams(int fontSize, Color4B fontColour, const rapidjson::Value& params)
{
    Vec2 pos = getVec2FromJson("position", params)/100.0f;
    std::string text = getStringFromJson("text", params);
    
    if(_usingExternalParams)
    {
        text = addExternalParamsToString(text);
    }
    
    const std::string& alignment = getStringFromJson("alignment", params);
    int newFontSize = getIntFromJson("fontSize", params);
    if(newFontSize != INT_MAX)
    {
        fontSize = newFontSize;
    }
    
    int lineSpacing = getIntFromJson("lineSpacing", params);
    if(lineSpacing == INT_MAX)
    {
        lineSpacing = 20;
    }
    
    fontColour = getColor4BFromJson("colour", params);
    
    Label* label = Label::createWithTTF(text, Style::Font::Regular, fontSize);
    label->setNormalizedPosition(pos);
    label->setTextColor(fontColour);
    label->setLineSpacing(lineSpacing);
    
    if(alignment == "left")
    {
        label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        label->setHorizontalAlignment(TextHAlignment::LEFT);
    }
    else if(alignment == "right")
    {
        label->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        label->setHorizontalAlignment(TextHAlignment::RIGHT);
    }
    else
    {
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        label->setHorizontalAlignment(TextHAlignment::CENTER);
    }
    
    _textLayer->addChild(label);
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
    if(_CTANode)
    {
        _CTANode->removeFromParent();
        _CTANode = nullptr;
    }
}

NS_AZOOMEE_END
