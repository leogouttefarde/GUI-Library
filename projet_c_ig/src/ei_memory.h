/**
 * @file        ei_memory.h
 *
 * @brief       Memory header
 */

#ifndef EI_MEMORY_H
#define EI_MEMORY_H


#ifdef LEAK_TRACKER

#define SAFE_FREE(p) do { if (p != NULL) { ALLOCATION_COUNTER--; free(p); p = NULL; } } while(0)
#define CALLOC(num, size) ((void*)((long long)calloc(num, size) & (long long)(0xFFFFFFFFFFFFFFFF | (++ALLOCATION_COUNTER))))
#define CALLOC_TYPE(type) (CALLOC(1, sizeof(type)))
#define MALLOC(size) (CALLOC(1, size))

extern int ALLOCATION_COUNTER;

#else

#define SAFE_FREE(p) do { if (p != NULL) { free(p), p = NULL; } } while (0)
#define CALLOC_TYPE(type) calloc(1, sizeof(type))
#define MALLOC(size) malloc(size)

#endif



#endif
