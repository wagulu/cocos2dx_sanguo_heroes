//
//  StrategyControl.h
//  Game
//
//  Created by fu.chenhao on 3/6/15.
//
//

#ifndef __Game__StrategyControl__
#define __Game__StrategyControl__

#include <stdio.h>

class StrategyControl
{
    
public:
    StrategyControl();
    ~StrategyControl();

    enum SOLDIER_CONTROL_TYPE
    {
        SOLDIER_STOP,
        SOLDIER_ATTACK_SOLDIER,
        SOLDIER_ATTACK_HERO,
        SOLDIER_PROTECT_HERO,
        SOLDIER_RECENTER,
        SOLDIER_DEAPRT,
        SOLDIER_BACK,
        
        SOLDIER_TOTAL,
        SOLDIER_BATTLE_END
    };
    
    enum HERO_CONTROL_TYPE
    {
        HERO_STOP,
        HERO_RUSH_OUT,
        
        HERO_TOTAL,
        HERO_BATTLE_END
    };
    
    SOLDIER_CONTROL_TYPE getSoldierControl(bool left) {  return left? m_soldierControlLeft: m_soldierControlRight; }
    void setSoldierControl(SOLDIER_CONTROL_TYPE control, bool left);
    
    void setHeroControl(HERO_CONTROL_TYPE control, int heroId);
    
    void setBattleEndForAll();
private:
    SOLDIER_CONTROL_TYPE m_soldierControlLeft = SOLDIER_STOP;
    SOLDIER_CONTROL_TYPE m_soldierControlRight = SOLDIER_STOP;    
};

#endif /* defined(__Game__StrategyControl__) */
