//
//  DizzyActionChangeColor.h
//  sanguoClient
//
//  Created by fuchenhao on 5/27/15.
//
//

#ifndef __sanguoClient__DizzyActionChangeColor__
#define __sanguoClient__DizzyActionChangeColor__

#include "DizzyAction.h"

class DizzyActionChangeColor : public DizzyAction
{
public:
    virtual ~DizzyActionChangeColor();
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual void update(entityx::Entity& entity, float dt);
};

#endif /* defined(__sanguoClient__DizzyActionChangeColor__) */
