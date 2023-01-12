/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_ASSIGNMENTTYPE_H
#define BUILD_ASSIGNMENTTYPE_H

#include <cstdint>
#include <iostream>

/**
 * This file contains an enum to keep track of which assignments are currently activated
 * At this moment, the activation of an assignment also enforces all previous assignments to be activated,
 *  e.g. if assignment 3 is activated, then assignments 1 and 2 are also automatically activated
 *
 * The CURRENT_ASSIGNMENT is represented as a bitstring where zero (0) indicates an assignment is not activated,
 * while an one (1) an assignment is activated,
 *  e.g. if assignments 1, 2, and 5 are activated then CURRENT_ASSIGNMENT becomes 10011
 *
 * One can use bitwise-arithmetics to extract the activated assignments,
 *  e.g. 'CURRENT_ASSIGNMENT & ASS_FOUR' checks if assignment 4 is activated
 */

typedef enum assignmenttype {
    ASS_ONE = 1 << 0,
    ASS_TWO = 1 << 1,
    ASS_THREE = 1 << 2,
    ASS_FOUR = 1 << 3,
    ASS_FIVE = 1 << 4
} AssignmentType;

#ifdef ASSIGNMENT
#if ASSIGNMENT == 1
    #define CURRENT_ASSIGNMENT ASS_ONE
#elif ASSIGNMENT == 2
    #define CURRENT_ASSIGNMENT (ASS_ONE | ASS_TWO)
#elif ASSIGNMENT == 3
    #define CURRENT_ASSIGNMENT (ASS_ONE | ASS_TWO | ASS_THREE)
#elif ASSIGNMENT == 4
    #define CURRENT_ASSIGNMENT (ASS_ONE | ASS_TWO | ASS_THREE | ASS_FOUR)
#else
    #define CURRENT_ASSIGNMENT (ASS_ONE | ASS_TWO | ASS_THREE | ASS_FOUR | ASS_FIVE)
#endif
#endif

#endif //BUILD_ASSIGNMENTTYPE_H
