//
//  TicketRequest.h
//  Game
//
//  Created by fu.chenhao on 3/27/15.
//
//

#ifndef __Game__TicketRequest__
#define __Game__TicketRequest__

#include <stdio.h>

class TicketRequest
{
private:
    float processSpeed;
    float overspeedRate;
    
    int nextTicket = 0;
    int waitTicket = 0;

public:
    
    TicketRequest(float processSpeed, float overspeedRate):
    processSpeed(processSpeed), overspeedRate(overspeedRate)
    {
    };
    virtual ~TicketRequest()
    {
    };

    void reset()
    {
        nextTicket = 0;
        waitTicket = 0;
    }

    bool isWaiting(int ticket) { return ticket >= waitTicket; }
//    int getWaitTicket() { return waitTicket; }
    int queryTicket() { return nextTicket++; };

    void update(float dt);
};

#endif /* defined(__Game__TicketRequest__) */
