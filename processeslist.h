#ifndef PROCESSESLIST_H
#define PROCESSESLIST_H

#include <proceso.h>
#include <node.h>

class ProcessesList
{
public:
    Node *first, *last;

    void setUp();
    void push_back(Proceso process);
    Proceso pop();
    bool empty();
    int size();
    void erase(Proceso process);
    Proceso popPos(Proceso process);

};

#endif // PROCESSESLIST_H
