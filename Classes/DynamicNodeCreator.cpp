//
//  DynamicNodeCreator.cpp
//  azoomee2
//
//  Created by Macauley on 21/09/2017.
//
//

#include "DynamicNodeCreator.h"
#include <AzoomeeCommon/UI/Style.h>

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
        rapidjson::Value& sizePercentages = configFile["nodeSize"];
        configNodeSize(sizePercentages);
    }
    
    //config close button
    if(configFile.HasMember("closeButton"))
    {
        rapidjson::Value& closeButtonToggle = configFile["closeButton"];
        configCloseButton(closeButtonToggle);
    }
    
    //config background colour
    if(configFile.HasMember("backgroundColour"))
    {
        rapidjson::Value& backgroundColour = configFile["backgroundColour"];
        configBackgroundColour(backgroundColour);
    }
    
    //config background image
    if(configFile.HasMember("backgroundImage"))
    {
        rapidjson::Value& backgroundImageData = configFile["backgroundImage"];
        configBackgroundImage(backgroundImageData);
    }
    
    //config buttons
    
    if(configFile.HasMember("buttons"))
    {
        rapidjson::Value& buttonsList = configFile["buttons"];
        configButtons(buttonsList);
    }
    
    //config extra images
    if(configFile.HasMember("images"))
    {
        rapidjson::Value& imageList = configFile["buttons"];
        configExtraImages(imageList);
    }
    //not in json yet
    
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
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), overlay);
    
    _stencil = ui::Scale9Sprite::create("res/CTA_Assets/deep_free_pop_over.png");
    _stencil->setContentSize(Size(_windowSize.width*0.75,_windowSize.height*0.67));
    
    _clippingNode = ClippingNode::create(_stencil);
    
    _bgColour = LayerColor::create(Color4B::WHITE, _windowSize.width, _windowSize.height);
    _bgColour->setPosition(-Vec2(_windowSize/2));
    _clippingNode->addChild(_bgColour);
    
    _maskedBGImage = Sprite::create("res/CTA_Assets/deep_free_pop_over_trans.png");
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
    
    _popupFrame = ui::Scale9Sprite::create("res/CTA_Assets/deep_free_pop_over_trans.png");
    _popupFrame->setPosition(_windowSize/2);
    _popupFrame->setAnchorPoint(Vec2(0.5,0.5));
    _popupFrame->setContentSize(Size(_windowSize.width*0.75,_windowSize.height*0.67));
    _CTANode->addChild(_popupFrame);
    
    _closeButton = ui::Button::create();
    _closeButton->loadTextures("res/CTA_Assets/close.png", "res/CTA_Assets/close.png");
    _closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _closeButton->setPosition(_popupFrame->getPosition() + _popupFrame->getContentSize() * 0.48);
    _closeButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type){
        _CTANode->removeFromParent();
        _CTANode = nullptr;
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
            
            _stencil->setContentSize(Size(_windowSize.width*width,_windowSize.height*height));
            _maskedBGImage->setScale(_stencil->getContentSize().width/_maskedBGImage->getContentSize().width, _stencil->getContentSize().height/_maskedBGImage->getContentSize().height);
            _popupFrame->setContentSize(Size(_windowSize.width*width,_windowSize.height*height));
            _popupButtonsLayer->setContentSize(Size(_windowSize.width*width,_windowSize.height*height));
            _closeButton->setScale(width);
        }
    }
}

void DynamicNodeCreator::configCloseButton(const rapidjson::Value &closeButtonToggle)
{
    if(closeButtonToggle.IsBool())
    {
        bool showCloseButton = closeButtonToggle.GetBool();
        
        _closeButton->setVisible(showCloseButton);
        _closeButton->setPosition(_popupFrame->getPosition() + _popupFrame->getContentSize()/2 - (_closeButton->getContentSize() * _closeButton->getScale())/2);
    }
}

void DynamicNodeCreator::configBackgroundColour(const rapidjson::Value &backgroundColour)
{
    if(backgroundColour.Size() == 3 && backgroundColour[0].IsInt() && backgroundColour[1].IsInt() && backgroundColour[2].IsInt())
    {
        Color3B newColour = Color3B(backgroundColour[0].GetInt(), backgroundColour[1].GetInt(), backgroundColour[2].GetInt());
        _bgColour->setColor(newColour);
    }
}

void DynamicNodeCreator::configBackgroundImage(const rapidjson::Value &backgroundImageData)
{
    if(backgroundImageData.HasMember("data"))
    {
        const rapidjson::Value& imageData = backgroundImageData["data"];
        
        if(imageData.IsString())
        {
            std::string dataStr = imageData.GetString();
            
            Texture2D* texture = getTextureFromBase64imageData(dataStr,"popupBGImage");
            if(texture){
                _maskedBGImage->initWithTexture(texture);
                
                if(backgroundImageData.HasMember("displayMode"))
                {
                    if(backgroundImageData["displayMode"].IsString())
                    {
                        std::string displaymode = backgroundImageData["displayMode"].GetString();
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
            /*
             Values not used:
             buttonId, buttonType, action, style
             These control the buttons callback function and button style
             */
            Vec2 pos;
            Vec2 size;
            std::string btnString;
            
            if(buttonsList[i].HasMember("position") && buttonsList[i]["position"].Size() == 2 && buttonsList[i]["position"][0].IsInt() && buttonsList[i]["position"][1].IsInt())
            {
                pos = Vec2(buttonsList[i]["position"][0].GetInt()/100.0f,buttonsList[i]["position"][1].GetInt()/100.0f);
            }
            else
            {
                continue;
            }
            
            if(buttonsList[i].HasMember("size") && buttonsList[i]["size"].Size() == 2 && buttonsList[i]["size"][0].IsInt() && buttonsList[i]["size"][1].IsInt())
            {
                size = Vec2(buttonsList[i]["size"][0].GetInt()/100.0f, buttonsList[i]["size"][1].GetInt()/100.0f);
            }
            else
            {
                continue;
            }
            if(buttonsList[i].HasMember("text") && buttonsList[i]["text"].IsString())
            {
                btnString = buttonsList[i]["text"].GetString();
            }
            
            ui::Button* button = ui::Button::create();
            button->loadTextures("res/CTA_Assets/rectangle_copy_3.png", "res/CTA_Assets/rectangle_copy_3.png"); //dependent on style tag
            button->setContentSize(Size(_popupButtonsLayer->getContentSize().width * size.x,_popupButtonsLayer->getContentSize().height * size.y));
            button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            button->setNormalizedPosition(pos);
            button->setScale9Enabled(true);
            //add button callback based on action tag here
            
            Label* label = Label::createWithTTF(btnString, "fonts/azoomee.ttf", button->getContentSize().height*0.4);
            label->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            label->setTextColor(Color4B::BLACK);
            button->addChild(label);
            _popupButtonsLayer->addChild(button);
            
        }
    }
}

void DynamicNodeCreator::configExtraImages(const rapidjson::Value &imageList)
{
    //not yet implemented
}

Texture2D* DynamicNodeCreator::getTextureFromBase64imageData(std::string data, const std::string& imageName)
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

NS_AZOOMEE_END
