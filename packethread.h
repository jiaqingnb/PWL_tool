#ifndef PACKETHREAD_H
#define PACKETHREAD_H

#include "packet_loss.h"

extern PWlloss* Loss;

typedef struct
{
    uint32_t data;
    uint32_t* link;
}s_queue;

class packethread
{
public:
    packethread();
    void run();

};

#endif // PACKETHREAD_H
