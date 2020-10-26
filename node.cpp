#include "node.h"

node::node(proceso process)
{
    this->process = process;
    previous = next = nullptr;
}
