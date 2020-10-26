#ifndef NODE_H
#define NODE_H

#include "proceso.h"

class node
{
public:

    proceso process;

    node *next, *previous;

    node(proceso process);
    friend class list;
};

#endif // NODE_H
