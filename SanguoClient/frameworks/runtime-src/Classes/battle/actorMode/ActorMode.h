//
//  ActorMode.h
//  Game
//
//  Created by fu.chenhao on 3/5/15.
//
//

#ifndef __Game__ActorMode__
#define __Game__ActorMode__

#include <stdio.h>



struct ActorMode
{
    enum MODE_TYPE
    {
//        DIZZY,
//        NUMBNESS,
//        FREEZE,

        DISABLE_SKILL,
        NO_DISABLE_SKILL,

        NO_DIZZY,

        NO_DMG_PHYSICAL,
        NO_DMG_MAGIC,
        
        NO_DEAD,
        
        TOTAL,
    };
    
    struct ModeData
    {
        MODE_TYPE m_type;
        bool m_actived = false;
        float m_duration = 0;
        float m_leftTime = 0;

        ModeData(MODE_TYPE type, float m_duration):
        m_type(type), m_duration(m_duration), m_leftTime(m_duration)
        {
            
        }
        ~ModeData()
        {
            
        }
        
    };
 
    
    
    static int isModeA_block_ModeB(ActorMode::MODE_TYPE a, ActorMode::MODE_TYPE b);
    
};




#endif /* defined(__Game__ActorMode__) */
