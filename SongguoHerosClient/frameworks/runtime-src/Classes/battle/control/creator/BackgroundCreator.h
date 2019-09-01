//
//  BackgroundCreator.h
//  Game
//
//  Created by fuchenhao on 3/9/15.
//
//

#ifndef __Game__BackgroundCreator__
#define __Game__BackgroundCreator__

#include "BattleEntity.h"
#include "MapHelper.h"
#include "../../BattleEvent.h"
#include "../../BattleStage.h"

class BackgroundCreator
{
public:
    
    BackgroundCreator(BattleStage* stage);
    ~BackgroundCreator();
    
    void addBackground(const std::string& floor, const std::string& bg1, const std::string& bg2_1, const std::string& bg2_2, const std::string& front);
    
private:
    
    BattleStage* m_stage;
    
    void addDrawLine();
};

#endif /* defined(__Game__BackgroundCreator__) */
