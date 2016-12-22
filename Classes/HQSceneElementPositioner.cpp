#include "HQSceneElementPositioner.h"
#include "ui/UIScrollView.h"

using namespace cocos2d;

void HQSceneElementPositioner::positionHQSceneElement(cocos2d::Layer* sender)
{
    //DEBUG ONLY
    sender->setName("TOBEPOSED");
    
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
    if(possibleNewPosition.y > 0)
    {
        sender->setPosition(possibleNewPosition);
    }
    else
    {
        possibleNewPosition = Point(lastEndPoint.x, scrollViewSize.height - senderSize.height);
        sender->setPosition(possibleNewPosition);
    }
    
    scrollView->setInnerContainerSize(Size(possibleNewPosition.x + senderSize.width, scrollViewSize.height));
    
    CCLOG("Prev element name: %s", previousChild->getName().c_str());
    CCLOG("Item number: %zd", scrollViewChildren.size());
    CCLOG("Item position set to: %f, %f", possibleNewPosition.x, possibleNewPosition.y);
    
    //DEBUG ONLY
    sender->setName("POSED");
}

/*

 Point HQScene::getItemPositionForBidirectionalScrollView(int highlight)
 {
 Size baseSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(this->getName());
 Point resultPoint = Point(50,50);
 int allocatedAmount = 1;
 
 if((highlight == 1)||(highlight == 2))
 {
 if(scrollViewSpaceAllocation.size() % 2 == 1) scrollViewSpaceAllocation.push_back(false); //if the last item is on the down side, we have to skip a column, as big item is coming.
 allocatedAmount = highlight * 2;                                                          //we need to allocate the amount of base-spaces - 2 for highlight = 1, 4 for highlight = 2
 }
 
 resultPoint = Point(20 + scrollViewSpaceAllocation.size() / 2 * baseSize.width, scrollViewSpaceAllocation.size() % 2 * baseSize.height + 20);
 for(int i = 0; i < allocatedAmount; i++) scrollViewSpaceAllocation.push_back(true);
 
 return resultPoint;
 }

 
*/
