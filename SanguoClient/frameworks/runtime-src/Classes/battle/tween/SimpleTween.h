//
//  SimpleTween.h
//  Game
//
//  Created by fu.chenhao on 3/16/15.
//
//

#ifndef __Game__SimpleTween__
#define __Game__SimpleTween__

#include <stdio.h>
#include "2d/CCTweenFunction.h"
#include "cocos2d.h"
struct SimpleTween
{
    SimpleTween(float duration, float s, float e): duration(duration), s(s), e(e), c(s) {};
    ~SimpleTween(){};

protected:
    float updateTime(float dt)
    {
        life += dt;
        if(life >= duration) life = duration;
        float time = life / duration;
        return time;
    }
    
    float calc(float time)
    {
        if (isFinish()) c = e;
        else c = time * (e - s) + s;
        return c;
    }
    
public:
    bool isFinish() { return life >= duration; }
    void init(float dd, float ss, float ee) { duration = dd; s = ss; e = ee; life = 0; c = ss; }
    float current() { return c; }

    virtual float in(float dt)
    {
        return 0;
    }
    virtual float out(float dt)
    {
        return 0;
    }
    virtual float inout(float dt)
    {
        return 0;
    }

private:
    float duration, life = 0;
    float s, e, c;
};

struct SimpleTween_linear : SimpleTween
{
    SimpleTween_linear(float duration, float s, float e): SimpleTween(duration, s, e) {};
    ~SimpleTween_linear(){}

    float in(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::linear(time);
        return calc(time);
    }
    float out(float dt)
    {
        return in(dt);
    }
    float inout(float dt)
    {
        return in(dt);
    }
};

struct SimpleTween_sine : SimpleTween
{
    SimpleTween_sine(float duration, float s, float e): SimpleTween(duration, s, e) {};
    ~SimpleTween_sine(){}
    
    float in(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::sineEaseIn(time);
        return calc(time);
    }
    float out(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::sineEaseOut(time);
        return calc(time);
    }
    float inout(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::sineEaseInOut(time);
        return calc(time);
    }
};

struct SimpleTween_quad : SimpleTween
{
    SimpleTween_quad(float duration, float s, float e): SimpleTween(duration, s, e) {};
    ~SimpleTween_quad(){}
    
    float in(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::quadEaseIn(time);
        return calc(time);
    }
    float out(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::quadEaseOut(time);
        return calc(time);
    }
    float inout(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::quadEaseInOut(time);
        return calc(time);
    }
};

struct SimpleTween_cubic : SimpleTween
{
    SimpleTween_cubic(float duration, float s, float e): SimpleTween(duration, s, e) {};
    ~SimpleTween_cubic(){}
    
    float in(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::cubicEaseIn(time);
        return calc(time);
    }
    float out(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::cubicEaseOut(time);
        return calc(time);
    }
    float inout(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::cubicEaseInOut(time);
        return calc(time);
    }
};

struct SimpleTween_quart : SimpleTween
{
    SimpleTween_quart(float duration, float s, float e): SimpleTween(duration, s, e) {};
    ~SimpleTween_quart(){}
    
    float in(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::quartEaseIn(time);
        return calc(time);
    }
    float out(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::quartEaseOut(time);
        return calc(time);
    }
    float inout(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::quartEaseInOut(time);
        return calc(time);
    }
};

struct SimpleTween_quint : SimpleTween
{
    SimpleTween_quint(float duration, float s, float e): SimpleTween(duration, s, e) {};
    ~SimpleTween_quint(){}
    
    float in(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::quintEaseIn(time);
        return calc(time);
    }
    float out(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::quintEaseOut(time);
        return calc(time);
    }
    float inout(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::quintEaseInOut(time);
        return calc(time);
    }
};

struct SimpleTween_expo : SimpleTween
{
    SimpleTween_expo(float duration, float s, float e): SimpleTween(duration, s, e) {};
    ~SimpleTween_expo(){}
    
    float in(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::expoEaseIn(time);
        return calc(time);
    }
    float out(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::expoEaseOut(time);
        return calc(time);
    }
    float inout(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::expoEaseInOut(time);
        return calc(time);
    }
};

struct SimpleTween_circ : SimpleTween
{
    SimpleTween_circ(float duration, float s, float e): SimpleTween(duration, s, e) {};
    ~SimpleTween_circ(){}
    
    float in(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::circEaseIn(time);
        return calc(time);
    }
    float out(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::circEaseOut(time);
        return calc(time);
    }
    float inout(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::circEaseInOut(time);
        return calc(time);
    }
};

struct SimpleTween_back : SimpleTween
{
    SimpleTween_back(float duration, float s, float e): SimpleTween(duration, s, e) {};
    ~SimpleTween_back(){}
    
    float in(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::backEaseIn(time);
        return calc(time);
    }
    float out(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::backEaseOut(time);
        return calc(time);
    }
    float inout(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::backEaseInOut(time);
        return calc(time);
    }
};

struct SimpleTween_bounce : SimpleTween
{
    SimpleTween_bounce(float duration, float s, float e): SimpleTween(duration, s, e) {};
    ~SimpleTween_bounce(){}
    
    float in(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::bounceEaseIn(time);
        return calc(time);
    }
    float out(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::bounceEaseOut(time);
        return calc(time);
    }
    float inout(float dt)
    {
        float time = updateTime(dt);
        time = cocos2d::tweenfunc::bounceEaseInOut(time);
        return calc(time);
    }
};

// ============

static void sample()
{
    SimpleTween_cubic tween(3, 100, 500);
    
    float dt = 0.016;
    while (true)
    {
        float currentValue = tween.in(dt);
        if(tween.isFinish())
        {
            CCLOG("value at this frame = %f", currentValue);
            break;
        }
    }
}

#endif /* defined(__Game__SimpleTween__) */
