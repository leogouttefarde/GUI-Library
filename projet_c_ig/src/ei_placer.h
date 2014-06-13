#ifndef EI_PLACER_H
#define EI_PLACER_H

#include "ei_geometrymanager.h"
#include "ei_types.h"
/**
 * \brief       Placer geometry manager
 */
typedef struct ei_placer_param_t {
        ei_geometry_param_t geom;       /* ei_geometry_param_t child */
        ei_anchor_t* anc;
        int* x;
        int* y;
        float* rel_x;
        float* rel_y;
        int* w;
        int* h;
        float* rel_w;
        float* rel_h;
} ei_placer_param_t;


#endif
