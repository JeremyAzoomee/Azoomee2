//
//  MeHQFavourites.h
//  azoomee2
//
//  Created by Macauley on 23/05/2018.
//

#ifndef MeHQFavourites_h
#define MeHQFavourites_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class MeHQFavourites : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void()> RefreshLayoutCallback;
private:
    
    cocos2d::ui::Layout* _carouselLayout = nullptr;
    
    RefreshLayoutCallback _refreshCallback = nullptr;
    
    bool _editEnabled = false;
    
    void buildEmptyCarousel();
    
protected:
    virtual void onSizeChanged() override;
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(MeHQFavourites);
    
    void setRefreshCallback(const RefreshLayoutCallback& callback);
};

NS_AZOOMEE_END

#endif /* MeHQFavourites_h */
