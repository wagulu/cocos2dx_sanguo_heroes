//
//  DizzyActionPlayHitAnimation.h
//  sanguoClient
//
//  Created by fuchenhao on 5/27/15.
//
//

#ifndef __sanguoClient__DizzyActionPlayHitAnimation__
#define __sanguoClient__DizzyActionPlayHitAnimation__

#include "DizzyAction.h"

class DizzyActionPlayHitAnimation : public DizzyAction
{
public:
    virtual ~DizzyActionPlayHitAnimation();
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual void update(entityx::Entity& entity, float dt);
};

#endif /* defined(__sanguoClient__DizzyActionPlayHitAnimation__) */
