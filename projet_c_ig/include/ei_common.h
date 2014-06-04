/**
 * @file	ei_utils.h
 *
 * @brief	Common header
 */

#ifndef EI_COMMON_H
#define EI_COMMON_H

#include <stdio.h>
#include <stdlib.h>

#define SAFE_FREE(p) if (p) { free(p); p = NULL; }


#endif
