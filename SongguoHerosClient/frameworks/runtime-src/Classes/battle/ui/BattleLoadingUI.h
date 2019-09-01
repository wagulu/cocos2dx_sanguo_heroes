//
//  BattleLoadingUI.h
//  sanguoGame
//
//  Created by fuchenhao on 7/20/15.
//
//

#ifndef __sanguoGame__BattleLoadingUI__
#define __sanguoGame__BattleLoadingUI__

#include <vector>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "entityx.h"
#include "BattleEvent.h"
#include "BattleConfig.h"

class BattleLoadingUI : public cocos2d::Layer
{
public:
    BattleLoadingUI();
    
    void updateProgress(float percent);
    
    CREATE_FUNC(BattleLoadingUI);
};

#endif /* defined(__sanguoGame__BattleLoadingUI__) */
