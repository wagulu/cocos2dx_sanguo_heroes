//
//  ActiveObject.h
//  Game
//
//  Created by fu.chenhao on 2/13/15.
//
//

#ifndef __ACTIVE_OBJECT_H__
#define __ACTIVE_OBJECT_H__

#include <stdio.h>

class ActiveObject
{
private:
    bool m_dead;
    
public:
    ActiveObject();
    virtual ~ActiveObject();
    
    inline void remove()   {   m_dead = true;  }
    inline bool shouldRemove()    {   return m_dead;  }
};

#endif /* defined(__ACTIVE_OBJECT_H__) */
