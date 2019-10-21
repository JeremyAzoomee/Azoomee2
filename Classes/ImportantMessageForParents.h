//
//  ImportantMessageForParents.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 14/08/2019.
//

#ifndef ImportantMessageForParents_h
#define ImportantMessageForParents_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/DynamicText.h>

NS_AZOOMEE_BEGIN

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

NS_AZOOMEE_END

#endif /* ImportantMessageForParents_h */
