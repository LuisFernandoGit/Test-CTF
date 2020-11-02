#include <node.h>

Node::Node(proceso process)
{
    this->process = process;
    previous = next = nullptr;
}
