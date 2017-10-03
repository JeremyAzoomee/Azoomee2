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

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<DynamicNodeCreator> sDynamicNodeCreatorSharedInstance;

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
    //Init CTA objects
    initCTANode();

    const std::string& jsonString = FileUtils::getInstance()->getStringFromFile(filepath);
    
    rapidjson::Document configFile;
    configFile.Parse(jsonString.c_str());
    
    if(configFile.HasParseError())
    {
        return _CTANode;
    }
    
    
    
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
    
    //return resultant CTA node
    return _CTANode;
    
}

void DynamicNodeCreator::initCTANode()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    _windowSize = Director::getInstance()->getWinSize();
    
    if(_CTANode)
    {
        _CTANode->removeFromParent();
    }
    
    _CTANode = Node::create();
    
    LayerColor* overlay = LayerColor::create(Style::Color_4B::semiTransparentOverlay, _windowSize.width, _windowSize.height);
    overlay->setName("overlay");
    _CTANode->addChild(overlay);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true; //block touches
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), overlay);
    
    _stencil = ui::Scale9Sprite::create(_kCTAAssetLoc + "deep_free_pop_over.png");
    _stencil->setContentSize(Size(_windowSize.width*0.75,_windowSize.height*0.67));
    
    _clippingNode = ClippingNode::create(_stencil);
    
    _bgColour = LayerColor::create(Color4B::WHITE, _windowSize.width, _windowSize.height);
    _bgColour->setPosition(Vec2(_windowSize*-0.5f));
    _clippingNode->addChild(_bgColour);
    
    _maskedBGImage = Sprite::create(_kCTAAssetLoc + "deep_free_pop_over_trans.png");
    _maskedBGImage->setScale(_stencil->getContentSize().width/_maskedBGImage->getContentSize().width, _stencil->getContentSize().height/_maskedBGImage->getContentSize().height);
    _clippingNode->addChild(_maskedBGImage);
    
    _clippingNode->setAlphaThreshold(0.5f);
    _clippingNode->setPosition(_windowSize/2);
    _CTANode->addChild(_clippingNode);
    
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
    
    _popupFrame = ui::Scale9Sprite::create(_kCTAAssetLoc + "deep_free_pop_over_trans.png");
    _popupFrame->setPosition(_windowSize/2);
    _popupFrame->setAnchorPoint(Vec2(0.5,0.5));
    _popupFrame->setContentSize(Size(_windowSize.width*0.75,_windowSize.height*0.67));
    _CTANode->addChild(_popupFrame);
    
    _closeButton = ui::Button::create();
    _closeButton->loadTextures(_kCTAAssetLoc + "close.png", _kCTAAssetLoc + "close.png");
    _closeButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _closeButton->setPosition(_popupFrame->getPosition() + _popupFrame->getContentSize()/2 - _closeButton->getContentSize()*0.75);
    _closeButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type){
        this->resetCTAPopup();
    });
    _CTANode->addChild(_closeButton);
    
}

