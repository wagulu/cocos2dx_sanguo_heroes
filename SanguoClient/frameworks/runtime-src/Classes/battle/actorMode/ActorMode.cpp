//
//  ActorMode.cpp
//  Game
//
//  Created by fu.chenhao on 3/5/15.
//
//

#include "ActorMode.h"


static int _MODE_MUTEX_TABLE[ActorMode::TOTAL][ActorMode::TOTAL] =
{
                        /*DISABLE_SKILL   NO_DISABLE_SKILL    NO_DIZZY    NO_DMG_PHYSICAL NO_DMG_MAGIC    NO_DEAD*/
    
//    /*DIZZY*/           {   0,              0,              0,                  0,              0,          0,},
//    /*NUMBNESS*/        {   0,              0,              0,                  0,              0,          0,},
//    /*FREEZE*/          {   0,              0,              0,                  0,              0,          0,},
    
    /*DISABLE_SKILL*/       {   0,              0,              0,                  0,          0,          0,},
    /*NO_DISABLE_SKILL*/    {   0,              0,              0,                  0,          0,          0,},
    
    /*NO_DIZZY*/            {   0,              0,              0,                  0,          0,          0,},
    
    /*NO_DMG_PHYSICAL*/     {   0,              0,              0,                  0,          0,          0,},
    /*NO_DMG_MAGIC*/        {   0,              0,              0,                  0,          0,          0,},
    
    /*NO_DEAD*/             {   0,              0,              0,                  0,          0,          0,},

};



int ActorMode::isModeA_block_ModeB(ActorMode::MODE_TYPE a, ActorMode::MODE_TYPE b)
{
    return _MODE_MUTEX_TABLE[a][b];
}
