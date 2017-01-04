#ifndef __TEXT_INPUT_LAYER_H__
#define __TEXT_INPUT_LAYER_H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"

USING_NS_CC;

class TextInputLayerDelegate
{
public:
    virtual void textInputIsValid(Layer* inputLayer) = 0;
    virtual void textInputNotValid(Layer* inputLayer) = 0;
};

class TextInputLayer
{
private:
    TextInputLayerDelegate* textInputDelegate;
    
    ui::EditBox* editBox;
    ui::Scale9Sprite* editBoxArea;
    
    const float editBoxCurveWidth = 60;
    
    int textInputType;
    
public:
    ui::EditBox* create(Size inputBoxSize, int textInputType);
    
    bool inputIsValid();
    
    const int TextInputIsEmail = 0;
    const int TextInputIsPassword = 1;
    const int TextInputIsPin = 2;
    
};

#endif

/*
 https://gist.github.com/syuhari/5425221
 
 
class MyClassDelegate
{
public:
    virtual void myDelegateMethod() = 0;
};

class MyClass
{
public:
    static void doSomething(int a, int b, MyDelegate *delegate)
    {
        m_delegate = delegate;
        
        //doSomething........
        
        this->m_delegate->myDelegateMethod();
    }
private:
    MyDelegate* m_delegate;
};

class MyUserClass : public MyClassDelegate
{
    void calc()
    {
        MyClass::doSomething(1, 2, this);
    }
    void myDelegateMethod()
    {
        ;
    }
}; */
