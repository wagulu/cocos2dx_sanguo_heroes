//
//  BattleResult.h
//  sanguoClient
//
//  Created by fuchenhao on 4/28/15.
//
//

#ifndef __sanguoClient__BattleResult__
#define __sanguoClient__BattleResult__

#include "BattleConfig.h"

using namespace std;

class BattleResult
{
public:
    enum class Reason
    {
        UNKNOWN,
        TIME_UP,
        EXIT_BEFORE_END,
        HERO_DEAD
    };
    
    void prepare();
    inline const string& getResult() { return m_result; };
    inline bool isWin() { return m_win; };
    inline Reason getReason() { return m_reason; };
    
    void setTimeUp();
    void setExitBeforeEnd();
    void setHeroDead(int heroId);
    
private:
    void setMyTeamAllDead();
    void setEnemyTeamAllDead();
    void recordHeroProperty(int heroId);
    
    void prepairHeroResult(BattleConfig::Side side, rapidjson::Value& data, rapidjson::Document::AllocatorType& allocator);
    void prepairSoldierResult(std::vector<entityx::Entity>& entities, rapidjson::Value& data, rapidjson::Document::AllocatorType& allocator);
    
    bool m_leftHeroAllDead = false;
    bool m_rightHeroAllDead = false;
    
    Reason m_reason = Reason::UNKNOWN;
    
    string m_result;
    bool m_win = false;
};

#endif /* defined(__sanguoClient__BattleResult__) */
