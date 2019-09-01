//
//  BattleCocosEvent.h
//  Game
//
//  Created by fu.chenhao on 3/25/15.
//
//

#ifndef __Game__BattleCocosEvent__
#define __Game__BattleCocosEvent__

#include <stdio.h>
#include "BattleEvent.h"

#define COCOS_EVENT(EVENT) struct EVENT : public cocos2d::EventCustom

class BattleCocosEvent
{
public:
    COCOS_EVENT(PopupCombo)
    {
        int *pTime;

        PopupCombo(int time):
        cocos2d::EventCustom(EVENT_POPUPCOMBO)
        {
            pTime = new int;
            *pTime = time;
            
            setUserData(pTime);
        }
        ~PopupCombo()
        {
            CC_SAFE_DELETE(pTime);
        }
    };
};

#endif /* defined(__Game__BattleCocosEvent__) */
