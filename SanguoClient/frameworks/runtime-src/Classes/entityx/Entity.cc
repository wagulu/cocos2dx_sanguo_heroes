/*
 * Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#include <algorithm>
#include "Entity.h"

namespace entityx {

const Entity::Id Entity::INVALID;
BaseComponent::Family BaseComponent::family_counter_ = 0;

void Entity::invalidate() {
  id_ = INVALID;
  manager_ = nullptr;
}

    //fu.chenhao, delay destroy entities
void Entity::destroy() {
    if(ENTITY_USE_ASYNCHRONOUS_DESTROY)
    {
        assert(valid());
//        assert(std::find(manager_->waitToDestroiedEntities.begin(), manager_->waitToDestroiedEntities.end(), *this) == manager_->waitToDestroiedEntities.end());
        manager_->waitToDestroiedEntities.push_back(*this);
        assert(manager_->waitToDestroiedEntities.size() < 4096);
    }
    else
    {
        _realDestroy();
    }
}
    
    void Entity::_realDestroy()
    {
        assert(valid());
        manager_->destroy(id_);
        invalidate();
        
    }


std::bitset<entityx::MAX_COMPONENTS> Entity::component_mask() const {
  return manager_->component_mask(id_);
}

EntityManager::EntityManager(EventManager &event_manager) : event_manager_(event_manager) {
}

EntityManager::~EntityManager() {
  reset();
}

void EntityManager::reset() {
  for (BasePool *pool : component_pools_) {
    if (pool) delete pool;
  }
  component_pools_.clear();
  entity_component_mask_.clear();
  entity_version_.clear();
  free_list_.clear();
  index_counter_ = 0;
}

    
    //fu.chenhao
    void EntityManager::cleanWaitToDestroiedEntities()
    {
        for(Entity entity : waitToDestroiedEntities)
        {
            if(entity.valid())
            {
                entity._realDestroy();
            }
        }
        waitToDestroiedEntities.clear();
    }


    

    
    
}  // namespace entityx
