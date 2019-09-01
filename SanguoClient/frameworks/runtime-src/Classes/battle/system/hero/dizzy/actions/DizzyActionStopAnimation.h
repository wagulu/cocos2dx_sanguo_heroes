//
//  DizzyActionStopAnimation.h
//  sanguoClient
//
//  Created by fuchenhao on 5/27/15.
//
//

#ifndef __sanguoClient__DizzyActionStopAnimation__
#define __sanguoClient__DizzyActionStopAnimation__

#include "DizzyAction.h"

class DizzyActionStopAnimation : public DizzyAction
{
public:
    virtual ~DizzyActionStopAnimation();
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual void update(entityx::Entity& entity, float dt);
};

#endif /* defined(__sanguoClient__DizzyActionStopAnimation__) */
