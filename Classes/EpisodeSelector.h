//
//  EpisodeSelector.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 20/09/2019.
//

#ifndef EpisodeSelector_h
#define EpisodeSelector_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObject.h>

NS_AZOOMEE_BEGIN

class EpisodeSelector : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    cocos2d::ui::Layout* _contentLayout = nullptr;
    cocos2d::ui::Layout* _headerLayout = nullptr;
    cocos2d::ui::Layout* _divider = nullptr;
    cocos2d::ui::ListView* _episodeListView = nullptr;
    cocos2d::Vector<cocos2d::ui::Layout*> _episodeBars;
    
    HQDataObjectRef _hqData = nullptr;
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setHqData(const HQDataObjectRef& hqData);
    
    CREATE_FUNC(EpisodeSelector);
};

NS_AZOOMEE_END

#endif /* EpisodeSelector_h */