void DynamicNodeCreator::configNodeSize(const rapidjson::Value &sizePercentages)
{
    if(sizePercentages.Size() == 2)
    {
        if(sizePercentages[0].IsInt() && sizePercentages[1].IsInt())
        {
            float width = sizePercentages[0].GetInt()/100.0f;
            float height = sizePercentages[1].GetInt()/100.0f;
            const Size& newSize = Size(_windowSize.width*width,_windowSize.height*height);
            
            _stencil->setContentSize(newSize);
            _maskedBGImage->setScale(_stencil->getContentSize().width/_maskedBGImage->getContentSize().width, _stencil->getContentSize().height/_maskedBGImage->getContentSize().height);
            _popupFrame->setContentSize(newSize);
            _popupButtonsLayer->setContentSize(newSize);
            _popupImages->setContentSize(newSize);
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
    std::string dataStr = getStringFromJson("data", backgroundImageData);
        
    Texture2D* texture = getTextureFromBase64imageData(dataStr,"popupBGImage");
    if(texture)
    {
        _maskedBGImage->initWithTexture(texture);
        
        const std::string& displaymode = getStringFromJson("displayMode", backgroundImageData);
        if(displaymode == "fill")
        {
            _maskedBGImage->setScale(_stencil->getContentSize().width/_maskedBGImage->getContentSize().width, _stencil->getContentSize().height/_maskedBGImage->getContentSize().height);
        }
        else if(displaymode == "fit")
        {
            float widthScale = _stencil->getContentSize().width/_maskedBGImage->getContentSize().width;
            float heightScale =   _stencil->getContentSize().height/_maskedBGImage->getContentSize().height;
            _maskedBGImage->setScale(MIN(widthScale, heightScale));
        }
    }
    
}

void DynamicNodeCreator::configButtons(const rapidjson::Value &buttonsList)
{
    if(buttonsList.IsArray())
    {
        for (int i = 0; i < buttonsList.Size(); i++)
        {
            /*
             Values not used:
             buttonId, buttonType, action, style
             These control the buttons callback function and button style
             */
            Vec2 pos;
            Vec2 size;
            ButtonActionDataRef actionData;
            
            pos = getVec2FromJson("position",buttonsList[i]);
            
            if(pos.x != 0 && pos.y != 0)
            {
                pos = pos/100.0f;
            }
            else
            {
                continue;
            }
            
            size = getVec2FromJson("size",buttonsList[i]);
            
            if(size.x != 0 && size.y != 0)
            {
                size = size/100.0f;
            }
            else
            {
                continue;
            }
            
            const std::string& btnString = getStringFromJson("text", buttonsList[i]);
            
            if(buttonsList[i].HasMember("action"))
            {
                const rapidjson::Value& actionParams = buttonsList[i]["action"];
                actionData = ButtonActionData::createWithJson(actionParams);
            }
            
            addButtonWithParams(size, pos, btnString, actionData);
            
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
            
            if(imageList[i].HasMember("opacity") && imageList[i]["opacity"].IsInt())
            {
                opacity = imageList[i]["opacity"].GetInt();
            }
            else
            {
                opacity = 255;
            }
            
            std::string dataStr = getStringFromJson("data", imageList[i]);
            const std::string& imageName = "popupImage" + std::to_string(i);
            Texture2D* texture = getTextureFromBase64imageData(dataStr,imageName);
            if(texture)
            {
                addImageWithParams(size, pos, opacity, texture);
            }

        }
    }

}

void DynamicNodeCreator::addButtonWithParams(const Vec2 &size, const Vec2 &pos, const std::string &buttonText, ButtonActionDataRef buttonActionData)
{
    ui::Button* button = ui::Button::create();
    button->loadTextures(_kCTAAssetLoc + "rectangle_copy_3.png", _kCTAAssetLoc + "rectangle_copy_3.png"); //will be dependent on style tag
    button->setContentSize(Size(_popupButtonsLayer->getContentSize().width * size.x,_popupButtonsLayer->getContentSize().height * size.y));
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setNormalizedPosition(pos);
    button->setScale9Enabled(true);
    button->setSwallowTouches(true);
    button->addTouchEventListener(CC_CALLBACK_2(DynamicNodeButtonListener::onButtonPressedCallFunc, DynamicNodeButtonListener::getInstance(),buttonActionData));
    
    Label* label = Label::createWithTTF(buttonText, Style::Font::Regular, button->getContentSize().height*0.4);
    label->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setTextColor(Color4B::BLACK);
    button->addChild(label);
    _popupButtonsLayer->addChild(button);
}

void DynamicNodeCreator::addImageWithParams(const Vec2 &size, const Vec2 &pos, int opacity, Texture2D *texture)
{
    Sprite* image = Sprite::createWithTexture(texture);
    image->setScale((_windowSize.width*size.x)/image->getContentSize().width, (_windowSize.height*size.y)/image->getContentSize().height);
    image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    image->setNormalizedPosition(pos);
    image->setOpacity(opacity/255.0f);
    _popupImages->addChild(image);

}

Texture2D* DynamicNodeCreator::getTextureFromBase64imageData(std::string& data, const std::string& imageName)
{
    if(data.length() > 22){
        data = data.substr(22);
        
        int len = 0;
        unsigned char *buffer;
        len = base64Decode((const unsigned char*)data.c_str(), (unsigned int)data.length(), &buffer);
        
        Image *img = new Image();
        img->initWithImageData(buffer, len);
        
        Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(img, imageName); // make sure cocos has the new texture in its cache
        
        delete img;
        return texture;
    }
    return nullptr;
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
