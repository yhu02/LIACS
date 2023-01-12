/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/



#include "debug.h"

extern int msglevel; /* the higher, the more messages... */

void pmesg(int level, const char* format, ...) {
    va_list args;

    if (level <= msglevel) {
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}
