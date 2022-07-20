/* pagetables -- A framework to experiment with memory management
 *
 *    settings.h - Various settings
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <stdint.h>

/*
 * Settings that may be changed via command-line arguments.
 * The variables are kept in main.cc
 */

extern bool UseASID;
extern bool LogMemoryAccesses;
extern int ProcessTimeQuantum;
extern uint32_t TLBEntries;


#endif /* __SETTINGS_H__ */
