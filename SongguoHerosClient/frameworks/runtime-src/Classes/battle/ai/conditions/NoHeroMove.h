//
//  NoHeroMove.h
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#ifndef __sanguoClient__NoHeroMove__
#define __sanguoClient__NoHeroMove__

#include "../AICondition.h"

class NoHeroMove : public AICondition
{
public:
    NoHeroMove(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};


#endif /* defined(__sanguoClient__NoHeroMove__) */
