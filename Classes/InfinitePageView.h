//
//  InfinitePageView.h
//  MediaCarouselTest
//
//  Created by Macauley on 13/08/2018.
//

#ifndef InfinitePageView_h
#define InfinitePageView_h


#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class InfinitePageView : public cocos2d::ui::PageView
{
    typedef cocos2d::ui::PageView Super;
private:
    std::vector<std::string> _pageDataList;
    int _currentIndex = 1;
    int _nextIndex = 2;
    int _prevIndex = 0;
    
    bool _pageTurned = false;
    
    virtual void update(float deltaT) override;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void setPageData(const std::vector<std::string>& pageData);
    
    CREATE_FUNC(InfinitePageView);
};

NS_AZOOMEE_END

#endif /* InfinitePageView_h */
