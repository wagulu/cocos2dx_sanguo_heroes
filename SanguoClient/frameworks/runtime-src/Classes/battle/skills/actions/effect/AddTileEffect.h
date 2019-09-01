//
//  AddTileEffect.h
//  sanguoClient
//
//  Created by fuchenhao on 4/24/15.
//
//

#ifndef __sanguoClient__AddTileEffect__
#define __sanguoClient__AddTileEffect__

#include "SkillAction.h"
#include "BattleConfig.h"

class AddTileEffect : public SkillAction
{
public:
    struct EffectConfigData
    {
        string* pEffect;
        float duration;
        float delay;
        float randomOffset;
        bool reverse;
        BattleConfig::TileResults tileResults;
        
        EffectConfigData() : pEffect(nullptr), duration(0), reverse(false), delay(0), randomOffset(0) {}
    };
    
    void fireAction(entityx::EntityManager &es) override;

protected:
    bool parseConfig(EffectConfigData& config);
};

#endif /* defined(__sanguoClient__AddTileEffect__) */
