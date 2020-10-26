#ifndef PROCESSESLIST_H
#define PROCESSESLIST_H

#include "proceso.h"
#include "node.h"

class processesList
{
public:
    node *first, *last;

    void setUp();
    void push_back(proceso process);
    proceso pop();
    bool empty();
    int size();
    void erase(proceso process);
    proceso popPos(proceso process);

};

#endif // PROCESSESLIST_H
