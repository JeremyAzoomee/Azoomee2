#ifndef AzoomeeCommon_LayoutParams_h
#define AzoomeeCommon_LayoutParams_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_BEGIN

// Some convenience methods for creating Layout params

#pragma mark - RelativeLayout

inline cocos2d::ui::RelativeLayoutParameter* CreateRelativeLayoutParam(cocos2d::ui::RelativeLayoutParameter::RelativeAlign align,
                                                                       cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    cocos2d::ui::RelativeLayoutParameter* params = cocos2d::ui::RelativeLayoutParameter::create();
    params->setAlign(align);
    params->setMargin(margin);
    return params;
}

inline cocos2d::ui::RelativeLayoutParameter* CreateTopLeftRelativeLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateRelativeLayoutParam(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT, margin);
}

inline cocos2d::ui::RelativeLayoutParameter* CreateTopCenterRelativeLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateRelativeLayoutParam(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL, margin);
}

inline cocos2d::ui::RelativeLayoutParameter* CreateTopRightRelativeLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateRelativeLayoutParam(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT, margin);
}

inline cocos2d::ui::RelativeLayoutParameter* CreateLeftCenterRelativeLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateRelativeLayoutParam(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL, margin);
}

inline cocos2d::ui::RelativeLayoutParameter* CreateCenterRelativeLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateRelativeLayoutParam(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT, margin);
}

inline cocos2d::ui::RelativeLayoutParameter* CreateRightCenterRelativeLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateRelativeLayoutParam(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL, margin);
}

inline cocos2d::ui::RelativeLayoutParameter* CreateBottomLeftRelativeLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateRelativeLayoutParam(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM, margin);
}

inline cocos2d::ui::RelativeLayoutParameter* CreateBottomCenterRelativeLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateRelativeLayoutParam(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL, margin);
}

inline cocos2d::ui::RelativeLayoutParameter* CreateBottomRightRelativeLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateRelativeLayoutParam(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM, margin);
}

#pragma mark - LinearLayout

inline cocos2d::ui::LinearLayoutParameter* CreateLinearLayoutParam(cocos2d::ui::LinearLayoutParameter::LinearGravity gravity,
                                                                   cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    cocos2d::ui::LinearLayoutParameter* params = cocos2d::ui::LinearLayoutParameter::create();
    params->setGravity(gravity);
    params->setMargin(margin);
    return params;
}

inline cocos2d::ui::LinearLayoutParameter* CreateLeftLinearLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateLinearLayoutParam(cocos2d::ui::LinearLayoutParameter::LinearGravity::LEFT, margin);
}

inline cocos2d::ui::LinearLayoutParameter* CreateTopLinearLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateLinearLayoutParam(cocos2d::ui::LinearLayoutParameter::LinearGravity::TOP, margin);
}

inline cocos2d::ui::LinearLayoutParameter* CreateRightLinearLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateLinearLayoutParam(cocos2d::ui::LinearLayoutParameter::LinearGravity::RIGHT, margin);
}

inline cocos2d::ui::LinearLayoutParameter* CreateBottomLinearLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateLinearLayoutParam(cocos2d::ui::LinearLayoutParameter::LinearGravity::BOTTOM, margin);
}

inline cocos2d::ui::LinearLayoutParameter* CreateCenterVerticalLinearLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateLinearLayoutParam(cocos2d::ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL, margin);
}

inline cocos2d::ui::LinearLayoutParameter* CreateCenterHorizontalLinearLayoutParam(cocos2d::ui::Margin margin = cocos2d::ui::Margin(0, 0, 0, 0))
{
    return CreateLinearLayoutParam(cocos2d::ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL, margin);
}

#pragma mark - % Aspect Ratio

/// Will apply aspect ratio to the widget's current height to calculate the width in % of widget
/// Use this if you need to use size percent on one dimension and retain the aspect ratio
/// This must be called whenever the size of the widget's parent changes
inline void SetSizePercentWidthAspectRatio(cocos2d::ui::Widget* widget, const cocos2d::Vec2& aspect)
{
    const float aspectRatio = aspect.x / aspect.y;
    const cocos2d::Size& parentSize = widget->getParent()->getContentSize();
    cocos2d::Vec2 sizePct = widget->getSizePercent();
    // Calc width in pt from aspect ratio
    const float widthPt = parentSize.height * sizePct.y * aspectRatio;
    sizePct.x = widthPt / parentSize.width;
    widget->setSizePercent(sizePct);
}

/// See CalculateSizePercentWidthAspectRatio
/// Uses a square aspect ratio
inline void SetSizePercentWidthForSquareAspectRatio(cocos2d::ui::Widget* widget)
{
    SetSizePercentWidthAspectRatio(widget, cocos2d::Vec2(1.0f, 1.0f));
}

/// Will apply aspect ratio to the widget's current width to calculate the height in % of widget
/// Use this if you need to use size percent on one dimension and retain the aspect ratio
/// This must be called whenever the size of the widget's parent changes
inline void SetSizePercentHeightAspectRatio(cocos2d::ui::Widget* widget, const cocos2d::Vec2& aspect)
{
    const float aspectRatio = aspect.y / aspect.x;
    const cocos2d::Size& parentSize = widget->getParent()->getContentSize();
    cocos2d::Vec2 sizePct = widget->getSizePercent();
    // Calc width in pt from aspect ratio
    const float heightPt = parentSize.width * sizePct.x * aspectRatio;
    sizePct.y = heightPt / parentSize.height;
    widget->setSizePercent(sizePct);
}

/// See CalculateSizePercentHeightAspectRatio
/// Uses a square aspect ratio
inline void SetSizePercentHeightForSquareAspectRatio(cocos2d::ui::Widget* widget)
{
    SetSizePercentHeightAspectRatio(widget, cocos2d::Vec2(1.0f, 1.0f));
}

NS_AZOOMEE_END

#endif
