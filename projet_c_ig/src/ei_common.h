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
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#define SAFE_FREE(p) if (p != NULL) { free(p); p = NULL; }
#define SAFE_RESET(p, size) if (p != NULL) { memset(p, 0, size); }
#define ALLOC_TYPE(type) malloc(sizeof(type))
#define CALLOC_TYPE(type) calloc(1, sizeof(type))

#ifndef M_PI
#define M_PI (3.141592653589793)
#endif
#define RAD_TO_DEG(x) ((x*180.0)/(M_PI))

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


#endif
