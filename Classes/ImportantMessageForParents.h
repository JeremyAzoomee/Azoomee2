//
//  ImportantMessageForParents.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 14/08/2019.
//

#ifndef ImportantMessageForParents_h
#define ImportantMessageForParents_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/DynamicText.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ImportantMessageForParents : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    cocos2d::ui::Layout* _contentLayout = nullptr;

    void createText();
    
public:
    
    bool init() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(ImportantMessageForParents);
};

NS_AZ_END

#endif /* ImportantMessageForParents_h */
