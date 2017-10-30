#include "HQSceneElementPositioner.h"
#include "ui/UIScrollView.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void HQSceneElementPositioner::positionHQSceneElement(cocos2d::Layer* sender,  bool isGroup)
{
    float groupHeightOffset = 0.0f;
    
    if(isGroup)
    {
        groupHeightOffset = ConfigStorage::getInstance()->getGroupContentItemTextHeight();
    }
    
    cocos2d::ui::ScrollView *scrollView = (cocos2d::ui::ScrollView *)sender->getParent()->getParent();
    Node *layout = (Node *)sender->getParent();
    
    Size scrollViewSize = scrollView->getInnerContainerSize();
    Size senderSize = sender->getBoundingBox().size;
    Vector<Node *> scrollViewChildren = layout->getChildren();
    
    if(scrollViewChildren.size() == 1)
    {
        sender->setPosition(0, scrollViewSize.height - senderSize.height - groupHeightOffset);
        return;
    }
    
    Node *previousChild = (Node *)scrollViewChildren.at(scrollViewChildren.size() - 2);
    
    Point lastStartPoint = previousChild->getPosition();
    Size lastContentSize = previousChild->getBoundingBox().size;
    Point lastEndPoint = Point(lastStartPoint.x + lastContentSize.width, lastStartPoint.y + lastContentSize.height);
    
    //First we try to put the new element under the previous element. If success, then okay,
    //If fails, we replace it next to the previous one on the top.
    
    Point possibleNewPosition = Point(lastStartPoint.x, lastStartPoint.y - senderSize.height);
    if(possibleNewPosition.y >= 0)
    {
        sender->setPosition(Point(possibleNewPosition.x, possibleNewPosition.y - groupHeightOffset * 3));
    }
    else
    {
        possibleNewPosition = Point(lastEndPoint.x, scrollViewSize.height - senderSize.height - groupHeightOffset);
        sender->setPosition(possibleNewPosition);
    }
    
    scrollView->setInnerContainerSize(Size(possibleNewPosition.x + senderSize.width, scrollViewSize.height));
}

NS_AZOOMEE_END
