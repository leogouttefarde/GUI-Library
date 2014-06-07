/**
 * @file	ei_utils.h
 *
 * @brief	Common header
 */

#ifndef EI_COMMON_H
#define EI_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE_FREE(p) if (p != NULL) { free(p); p = NULL; }
#define SAFE_RESET(p, size) if (p != NULL) { memset(p, 0, size); }
#define ALLOC_TYPE(type) malloc(sizeof(type))

void ei_event_init();

#endif
