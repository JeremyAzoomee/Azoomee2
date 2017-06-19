#include "ConfirmationLayer.h"
#include "ui/UIScale9Sprite.h"

bool ConfirmationLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//---------------------ADD DETAILS---------------------------------

void ConfirmationLayer::addDetailsToLayer(std::string setChildName, std::string setFriendName, std::string setFriendCode)
{
    childName = setChildName;
    friendName = setFriendName;
    friendCode = setFriendCode;
    
    DrawNode* topLine = DrawNode::create();
    topLine->setLineWidth(2);
    topLine->drawLine(Vec2(0, this->getContentSize().height), Vec2(this->getContentSize().width,this->getContentSize().height), Color4F(150.96/255.0f, 150.96/255.0f, 150.06/255.0f,255/255.0f));
    this->addChild(topLine);
    
    DrawNode* bottomLine = DrawNode::create();
    bottomLine->setLineWidth(2);
    bottomLine->drawLine(Vec2(0, 0), Vec2(this->getContentSize().width,0), Color4F(150.96/255.0f, 150.96/255.0f, 150.06/255.0f,255/255.0f));
    this->addChild(bottomLine);
}
