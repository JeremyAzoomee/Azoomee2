//
//  DynamicNodeCreator.cpp
//  azoomee2
//
//  Created by Macauley on 21/09/2017.
//
//

#include "DynamicNodeCreator.h"
#include "ui/CocosGUI.h"
#include <AzoomeeCommon/Data/Json.h>
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
    
    const Size& windowSize = Director::getInstance()->getWinSize();
    
    Node* CTANode = Node::create();
    
    LayerColor* overlay = LayerColor::create(Style::Color_4B::semiTransparentOverlay, windowSize.width, windowSize.height);
    overlay->setName("overlay");
    CTANode->addChild(overlay);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), overlay);
    
    auto stencil = ui::Scale9Sprite::create("res/CTA_Assets/deep_free_pop_over.png");
    stencil->setContentSize(Size(windowSize.width*0.75,windowSize.height*0.67));
    
    auto clippingNode = ClippingNode::create(stencil);
    
    auto bgColour = LayerColor::create(Color4B::WHITE, windowSize.width, windowSize.height);
    bgColour->setPosition(-Vec2(windowSize/2));
    clippingNode->addChild(bgColour);
    
    auto maskedSprite = Sprite::create("res/CTA_Assets/deep_free_pop_over_trans.png");
    maskedSprite->setScale(stencil->getContentSize().width/maskedSprite->getContentSize().width, stencil->getContentSize().height/maskedSprite->getContentSize().height);
    clippingNode->addChild(maskedSprite);
    
    clippingNode->setAlphaThreshold(0.5f);
    clippingNode->setPosition(windowSize/2);
    CTANode->addChild(clippingNode);
    
    auto popupButtonsLayer = Node::create();
    popupButtonsLayer->setContentSize(stencil->getContentSize());
    popupButtonsLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    popupButtonsLayer->setPosition(windowSize/2);
    CTANode->addChild(popupButtonsLayer);
    
    auto popupFrame = ui::Scale9Sprite::create("res/CTA_Assets/deep_free_pop_over_trans.png");
    popupFrame->setPosition(windowSize/2);
    popupFrame->setAnchorPoint(Vec2(0.5,0.5));
    popupFrame->setContentSize(Size(windowSize.width*0.75,windowSize.height*0.67));
    CTANode->addChild(popupFrame);
    
    auto closeButton = ui::Button::create();
    closeButton->loadTextures("res/CTA_Assets/close.png", "res/CTA_Assets/close.png");
    closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    closeButton->setPosition(popupFrame->getPosition() + popupFrame->getContentSize() * 0.48);
    closeButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type){CTANode->removeFromParent();});
    CTANode->addChild(closeButton);

    const std::string& jsonString = FileUtils::getInstance()->getStringFromFile(filepath);
    
    rapidjson::Document configFile;
    configFile.Parse(jsonString.c_str());
    
    if(configFile.HasParseError())
    {
        return CTANode;
    }
    
    /*
     values not used:
     nodeId, groupId
     these are specific for asset gathering and storing purposes, not needed in theis viewer
     */
    
    if(configFile.HasMember("nodeSize"))
    {
        rapidjson::Value& sizePercentages = configFile["nodeSize"];
        if(sizePercentages.Size() == 2)
        {
            if(sizePercentages[0].IsInt() && sizePercentages[1].IsInt())
            {
                float width = sizePercentages[0].GetInt()/100.0f;
                float height = sizePercentages[1].GetInt()/100.0f;
                
                stencil->setContentSize(Size(windowSize.width*width,windowSize.height*height));
                maskedSprite->setScale(stencil->getContentSize().width/maskedSprite->getContentSize().width, stencil->getContentSize().height/maskedSprite->getContentSize().height);
                popupFrame->setContentSize(Size(windowSize.width*width,windowSize.height*height));
                popupButtonsLayer->setContentSize(Size(windowSize.width*width,windowSize.height*height));
                closeButton->setScale(width);
            }
        }
    }
    
    if(configFile.HasMember("closeButton"))
    {
        rapidjson::Value& closeButtonToggle = configFile["closeButton"];
        if(closeButtonToggle.IsBool())
        {
            bool showCloseButton = closeButtonToggle.GetBool();
            
            closeButton->setVisible(showCloseButton);
            closeButton->setPosition(popupFrame->getPosition() + popupFrame->getContentSize()/2 - (closeButton->getContentSize() * closeButton->getScale())/2);
        }
    }
    
    if(configFile.HasMember("backgroundColour"))
    {
        rapidjson::Value& backgroundColour = configFile["backgroundColour"];
        
        if(backgroundColour.Size() == 3 && backgroundColour[0].IsInt() && backgroundColour[1].IsInt() && backgroundColour[2].IsInt())
        {
            Color3B newColour = Color3B(backgroundColour[0].GetInt(), backgroundColour[1].GetInt(), backgroundColour[2].GetInt());
            bgColour->setColor(newColour);
        }
    }
    
    if(configFile.HasMember("backgroundImage"))
    {
        rapidjson::Value& backgroundImageData = configFile["backgroundImage"];
        
        if(backgroundImageData.HasMember("data"))
        {
            rapidjson::Value& imageData = backgroundImageData["data"];
            
            if(imageData.IsString())
            {
                std::string dataStr = imageData.GetString();
                if(dataStr.length() > 22){
                    dataStr = dataStr.substr(22);
                    
                    int len = 0;
                    unsigned char *buffer;
                    len = base64Decode((const unsigned char*)dataStr.c_str(), (unsigned int)dataStr.length(), &buffer);
                    
                    Image *img = new Image();
                    
                    img->initWithImageData(buffer, len);
                    Texture2D *texture = new Texture2D();
                    texture->initWithImage(img);
                    
                    delete img;
                    
                    maskedSprite->initWithTexture(texture);
                    
                    if(backgroundImageData.HasMember("displayMode"))
                    {
                        if(backgroundImageData["displayMode"].IsString())
                        {
                            std::string displaymode = backgroundImageData["displayMode"].GetString();
                            if(displaymode == "fill")
                            {
                                maskedSprite->setScale(stencil->getContentSize().width/maskedSprite->getContentSize().width, stencil->getContentSize().height/maskedSprite->getContentSize().height);
                            }
                            else if(displaymode == "fit")
                            {
                                float widthScale = stencil->getContentSize().width/maskedSprite->getContentSize().width;
                                float heightScale =   stencil->getContentSize().height/maskedSprite->getContentSize().height;
                                maskedSprite->setScale(MIN(widthScale, heightScale));
                            }
                        }
                    }
                    
                    
                }
            }
        }
    }
    
    popupButtonsLayer->removeAllChildren();
    
    if(configFile.HasMember("buttons"))
    {
        rapidjson::Value& buttonsList = configFile["buttons"];
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
                button->setContentSize(Size(popupButtonsLayer->getContentSize().width * size.x,popupButtonsLayer->getContentSize().height * size.y));
                button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                button->setNormalizedPosition(pos);
                button->setScale9Enabled(true);
                //add button callback based on action tag here
                
                Label* label = Label::createWithTTF(btnString, "fonts/azoomee.ttf", button->getContentSize().height*0.4);
                label->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
                label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                label->setTextColor(Color4B::BLACK);
                button->addChild(label);
                popupButtonsLayer->addChild(button);
                
            }
        }
    }
    
    return CTANode;
    
}


NS_AZOOMEE_END
