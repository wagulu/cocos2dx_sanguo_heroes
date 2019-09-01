//
//  TicketRequest.cpp
//  Game
//
//  Created by fu.chenhao on 3/27/15.
//
//

#include "TicketRequest.h"

void TicketRequest::update(float dt)
{
    int c = processSpeed * dt;
    int n = nextTicket - waitTicket;
    if(n > c)
    {
        n = c + (n - c) * overspeedRate;
    }

    waitTicket += n;
    if(waitTicket > nextTicket)
    {
        waitTicket = nextTicket;
    }
}
