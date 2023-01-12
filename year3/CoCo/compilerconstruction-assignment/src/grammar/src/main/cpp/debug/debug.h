/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef _DEBUG_H
#define _DEBUG_H
#include <stdarg.h>
#include <stdio.h>

/**
 * We use pmesg to print error messages during grammar parsing
 */

#if defined(__cplusplus)
extern "C" {
#endif

void pmesg(int level, const char* format, ...);

#if defined(__cplusplus)
}
#endif

#endif
