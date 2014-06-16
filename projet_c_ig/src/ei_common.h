/**
 *  @file       ei_common.h
 *
 *  @brief      Common header. Contains defines for allocations and frequent operations.
 *
 *  \author
 *  Created by Antoine DELAITE, Eric BUREL, Léo GOUTTEFARDE on 05.06.14.
 *  Copyright 2014 Ensimag. All rights reserved.
 */

#ifndef EI_COMMON_H
#define EI_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#ifndef NDEBUG
#define LEAK_TRACKER
#endif

// Conversion flottant entier
#define F2I(x) (int)floor(x)
#define I2F(x) (float)(x)

#include "ei_memory.h"


#define SAFE_RESET(p, size) do { if (p != NULL) { memset(p, 0, size); } } while (0)
#define ALLOC_TYPE(type) malloc(sizeof(type))
#define SAFE_ALLOC(var, type) do { if (var == NULL) { var = CALLOC_TYPE(type); } } while (0)
#define UNUSED(arg) ((void)(arg))

#ifndef M_PI
#define M_PI (3.141592653589793)
#endif
#define RAD_TO_DEG(x) ((x*180.0)/(M_PI))

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


#endif
