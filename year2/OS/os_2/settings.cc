/* pagetables -- A framework to experiment with memory management
 *
 *    settings.cc - Settings for Program driver
 *
 * Copyright (C) 2017,2018  Leiden University, The Netherlands.
 */

#include "settings.h"

/* Variables to store settings that may be changed via command-line args. */
bool UseASID = false;
bool LogMemoryAccesses = false;
int ProcessTimeQuantum = 1000;
uint32_t TLBEntries = 32;
