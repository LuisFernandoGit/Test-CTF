#include "processeslist.h"

void processesList::setUp()
{
    first = last = nullptr;
}

void processesList::push_back(proceso process)
{
    node *newProcess = new node(process);
    if(first == nullptr)
        first = last = newProcess;
    else{
        newProcess->previous = last;
        last->next = newProcess;
        last = newProcess;;
    }
}

proceso processesList::pop()
{
    node *temp = first;
    proceso process = first->process;
    if (first == last)
        first = last = nullptr;
    else{
        first = first->next;
        first->previous = nullptr;
    }
    delete (temp);
    return  process;
}

bool processesList::empty()
{
    if(first)
        return false;
    else
        return true;
}

int processesList::size()
{
    int size = 0;
    node *aux = first;
    while(aux){
        size++;
        aux = aux->next;
    }
    return size;
}

void processesList::erase(proceso process)
{
    node *aux = first;
    while(aux){
        if(aux->process.ID == process.ID)
            break;
        aux = aux->next;
    }
    if(aux == first && aux == last)
        first = last = nullptr;
    else if(aux == first){
        first = first->next;
        first->previous = nullptr;
    }
    else if(aux == last){
        last = last->previous;
        last = last->next;
    }
    else{
        aux->previous->next = aux->next;
        aux->next->previous = aux->previous;
    }
    delete (aux);
}

proceso processesList::popPos(proceso process)
{
    node *aux = first;
    while(aux){
        if(aux->process.ID == process.ID)
            break;
        aux = aux->next;
    }
    if(aux == first && aux == last)
        first = last = nullptr;
    else if(aux == first){
        first = first->next;
        first->previous = nullptr;
    }
    else if(aux == last){
        last = last->previous;
        last = last->next;
    }
    else{
        aux->previous->next = aux->next;
        aux->next->previous = aux->previous;
    }
    delete (aux);
    return process;
}
