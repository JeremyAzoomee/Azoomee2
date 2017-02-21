#include "HQSceneElementPositioner.h"
#include "ui/UIScrollView.h"

using namespace cocos2d;

void HQSceneElementPositioner::positionHQSceneElement(cocos2d::Layer* sender)
{
    cocos2d::ui::ScrollView *scrollView = (cocos2d::ui::ScrollView *)sender->getParent()->getParent();
    Node *layout = (Node *)sender->getParent();
    
    Size scrollViewSize = scrollView->getInnerContainerSize();
    Size senderSize = sender->getContentSize();
    Vector<Node *> scrollViewChildren = layout->getChildren();
    
    if(scrollViewChildren.size() == 1)
    {
        sender->setPosition(0, scrollViewSize.height - senderSize.height);
        return;
    }
    
    Node *previousChild = (Node *)scrollViewChildren.at(scrollViewChildren.size() - 2);
    
    Point lastStartPoint = previousChild->getPosition();
    Size lastContentSize = previousChild->getContentSize();
    Point lastEndPoint = Point(lastStartPoint.x + lastContentSize.width, lastStartPoint.y + lastContentSize.height);
    
    //First we try to put the new element under the previous element. If success, then okay,
    //If fails, we replace it next to the previous one on the top.
    
    Point possibleNewPosition = Point(lastStartPoint.x, lastStartPoint.y - senderSize.height);
    if(possibleNewPosition.y >= 0)
    {
        sender->setPosition(possibleNewPosition);
    }
    else
    {
        possibleNewPosition = Point(lastEndPoint.x, scrollViewSize.height - senderSize.height);
        sender->setPosition(possibleNewPosition);
    }
    
    CCLOG("positionx: %f, positiony: %f", sender->getPosition().x, sender->getPosition().y);
    
    scrollView->setInnerContainerSize(Size(possibleNewPosition.x + senderSize.width, scrollViewSize.height));
}
