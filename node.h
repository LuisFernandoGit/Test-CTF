#ifndef NODE_H
#define NODE_H

#include <proceso.h>

class Node
{
public:

    proceso process;

    Node *next, *previous;

    Node(proceso process);
    friend class list;
};

#endif // NODE_H
