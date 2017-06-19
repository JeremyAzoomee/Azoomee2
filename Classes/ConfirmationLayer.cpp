#include "ConfirmationLayer.h"
#include "ui/UIScale9Sprite.h"

ConfirmationLayer* ConfirmationLayer::createWithDetails(std::string setChildName, std::string setFriendName, std::string setFriendCode)
{
    auto layer = ConfirmationLayer::create();
    layer->childName = setChildName;

    
    return layer;
}

bool ConfirmationLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}
