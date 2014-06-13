#ifndef EI_PARAMS_H
#define EI_PARAMS_H

// Cet include aurait pu etre supprimé en déclarant ici le type
// ei_geometry_param_t
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


typedef struct ei_gridder_param_t {
        ei_geometry_param_t geom;
        int* lin;
        int* col;
        int* w;
        int* h;
        // Pour forcer la taille de la grille
        int* force_w;
        int* force_h;
        // Booleen pour definir les widgets a recalculer
        ei_bool_t seen;
} ei_gridder_param_t;

#endif
