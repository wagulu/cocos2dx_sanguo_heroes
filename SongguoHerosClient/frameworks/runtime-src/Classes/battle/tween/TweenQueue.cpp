//
//  TweenQueue.cpp
//  Game
//
//  Created by fu.chenhao on 3/19/15.
//
//

#include "TweenQueue.h"

TweenQueue::TweenQueue()
{
    
}

TweenQueue::~TweenQueue()
{
    for(Segment* s : m_queue)
    {
        CC_SAFE_DELETE(s);
    }
    m_queue.clear();
}

void TweenQueue::pushTween(TARGET type, OPERATION operation, SimpleTween* pTween, float m_delay)
{
    Segment* s = new Segment(type, operation, pTween, m_delay);
    m_queue.push_back(s);
}

void TweenQueue::clearTween()
{
    for(Segment* s : m_queue)
    {
        CC_SAFE_DELETE(s);
    }
    m_queue.clear();
}

std::vector<TweenQueueResult>& TweenQueue::update(float dt)
{
    m_result.clear();
//    std::vector<TweenQueueResult> result;
    
    for(long i = m_queue.size() - 1; i >= 0; --i)
    {
        Segment* s = m_queue[i];
        s->m_delay -= dt;
        if(s->m_delay <= 0)
        {
            TweenQueueResult r;
            
            r.m_lastTarget = s->m_target;
            r.m_lastOperation = s->m_operation;
            
            if(r.m_lastOperation == IN)
            {
                r.m_lastValue = s->m_pTween->in(dt);
            }
            else if(r.m_lastOperation == OUT)
            {
                r.m_lastValue = s->m_pTween->out(dt);
            }
            else if(r.m_lastOperation == INOUT)
            {
                r.m_lastValue = s->m_pTween->inout(dt);
            }
            else
            {
                r.m_lastValue = 0;
            }
            
            if(s->m_pTween->isFinish())
            {
                m_queue.erase(std::find(m_queue.begin(), m_queue.end(), s));
                CC_SAFE_DELETE(s);
            }
    
            m_result.insert(m_result.begin(), r);
        }
        
    }
    
    return m_result;
    
}
