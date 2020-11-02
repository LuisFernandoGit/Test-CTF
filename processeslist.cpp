#include <processeslist.h>

void ProcessesList::setUp()
{
    first = last = nullptr;
}

void ProcessesList::push_back(Proceso process)
{
    Node *newProcess = new Node(process);
    if(first == nullptr)
    {
        first = last = newProcess;
    }
    else
    {
        newProcess->previous = last;
        last->next = newProcess;
        last = newProcess;;
    }
}

Proceso ProcessesList::pop()
{
    Node *temp = first;
    Proceso process = first->process;
    if (first == last)
    {
        first = last = nullptr;
    }
    else
    {
        first = first->next;
        first->previous = nullptr;
    }
    delete (temp);
    return  process;
}

bool ProcessesList::empty()
{
    if(first)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int ProcessesList::size()
{
    int size = 0;
    Node *aux = first;
    while(aux)
    {
        size++;
        aux = aux->next;
    }
    return size;
}

void ProcessesList::erase(Proceso process)
{
    Node *aux = first;
    while(aux)
    {
        if(aux->process.ID == process.ID)
        {
            break;
        }
        aux = aux->next;
    }
    if(aux == first && aux == last)
    {
        first = last = nullptr;
    }
    else if(aux == first)
    {
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

Proceso ProcessesList::popPos(Proceso process)
{
    Node *aux = first;
    while(aux)
    {
        if(aux->process.ID == process.ID)
        {
            break;
        }
        aux = aux->next;
    }
    if(aux == first && aux == last)
    {
        first = last = nullptr;
    }
    else if(aux == first)
    {
        first = first->next;
        first->previous = nullptr;
    }
    else if(aux == last)
    {
        last = last->previous;
        last = last->next;
    }
    else
    {
        aux->previous->next = aux->next;
        aux->next->previous = aux->previous;
    }
    delete (aux);
    return process;
}
