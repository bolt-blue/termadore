#include "tdsignal.h"
#include "screen.h"

#include <stdlib.h>

void sig_handler(int signo)
{
    switch(signo) {
    case SIGINT:
        cleanup();
        exit(1);
        break;
    default:
        break;
    }
}
