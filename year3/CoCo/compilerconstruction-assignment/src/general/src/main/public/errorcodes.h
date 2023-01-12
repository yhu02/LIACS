/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_ERRORCODES_H
#define BUILD_ERRORCODES_H

/**
 * This file contains an enum to more provide more specific returncodes
 * This allows us to determine in which stage of the compiler an error occurred
 */

typedef enum errorcodes {
    IO_ERROR = 1,           // input-output error
    GRAMMAR_ERROR = 2,      // error in grammar-parsing stage
    IC_ERROR = 3,
    MC_ERROR = 4
} ERRORCODES;

#endif //BUILD_ERRORCODES_H
